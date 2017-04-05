/***************************************************
 * 文件名：Contorl.c
 * 描  述： 控制器操作头文件，游戏逻辑实现部分
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/
#include "Contorl.h"
#include <time.h>
#include <conio.h>

static HANDLE g_hmutex = INVALID_HANDLE_VALUE;

/**
 * 降落线程
 * @param contorlPointer
 * @return
 */
DWORD WINAPI threadBlockDown(LPVOID contorlPointer){
    PContorl this = (PContorl)contorlPointer; // 指针强转
    this->blockDropDownThread(this); //  调用降落逻辑
    return TRUE;
}
/**
 * 输入接收线程
 * @param contorlPointer
 * @return
 */
DWORD WINAPI threadReciveInput(LPVOID contorlPointer){
    PContorl this = (PContorl)contorlPointer; // 指针强转
    this->reciveInput(this); // 调用输入处理逻辑
    return TRUE;
}

/**
 * 游戏入口
 * @param this
 */
void contorl_startGame(PContorl this){
    int i = 0x7FFFFFFF;
    //contorl_showMenue(this);
    // 创建信号量
    g_hmutex = CreateMutexA(NULL,FALSE,"Test");
    if(g_hmutex==NULL){
        return;
    }
    // 创建输入处理线程
    DWORD reciveThId;
    HANDLE handlereciveTh =  CreateThread(NULL,0,threadReciveInput,this,0,&reciveThId);
    if(handlereciveTh == NULL){
        return;
    }
    CloseHandle(handlereciveTh); // 忽略线程返回信号，线程结束时由操作系统来释放线程占据的资源
    // 创建方块降落线程
    DWORD downThId ;
    HANDLE handleDownTh = CreateThread(NULL,0,threadBlockDown,this,0,&downThId);
    if(handleDownTh == NULL){
        return;
    }
    CloseHandle(handleDownTh);
    //
    while(i--){ // 使用i -- 模式模拟死循环。主要时因为讨厌IDE 会把死循环代码变成一块
        if(NULL==this->nowShap){  // 根据情况创建当前方块和下一个方块
            this->nowShap = this->nextShap?this->nextShap:this->randShap(this);
            this->nextShap = this->randShap(this);
        }
        // 线程互斥信号的返回结果
        DWORD waitResult = 0;
        short exit; // 退出标识
        while(NULL!=this->nowShap){
            // 获得信号量
            waitResult = WaitForSingleObject(g_hmutex,INFINITE);
            exit = 0; // 默认不退出
            switch (waitResult){ // 根据信号量实现相关操作
                case WAIT_OBJECT_0:  // 正常
                    if(this->gameState==3){ // 如果游戏已经结束。
                        TerminateThread(handleDownTh,0); // 停止降落线程
                        TerminateThread(handlereciveTh,0); // 停止接收线程
                        this->apiTool->fillOut(this->apiTool); // 初始化屏幕
                        this->draw->overInfo(this->draw); // 输出游戏结束信息
                        this->apiTool->outFlush(this->apiTool); // 展示游戏结束信息
                        if(getch()==13){ // 接收回车键
                            ReleaseMutex(g_hmutex); // 释放信号量
                            CloseHandle(g_hmutex); // 忽略信号量处理。交给操作系统释放占用内存空间
                            return; // 结束方法，回到主菜单
                        }
                    }
                    if(NULL==this->nowShap){ // 如果方块被置空退出循环。
                        exit = 1;
                    }else{ // 方块存在
                        this->apiTool->fillOut(this->apiTool);  // 初始化屏幕
                        this->draw->drawShap(this->draw,this->nextShap); // 描绘下一个形状
                        this->draw->drawMap(this->draw,this->map,this->leave); // 描绘地图信息、分数、等级信息
                        this->apiTool->outFlush(this->apiTool); // 展示已经输出的相关信息
                        ReleaseMutex(g_hmutex); // 释放信号量
                    }
                    break;
                case WAIT_ABANDONED: // 如果信号量被废弃，退出
                    exit = 1;
                    break;
            }
            if(exit){ // 如果退出循环，释放信号量结束循环。回到主菜单
                ReleaseMutex(g_hmutex);
                break;
            }
            Sleep(100); // 100毫秒进行下一次绘制
        }
    }
}
/**
 * 展示菜单
 * @param this
 */
