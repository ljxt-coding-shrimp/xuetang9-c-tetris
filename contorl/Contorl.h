/***************************************************
 * 文件名：Contorl.h
 * 描  述： 控制器定义文件。主要定义相关游戏操作方法
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/

#ifndef TETRIS_CONTORL_H
#define TETRIS_CONTORL_H

#endif //TETRIS_CONTORL_H

#include "../view/Draw.h"

#include <stdlib.h>
#include <memory.h>

#define BASE_BLOCK_SIZE 7

typedef struct _Contorl{
    /**
     * windows API 工具导入
     */
    PWinApiTool apiTool;
    /**
     * 基础方块
     * I / J / L /  O / S / T / Z
     */
    PShap shaps[BASE_BLOCK_SIZE];
    /**
     * 绘制层
     */
    PDraw draw;
    /**
     * 游戏地图
     */
    PMap map;
    /**
     * 当前操作方块
     */
    PShap nowShap;
    /**
     * 下一个方块
     */
    PShap nextShap;
    /**
     * 当前用户输入的键位
     */
    int inputKey;
    /**
     * 游戏状态
     * 0 未开始 / 1 / 游戏中 / 2 暂停 / 3 游戏结束
     */
    int gameState;
    /**
     * 当前等级
     */
    int leave;
    /**
     * 展示菜单
     */
    void (*showMenue)(struct _Contorl *);
    /**
     * 随机获取一个方块
     * @return
     */
    PShap (*randShap)(struct _Control *);
    /**
     * 启动游戏
     */
    void (*startGame)(struct _Contorl *);
    /**
     * 方块掉落线程函数
     */
    void (*blockDropDownThread)(struct _Contorl *);
    /**
     * 接收输入线程函数
     */
    void (*reciveInput)(struct _Control *);
    /**
     * 根据输入操作方块
     */
    void (*operatorByInput)(struct _Control *);
    /**
     * 初始化数据
     */
    void (*init)(struct _Control *);
} Contorl ,* PContorl;

PContorl contorl_constructor();

void contorl_finalize(PContorl);