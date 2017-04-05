/***************************************************
 * 文件名：Map.h
 * 描  述： 地图信息，以及相关处理的定义文件
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/

#ifndef TETRIS_MAP_H
#define TETRIS_MAP_H
#endif //TETRIS_MAP_H

#ifndef SHAP_INCLUDE
#define SHAP_INCLUDE
    #include "Shap.h"
#endif

#include <windows.h>

typedef struct _Map{
    /**
     * 地图数据
     */
    CHAR_INFO data[20][10];
    /**
     * 放置临时方块时，地图的数据
     */
    CHAR_INFO viewData[20][10];
    /**
     * 地图宽度
     */
    int width;
    /**
     * 地图高度
     */
    int height;
    /**
     * 被删除的行，(实际删除的行与奖励行)
     */
    int delLine;
    /**
     * 把方块放置到地图中
     */
    void (*putBlock)(struct _Map *,PShap);
    /**
     * 方块是否可以继续移动
     * @return
     */
    short (*moveAble)(struct _Map *,PShap);
    /**
     * 方块已经触底
     * @return
     */
    short (*shapBottom)(struct _Map *,PShap);
    /**
     * 备份地图
     */
    void (*backMap)(struct _Map *);
    /**
     * 重置地图
     */
    void (*reset)(struct _Map *);
    /**
     * 删除空行
     */
    void (*removeLine)(struct _Map *);

}Map,* PMap;

short map_shapBottom(PMap this,PShap shap);

PMap map_constructor();

void map_finalize(PMap this);