void contorl_showMenue(PContorl this) {
    int i = 0xFFFFFFFF;
    while(i--){ //
        // 初始化输出屏幕
        this->apiTool->fillOut(this->apiTool);
        // 绘制开始菜单
        this->draw->startMenu(this->draw);
        // 展示开始菜单信息
        this->apiTool->outFlush(this->apiTool);
        int input = getch(); // 接收用户输入
        if(input==13){ // 得到回车键。开始游戏
            this->gameState = 1; // 游戏状态设置为开始游戏
            this->startGame(this); // 启动游戏
        }
    }
}

/**
 * 降落处理
 * @param this
 */
void contorl_blockDropDown(PContorl this){
    int i=0x7FFFFFFF;
    //信号信息
    DWORD waitResult;
    while(i--){
        // 如果游戏状态不是正在游戏，100 毫秒之后重新尝试
        if(this->gameState==0 || this->gameState == 2 || this->gameState == 3){
            Sleep(100);
            continue;
        }
        short end = 0;
        // 获得信号量
        waitResult = WaitForSingleObject(g_hmutex,INFINITE);
        switch(waitResult){
            case WAIT_OBJECT_0:
                if(this->nowShap == NULL){ // 方块为空，释放信号量 ，开始下一轮循环
                    ReleaseMutex(g_hmutex); //
                    Sleep(100);
                    continue;
                }
                if(!this->map->moveAble(this->map,this->nowShap)){ // 游戏结束
                    this->gameState = 3;
                    this->nowShap->x = 4; // 初始化方块位置
                    this->nowShap->y = 0;
                    this->nowShap = NULL; // 释放控制的方块
                    this->map->reset(this->map);
                    ReleaseMutex(g_hmutex);
                    return;
                }

                // 下移操作
                this->nowShap->y++;
                if(this->map->moveAble(this->map,this->nowShap)){  // 判断下一个位置是否可用
                    this->map->putBlock(this->map,this->nowShap); // 展示下一个位置
                }else{
                    this->nowShap->y--; // 退回
                }
                // 触底判断
                if(this->map->shapBottom(this->map,this->nowShap)){
                    this->map->backMap(this->map); // 备份地图
                    this->nowShap->x = 4; // 初始化方块位置
                    this->nowShap->y = 0;
                    this->map->removeLine(this->map); // 移除满行
                    // 计算等级
                    this->leave = this->map->delLine / 20;
                    this->nowShap = NULL; // 释放控制的方块
                }
                ReleaseMutex(g_hmutex); // 释放信号量
                break;
            case WAIT_ABANDONED: // 异常信号，结束
                end =1;
                break;
            case WAIT_FAILED:{ // 出现异常。结束
                int val = GetLastError();
                break;
            }
        }
        if(end){ // 被置为结束， 跳出循环
            break;
        }else{
            int now_s_time = 500 - this->leave * 50; // 计算当前等级的休眠时间
            Sleep(now_s_time);
        }
    }
}

/**
 * 输入接收处理
 * @param this
 */
void contorl_reciveInput(PContorl this){
    int i=0xFFFFFFF;
    while(i--){
        if(!this->inputKey){ // 如果上一次没有输入
            this->inputKey = getch(); // 接收输入
            this->operatorByInput(this); // 根据输入执行相关操作
            this->inputKey = 0; // 重置输入键位为0
        }
        Sleep(50); // 50毫秒之后，执行下一次操作
        if(this->inputKey == -1){ // 如果输入被置为-1 ,退出循环处理
            break;
        }
    }
}

/**
 * 输入键位的操作
 * @param this
 */
