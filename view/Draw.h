/***************************************************
 * 文件名：Draw.h
 * 描  述： 绘制的定义文件，主要定义界面绘制的相关函数以及结构体
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/

#include "../model/Map.h"
#include "../common/WinApiTool.h"

#ifndef TETRIS_DRAW_H
#define TETRIS_DRAW_H

#endif //TETRIS_DRAW_H



typedef struct _Draw{
    /**
     * windows API 工具导入
     */
    PWinApiTool apiTool;
    /**
     * 开始菜单
     */
    void (*startMenu)(struct _Draw *);
    /**
     * 游戏结束信息.
     */
    void (*overInfo)(struct _Draw *);
    /**
     * 绘制方块
     */
    void (*drawShap)(struct _Draw *,PShap);
    /**
     * 绘制地图
     */
    void (*drawMap)(struct _Draw *,PMap,int);
} Draw , * PDraw;


/**
 * 绘图构造方法
 * @return
 */
PDraw draw_constructor();
/**
 * 绘图释放方法
 */
void draw_finalize(PDraw);