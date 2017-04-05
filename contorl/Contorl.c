/***************************************************
 * �ļ�����Contorl.c
 * ��  ���� ����������ͷ�ļ�����Ϸ�߼�ʵ�ֲ���
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
 *
 **************************************************/
#include "Contorl.h"
#include <time.h>
#include <conio.h>

static HANDLE g_hmutex = INVALID_HANDLE_VALUE;

/**
 * �����߳�
 * @param contorlPointer
 * @return
 */
DWORD WINAPI threadBlockDown(LPVOID contorlPointer){
    PContorl this = (PContorl)contorlPointer; // ָ��ǿת
    this->blockDropDownThread(this); //  ���ý����߼�
    return TRUE;
}
/**
 * ��������߳�
 * @param contorlPointer
 * @return
 */
DWORD WINAPI threadReciveInput(LPVOID contorlPointer){
    PContorl this = (PContorl)contorlPointer; // ָ��ǿת
    this->reciveInput(this); // �������봦���߼�
    return TRUE;
}

/**
 * ��Ϸ���
 * @param this
 */
void contorl_startGame(PContorl this){
    int i = 0x7FFFFFFF;
    //contorl_showMenue(this);
    // �����ź���
    g_hmutex = CreateMutexA(NULL,FALSE,"Test");
    if(g_hmutex==NULL){
        return;
    }
    // �������봦���߳�
    DWORD reciveThId;
    HANDLE handlereciveTh =  CreateThread(NULL,0,threadReciveInput,this,0,&reciveThId);
    if(handlereciveTh == NULL){
        return;
    }
    CloseHandle(handlereciveTh); // �����̷߳����źţ��߳̽���ʱ�ɲ���ϵͳ���ͷ��߳�ռ�ݵ���Դ
    // �������齵���߳�
    DWORD downThId ;
    HANDLE handleDownTh = CreateThread(NULL,0,threadBlockDown,this,0,&downThId);
    if(handleDownTh == NULL){
        return;
    }
    CloseHandle(handleDownTh);
    //
    while(i--){ // ʹ��i -- ģʽģ����ѭ������Ҫʱ��Ϊ����IDE �����ѭ��������һ��
        if(NULL==this->nowShap){  // �������������ǰ�������һ������
            this->nowShap = this->nextShap?this->nextShap:this->randShap(this);
            this->nextShap = this->randShap(this);
        }
        // �̻߳����źŵķ��ؽ��
        DWORD waitResult = 0;
        short exit; // �˳���ʶ
        while(NULL!=this->nowShap){
            // ����ź���
            waitResult = WaitForSingleObject(g_hmutex,INFINITE);
            exit = 0; // Ĭ�ϲ��˳�
            switch (waitResult){ // �����ź���ʵ����ز���
                case WAIT_OBJECT_0:  // ����
                    if(this->gameState==3){ // �����Ϸ�Ѿ�������
                        TerminateThread(handleDownTh,0); // ֹͣ�����߳�
                        TerminateThread(handlereciveTh,0); // ֹͣ�����߳�
                        this->apiTool->fillOut(this->apiTool); // ��ʼ����Ļ
                        this->draw->overInfo(this->draw); // �����Ϸ������Ϣ
                        this->apiTool->outFlush(this->apiTool); // չʾ��Ϸ������Ϣ
                        if(getch()==13){ // ���ջس���
                            ReleaseMutex(g_hmutex); // �ͷ��ź���
                            CloseHandle(g_hmutex); // �����ź���������������ϵͳ�ͷ�ռ���ڴ�ռ�
                            return; // �����������ص����˵�
                        }
                    }
                    if(NULL==this->nowShap){ // ������鱻�ÿ��˳�ѭ����
                        exit = 1;
                    }else{ // �������
                        this->apiTool->fillOut(this->apiTool);  // ��ʼ����Ļ
                        this->draw->drawShap(this->draw,this->nextShap); // �����һ����״
                        this->draw->drawMap(this->draw,this->map,this->leave); // ����ͼ��Ϣ���������ȼ���Ϣ
                        this->apiTool->outFlush(this->apiTool); // չʾ�Ѿ�����������Ϣ
                        ReleaseMutex(g_hmutex); // �ͷ��ź���
                    }
                    break;
                case WAIT_ABANDONED: // ����ź������������˳�
                    exit = 1;
                    break;
            }
            if(exit){ // ����˳�ѭ�����ͷ��ź�������ѭ�����ص����˵�
                ReleaseMutex(g_hmutex);
                break;
            }
            Sleep(100); // 100���������һ�λ���
        }
    }
}
/**
 * չʾ�˵�
 * @param this
 */