void contorl_operatorByInput(PContorl this){
    // 如果不是空格键和退出键/
    if(this->inputKey!=VK_SPACE && this->inputKey != VK_ESCAPE){
        if(this->gameState==0 || this->gameState == 2 || this->gameState == 3){ // 如果游戏状态不是正在游戏，忽略输入
            return;
        }
    }
    // 获取信号量
    DWORD wiatresult = WaitForSingleObject(g_hmutex,INFINITE);
    // 如果没有正在操作的方块
    if(this->nowShap == NULL){
        ReleaseMutex(g_hmutex); // 释放信号量，获取下一次键位输入
        return;
    }
    switch (this->inputKey){
        case 'a':
        case 0x4B: // 左方向键 VK_LEFT 左移
            this->nowShap->x--;
            if(this->map->moveAble(this->map,this->nowShap)){
                this->map->putBlock(this->map,this->nowShap);
            }else{
                this->nowShap->x++;
            }
            break;
        case 'd':
        case 0x4D: // 右方向键
            this->nowShap->x++;
            if(this->map->moveAble(this->map,this->nowShap)){
                this->map->putBlock(this->map,this->nowShap);
            }else{
                this->nowShap->x--;
            }
            break;
        case 'w':
        case 0x48: // 上方向键
            this->nowShap->next(this->nowShap);
            if(this->map->moveAble(this->map,this->nowShap)){
                this->map->putBlock(this->map,this->nowShap);
            }else{
                this->nowShap->prev(this->nowShap);
            }
            break;
        case 's':
        case 0x50: // 下方向键
            this->nowShap->y++;
            if(this->map->moveAble(this->map,this->nowShap)){
                this->map->putBlock(this->map,this->nowShap);
            }else{
                this->nowShap->y--;
            }
            break;
        case VK_SPACE:
            // 暂停功能, 设置游戏状态为暂停
            this->gameState = this->gameState==1 ? 2 : 1;
            break;
        case VK_ESCAPE:
            // 退出游戏
            exit(0);
            break;
    }
    // 释放信号量
    ReleaseMutex(g_hmutex);
    // 将输入置0
    this->inputKey = 0;
}

/**
 * 随机获取一个方块
 * @param this
 * @return
 */
PShap contorl_randShap(PContorl this){
    PShap shap = this->shaps[rand()%BASE_BLOCK_SIZE];
    // 随机设置一种颜色
    shap->color = rand()%0xF;
    return shap;
}

/**
 * 控制器初始化
 * @param this
 */
void contorl_init(PContorl this){
    // 随机数种子
    srand(time(0));
    // 初始化 7 种方块，每种方块使用4种形状，便于变形
    int blocks [BASE_BLOCK_SIZE][4] = {
            {0xF000,0x8888,0xF000,0x8888}, // I
            {0x311,0x170,0x7400,0x88c0}, // J
            {0xC880,0x8e0,0x1130,0xe200}, // L
            {0x4c80,0x63,0x4c80,0x63},  // S
            {0x2310,0x6c00,0x2310,0x6c00}, //Z
            {0xe40,0x8C8,0x4E,0x1310}, // T
            {0x660,0x660,0x660,0x660} // O
    };
    // 所有方块的初始化
    int i;
    for(i=0;i<BASE_BLOCK_SIZE;i++){ // 初始化7种方块对象
        PShap  shap = shap_constructor();
        // 把默认方块数据放置到形状中
        int j;
        for(j=0;j<4;j++){
            shap->shaps[j] = blocks[i][j];
        }
        // 随机设置颜色
        shap->color = rand()%0xF;
        // 形状赋值
        this->shaps[i] = shap;
    }
    // 地图初始化
    this->map = map_constructor();
    // 绘制工具的初始化
    this->draw = draw_constructor();
    // API 工具的调用
    this->apiTool = this->draw->apiTool;
    // 当前方块设置为空指针
    this->nowShap = NULL;
    // 下一个方块
    this->nextShap = NULL;
    // 输入键没有键位输入
    this->inputKey = 0;
    // 默认游戏状态为未开始
    this->gameState = 0;
    // 默认游戏等级为0
    this->leave = 0;
}

/**
 * 控制器构造器
 * @return
 */
PContorl contorl_constructor(){
    // 申请控制器内存空间
    PContorl this = malloc(sizeof(Contorl));
    // 初始化控制器内存空间
    memset(this,0,sizeof(Contorl));
    // 绑定相关方法
    this->init = contorl_init;
    this->startGame = contorl_startGame;
    this->randShap = contorl_randShap;
    this->blockDropDownThread = contorl_blockDropDown;
    this->reciveInput = contorl_reciveInput;
    this->operatorByInput = contorl_operatorByInput;
    this->showMenue = contorl_showMenue;
    // 调用初始化方法
    this->init(this);
    return this;
}

void contorl_finalize(PContorl this){
    if(NULL!=this->map){ // 释放地图对象
        map_finalize(this->map);
    }
    if(NULL!=this->draw){ // 释放绘制对象
        draw_finalize(this->draw);
    }
    int i;
    for(i=0;i<BASE_BLOCK_SIZE;i++){ // 释放方块占用内存对象
        shap_finalize(this->shaps[i]);
    }
    free(this); // 释放当前对象
    this = NULL;
}