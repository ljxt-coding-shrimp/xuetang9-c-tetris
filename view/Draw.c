/***************************************************
 * �ļ�����Draw.c
 * ��  ���� ����ʵ���ļ�
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
 *
 **************************************************/
#include "Draw.h"
#include <stdio.h>
/**
 * ���Ƶ�����״
 * @param this
 * @param shap
 */
void draw_shap(PDraw this,PShap shap){
    if(NULL==shap){ // ����״ֱ�ӷ���
        return;
    }
    // �õ�Ĭ�Ϸ���
    int block = shap->shaps[shap->state_default];
    // ���ð�ɫ��������ɫ����
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xF0);
    // �õ��������ɫ
    int color = shap->color;
    // ������ʼ��������
    COORD pos = {56,4};
    // ������ʼ����
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos);
    // �Ѿ����������
    int outline = 0;
    int bline; // һ������
    // ���ñ߿���ɫ
    SetConsoleTextAttribute(this->apiTool->h_stdout,0x40);
    printf("            ");
    int num = 1;
    while(block){ // ���д���
        pos.X = 56; // ����X����
        pos.Y++; // Y��������
        SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // ��������
        SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // ���ñ߿���ɫ
        printf("  "); // ���߿�
        outline++;  // ����������ݵ���
        bline = block & 0xF;  // ��ȡһ�з��������
        SetConsoleTextAttribute(this->apiTool->h_stdout,0xF0+shap->color); // ������ɫλ�������ɫ
        while(bline){  // ���л���
            if(bline & 0x1){ // ȡһλ���ݣ�����Ƿ���λ���� ����
                printf("��");
            }else{ // ������ƿհ�
                printf("  ");
            }
            bline>>=1; // ��һλ
        }
        pos.X = 66; // �����Ҳ�����
        SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // �߿���ɫ
        SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // ����λ��
        printf("  "); // ���Ʊ߿�
        block >>=4; // ������һ��
    }
    // ���С��4�У��������
    if(outline<4){
        int i;
        for(i=0;i<4-outline;i++){ // �հ��д���
            pos.Y++; // Y����
            pos.X = 56; // ���
            SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // ����λ��
            printf("  "); // ���Ʊ߿�
            pos.X = 66; // �Ҳ�
            SetConsoleCursorPosition(this->apiTool->h_stdout,pos);
            printf("  "); // ���Ʊ߿�
        }
    }
    // ���
    pos.X = 56;
    pos.Y++; // Y����λ��
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // ��һ������
    SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // �߿���ɫ
    printf("            "); // ���Ƶײ��߿�
}

/**
 * ���Ƶ�ͼ���ȼ���������Ϣ
 * @param this
 * @param map
 * @param leave
 */
void draw_drawMap(PDraw this,PMap map,int leave){
    // ��ͼ������
    COORD pos = {30,4};
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // ��������
    SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // �߿���ɫ
    printf("                        "); // �����߿�
    int i,j; // ����ѭ���߿�
    for(i=0;i<20;i++){ // �д���
        pos.Y ++; // X�������
        SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // ���ù��
        SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // ���ñ߿���ɫ
        printf("  "); // �������߿�
        for(j=0;j<10;j++){
            CHAR_INFO now = map->viewData[i][j]; // ��ǰ������Ϣ
            SetConsoleTextAttribute(this->apiTool->h_stdout,0xF0+now.Attributes); //���÷�����ɫ
            if(now.Char.AsciiChar){ // ����λ���з��飬���Ʒ���
                printf("��");
            }else{ // ������ƿհ�
                printf("  "); // ��
            }
        }
        SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // ���ñ߿���ɫ
        printf("  "); // ���Ʊ߿�
    }
    pos.Y ++; // Y��������
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // ����λ��
    SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // ���ñ߿���ɫ
    printf("                        "); // ���Ƶײ��߿�
    pos.X = 58; // ������������
    pos.Y = 12;
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xFC); // ������ɫΪ��ɫ
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // �ƶ���굽����λ��
    printf("����:%d ",map->delLine * 1000); // ���������Ϣ
    pos.Y = 14; // �ȼ�Y����
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xFC); // ������ɫΪ��ɫ
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // �ƶ���굽����λ��
    printf("�ȼ�:%d ",leave); // ����ȼ���Ϣ
}

/**
 * ���ÿ�ʼ�˵�
 * @param this
 */
void draw_startMenu(PDraw this){
    COORD pos1 = {36,10},pos2={36,12},pos3={36,14}; // ÿ������
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xF0);// ��һ����ɫ ���׵׺���
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos1);  // ��һ�й��
    printf("�س���ʼ��Ϸ"); // ��һ����Ϣ
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos2); // ���
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xFC); // �׵׺��֣���ʾ��Ϣ
    printf("ע:��Ϸ�пո������ͣ��Ϸ,ESC�˳�����"); // ���Ƽ�λ
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos3); //  �����й��
    printf("a/<- ���ơ�d/->���ơ�s/�� �½���w/�� ����"); // ������λ
}

/**
 * ��Ϸ������Ϣ
 * @param this
 */
void draw_overInfo(PDraw this){
    COORD pos1 = {50,10},pos2={50,12}; // ����
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xF0); //��ɫ
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos1); // ��һ�����꣬
    printf("��ƨ��..."); // ������ʾ
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos2); // �ڶ�������
    printf("�س����ص����˵�."); // ������ʾ
}

/**
 * ���ƹ��߹�����
 * @return
 */
PDraw draw_constructor(){
    // ʢ���ڴ�ռ�
    PDraw this = malloc(sizeof(Draw));
    memset(this,0,sizeof(Draw));
    // ����غ���
    this->drawShap = draw_shap;
    this->drawMap = draw_drawMap;
    this->startMenu = draw_startMenu;
    this->apiTool = winapitool_constructor();
    this->overInfo = draw_overInfo;
    return this;
}
/**
 * �ͷŻ��ƹ���
 * @param this
 */
void draw_finalize(PDraw this){
    if(this->apiTool!=NULL){ // �ͷ�windows API ����
        winapitool_finalize(this->apiTool);
    }
    free(this); // �ͷ��Լ�
    this = NULL;
}