void contorl_showMenue(PContorl this) {
    int i = 0xFFFFFFFF;
    while(i--){ //
        // ��ʼ�������Ļ
        this->apiTool->fillOut(this->apiTool);
        // ���ƿ�ʼ�˵�
        this->draw->startMenu(this->draw);
        // չʾ��ʼ�˵���Ϣ
        this->apiTool->outFlush(this->apiTool);
        int input = getch(); // �����û�����
        if(input==13){ // �õ��س�������ʼ��Ϸ
            this->gameState = 1; // ��Ϸ״̬����Ϊ��ʼ��Ϸ
            this->startGame(this); // ������Ϸ
        }
    }
}

/**
 * ���䴦��
 * @param this
 */
void contorl_blockDropDown(PContorl this){
    int i=0x7FFFFFFF;
    //�ź���Ϣ
    DWORD waitResult;
    while(i--){
        // �����Ϸ״̬����������Ϸ��100 ����֮�����³���
        if(this->gameState==0 || this->gameState == 2 || this->gameState == 3){
            Sleep(100);
            continue;
        }
        short end = 0;
        // ����ź���
        waitResult = WaitForSingleObject(g_hmutex,INFINITE);
        switch(waitResult){
            case WAIT_OBJECT_0:
                if(this->nowShap == NULL){ // ����Ϊ�գ��ͷ��ź��� ����ʼ��һ��ѭ��
                    ReleaseMutex(g_hmutex); //
                    Sleep(100);
                    continue;
                }
                if(!this->map->moveAble(this->map,this->nowShap)){ // ��Ϸ����
                    this->gameState = 3;
                    this->nowShap->x = 4; // ��ʼ������λ��
                    this->nowShap->y = 0;
                    this->nowShap = NULL; // �ͷſ��Ƶķ���
                    this->map->reset(this->map);
                    ReleaseMutex(g_hmutex);
                    return;
                }

                // ���Ʋ���
                this->nowShap->y++;
                if(this->map->moveAble(this->map,this->nowShap)){  // �ж���һ��λ���Ƿ����
                    this->map->putBlock(this->map,this->nowShap); // չʾ��һ��λ��
                }else{
                    this->nowShap->y--; // �˻�
                }
                // �����ж�
                if(this->map->shapBottom(this->map,this->nowShap)){
                    this->map->backMap(this->map); // ���ݵ�ͼ
                    this->nowShap->x = 4; // ��ʼ������λ��
                    this->nowShap->y = 0;
                    this->map->removeLine(this->map); // �Ƴ�����
                    // ����ȼ�
                    this->leave = this->map->delLine / 20;
                    this->nowShap = NULL; // �ͷſ��Ƶķ���
                }
                ReleaseMutex(g_hmutex); // �ͷ��ź���
                break;
            case WAIT_ABANDONED: // �쳣�źţ�����
                end =1;
                break;
            case WAIT_FAILED:{ // �����쳣������
                int val = GetLastError();
                break;
            }
        }
        if(end){ // ����Ϊ������ ����ѭ��
            break;
        }else{
            int now_s_time = 500 - this->leave * 50; // ���㵱ǰ�ȼ�������ʱ��
            Sleep(now_s_time);
        }
    }
}

/**
 * ������մ���
 * @param this
 */
void contorl_reciveInput(PContorl this){
    int i=0xFFFFFFF;
    while(i--){
        if(!this->inputKey){ // �����һ��û������
            this->inputKey = getch(); // ��������
            this->operatorByInput(this); // ��������ִ����ز���
            this->inputKey = 0; // ���������λΪ0
        }
        Sleep(50); // 50����֮��ִ����һ�β���
        if(this->inputKey == -1){ // ������뱻��Ϊ-1 ,�˳�ѭ������
            break;
        }
    }
}

/**
 * �����λ�Ĳ���
 * @param this
 */
