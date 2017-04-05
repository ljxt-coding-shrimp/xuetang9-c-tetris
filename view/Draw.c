/***************************************************
 * 文件名：Draw.c
 * 描  述： 绘制实现文件
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/
#include "Draw.h"
#include <stdio.h>
/**
 * 绘制单个形状
 * @param this
 * @param shap
 */
void draw_shap(PDraw this,PShap shap){
    if(NULL==shap){ // 空形状直接返回
        return;
    }
    // 得到默认方块
    int block = shap->shaps[shap->state_default];
    // 设置白色背景，黑色文字
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xF0);
    // 得到方块的颜色
    int color = shap->color;
    // 定义起始绘制坐标
    COORD pos = {56,4};
    // 设置起始坐标
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos);
    // 已经输出的行数
    int outline = 0;
    int bline; // 一行数据
    // 设置边框颜色
    SetConsoleTextAttribute(this->apiTool->h_stdout,0x40);
    printf("            ");
    int num = 1;
    while(block){ // 逐行处理
        pos.X = 56; // 设置X坐标
        pos.Y++; // Y坐标下移
        SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // 设置坐标
        SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // 设置边框颜色
        printf("  "); // 左侧边框
        outline++;  // 输出的行数据递增
        bline = block & 0xF;  // 获取一行方块的数据
        SetConsoleTextAttribute(this->apiTool->h_stdout,0xF0+shap->color); // 设置颜色位方块的颜色
        while(bline){  // 逐列绘制
            if(bline & 0x1){ // 取一位数据，如果是方块位绘制 方块
                printf("■");
            }else{ // 否则绘制空白
                printf("  ");
            }
            bline>>=1; // 下一位
        }
        pos.X = 66; // 设置右侧坐标
        SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // 边框颜色
        SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // 设置位置
        printf("  "); // 绘制边框
        block >>=4; // 处理下一行
    }
    // 如果小于4行，补齐空行
    if(outline<4){
        int i;
        for(i=0;i<4-outline;i++){ // 空白行处理
            pos.Y++; // Y坐标
            pos.X = 56; // 左侧
            SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // 设置位置
            printf("  "); // 绘制边框
            pos.X = 66; // 右侧
            SetConsoleCursorPosition(this->apiTool->h_stdout,pos);
            printf("  "); // 绘制边框
        }
    }
    // 左侧
    pos.X = 56;
    pos.Y++; // Y坐标位置
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // 下一行坐标
    SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // 边框颜色
    printf("            "); // 绘制底部边框
}

/**
 * 绘制地图，等级，分数信息
 * @param this
 * @param map
 * @param leave
 */
void draw_drawMap(PDraw this,PMap map,int leave){
    // 地图左侧起点
    COORD pos = {30,4};
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // 设置坐标
    SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // 边框颜色
    printf("                        "); // 顶部边框
    int i,j; // 行列循环边框
    for(i=0;i<20;i++){ // 行处理
        pos.Y ++; // X坐标递增
        SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // 设置光标
        SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // 设置边框颜色
        printf("  "); // 绘制左侧边框
        for(j=0;j<10;j++){
            CHAR_INFO now = map->viewData[i][j]; // 当前方块信息
            SetConsoleTextAttribute(this->apiTool->h_stdout,0xF0+now.Attributes); //设置方块颜色
            if(now.Char.AsciiChar){ // 当卡位置有方块，绘制方块
                printf("■");
            }else{ // 否则绘制空白
                printf("  "); // □
            }
        }
        SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // 设置边框颜色
        printf("  "); // 绘制边框
    }
    pos.Y ++; // Y坐标下移
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // 设置位置
    SetConsoleTextAttribute(this->apiTool->h_stdout,0x40); // 设置边框颜色
    printf("                        "); // 绘制底部边框
    pos.X = 58; // 分数坐标设置
    pos.Y = 12;
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xFC); // 设置颜色为红色
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // 移动光标到坐标位置
    printf("分数:%d ",map->delLine * 1000); // 输出分数信息
    pos.Y = 14; // 等级Y坐标
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xFC); // 设置颜色为红色
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos); // 移动光标到坐标位置
    printf("等级:%d ",leave); // 输出等级信息
}

/**
 * 设置开始菜单
 * @param this
 */
void draw_startMenu(PDraw this){
    COORD pos1 = {36,10},pos2={36,12},pos3={36,14}; // 每行坐标
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xF0);// 第一行颜色 ，白底黑字
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos1);  // 第一行光标
    printf("回车开始游戏"); // 第一行信息
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos2); // 光标
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xFC); // 白底红字，提示信息
    printf("注:游戏中空格可以暂停游戏,ESC退出程序"); // 控制键位
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos3); //  第三行光标
    printf("a/<- 左移、d/->右移、s/↓ 下降、w/↑ 变形"); // 操作键位
}

/**
 * 游戏结束信息
 * @param this
 */
void draw_overInfo(PDraw this){
    COORD pos1 = {50,10},pos2={50,12}; // 坐标
    SetConsoleTextAttribute(this->apiTool->h_stdout,0xF0); //颜色
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos1); // 第一行坐标，
    printf("隔屁了..."); // 结束提示
    SetConsoleCursorPosition(this->apiTool->h_stdout,pos2); // 第二行坐标
    printf("回车键回到主菜单."); // 操作提示
}

/**
 * 绘制工具构造器
 * @return
 */
PDraw draw_constructor(){
    // 盛情内存空间
    PDraw this = malloc(sizeof(Draw));
    memset(this,0,sizeof(Draw));
    // 绑定相关函数
    this->drawShap = draw_shap;
    this->drawMap = draw_drawMap;
    this->startMenu = draw_startMenu;
    this->apiTool = winapitool_constructor();
    this->overInfo = draw_overInfo;
    return this;
}
/**
 * 释放绘制工具
 * @param this
 */
void draw_finalize(PDraw this){
    if(this->apiTool!=NULL){ // 释方windows API 工具
        winapitool_finalize(this->apiTool);
    }
    free(this); // 释放自己
    this = NULL;
}