void contorl_operatorByInput(PContorl this){
    // ������ǿո�����˳���/
    if(this->inputKey!=VK_SPACE && this->inputKey != VK_ESCAPE){
        if(this->gameState==0 || this->gameState == 2 || this->gameState == 3){ // �����Ϸ״̬����������Ϸ����������
            return;
        }
    }
    // ��ȡ�ź���
    DWORD wiatresult = WaitForSingleObject(g_hmutex,INFINITE);
    // ���û�����ڲ����ķ���
    if(this->nowShap == NULL){
        ReleaseMutex(g_hmutex); // �ͷ��ź�������ȡ��һ�μ�λ����
        return;
    }
    switch (this->inputKey){
        case 'a':
        case 0x4B: // ����� VK_LEFT ����
            this->nowShap->x--;
            if(this->map->moveAble(this->map,this->nowShap)){
                this->map->putBlock(this->map,this->nowShap);
            }else{
                this->nowShap->x++;
            }
            break;
        case 'd':
        case 0x4D: // �ҷ����
            this->nowShap->x++;
            if(this->map->moveAble(this->map,this->nowShap)){
                this->map->putBlock(this->map,this->nowShap);
            }else{
                this->nowShap->x--;
            }
            break;
        case 'w':
        case 0x48: // �Ϸ����
            this->nowShap->next(this->nowShap);
            if(this->map->moveAble(this->map,this->nowShap)){
                this->map->putBlock(this->map,this->nowShap);
            }else{
                this->nowShap->prev(this->nowShap);
            }
            break;
        case 's':
        case 0x50: // �·����
            this->nowShap->y++;
            if(this->map->moveAble(this->map,this->nowShap)){
                this->map->putBlock(this->map,this->nowShap);
            }else{
                this->nowShap->y--;
            }
            break;
        case VK_SPACE:
            // ��ͣ����, ������Ϸ״̬Ϊ��ͣ
            this->gameState = this->gameState==1 ? 2 : 1;
            break;
        case VK_ESCAPE:
            // �˳���Ϸ
            exit(0);
            break;
    }
    // �ͷ��ź���
    ReleaseMutex(g_hmutex);
    // ��������0
    this->inputKey = 0;
}

/**
 * �����ȡһ������
 * @param this
 * @return
 */
PShap contorl_randShap(PContorl this){
    PShap shap = this->shaps[rand()%BASE_BLOCK_SIZE];
    // �������һ����ɫ
    shap->color = rand()%0xF;
    return shap;
}

/**
 * ��������ʼ��
 * @param this
 */
void contorl_init(PContorl this){
    // ���������
    srand(time(0));
    // ��ʼ�� 7 �ַ��飬ÿ�ַ���ʹ��4����״�����ڱ���
    int blocks [BASE_BLOCK_SIZE][4] = {
            {0xF000,0x8888,0xF000,0x8888}, // I
            {0x311,0x170,0x7400,0x88c0}, // J
            {0xC880,0x8e0,0x1130,0xe200}, // L
            {0x4c80,0x63,0x4c80,0x63},  // S
            {0x2310,0x6c00,0x2310,0x6c00}, //Z
            {0xe40,0x8C8,0x4E,0x1310}, // T
            {0x660,0x660,0x660,0x660} // O
    };
    // ���з���ĳ�ʼ��
    int i;
    for(i=0;i<BASE_BLOCK_SIZE;i++){ // ��ʼ��7�ַ������
        PShap  shap = shap_constructor();
        // ��Ĭ�Ϸ������ݷ��õ���״��
        int j;
        for(j=0;j<4;j++){
            shap->shaps[j] = blocks[i][j];
        }
        // ���������ɫ
        shap->color = rand()%0xF;
        // ��״��ֵ
        this->shaps[i] = shap;
    }
    // ��ͼ��ʼ��
    this->map = map_constructor();
    // ���ƹ��ߵĳ�ʼ��
    this->draw = draw_constructor();
    // API ���ߵĵ���
    this->apiTool = this->draw->apiTool;
    // ��ǰ��������Ϊ��ָ��
    this->nowShap = NULL;
    // ��һ������
    this->nextShap = NULL;
    // �����û�м�λ����
    this->inputKey = 0;
    // Ĭ����Ϸ״̬Ϊδ��ʼ
    this->gameState = 0;
    // Ĭ����Ϸ�ȼ�Ϊ0
    this->leave = 0;
}

/**
 * ������������
 * @return
 */
PContorl contorl_constructor(){
    // ����������ڴ�ռ�
    PContorl this = malloc(sizeof(Contorl));
    // ��ʼ���������ڴ�ռ�
    memset(this,0,sizeof(Contorl));
    // ����ط���
    this->init = contorl_init;
    this->startGame = contorl_startGame;
    this->randShap = contorl_randShap;
    this->blockDropDownThread = contorl_blockDropDown;
    this->reciveInput = contorl_reciveInput;
    this->operatorByInput = contorl_operatorByInput;
    this->showMenue = contorl_showMenue;
    // ���ó�ʼ������
    this->init(this);
    return this;
}

void contorl_finalize(PContorl this){
    if(NULL!=this->map){ // �ͷŵ�ͼ����
        map_finalize(this->map);
    }
    if(NULL!=this->draw){ // �ͷŻ��ƶ���
        draw_finalize(this->draw);
    }
    int i;
    for(i=0;i<BASE_BLOCK_SIZE;i++){ // �ͷŷ���ռ���ڴ����
        shap_finalize(this->shaps[i]);
    }
    free(this); // �ͷŵ�ǰ����
    this = NULL;
}