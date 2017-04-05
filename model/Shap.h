/***************************************************
 * 文件名：Shap.h
 * 描  述： 形状描述头文件
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/

#ifndef TETRIS_SHAP_H
#define TETRIS_SHAP_H
#endif //TETRIS_SHAP_H
/**
 *
 */
typedef struct _Shap{
    /**
     * 形状数组
     */
    int shaps[4];
    /**
     * 默认状态
     */
    short state_default;
    /**
     * 当前状态
     */
    short state_index;
    /**
     * x坐标
     */
    int x;
    /**
     * y坐标
     */
    int y;

    /**
     * 颜色
     */
    int color;
    /**
     * 原生的方块
     * @return
     */
    int (*nativeBlock)(struct _Shap *);
    /**
     * 方块宽度
     * @return
     */
    int (*getWidth)(struct _Shap *);
    /**
     * 方块高度
     * @return
     */
    int (*getHeight)(struct _Shap *);
    /**
     * 清理空格后的方块
     * @return
     */
    int (*trimBlock)(struct _Shap *);
    /**
     * 变形
     */
    void (*next)(struct _Shap *);
    /**
     * 上一个形状
     */
    void (*prev)(struct _Shap *);
    /**
     * 初始化
     */
    void (*init)(struct _Shap *);
    /**
     * 移动
     */
    void (*move)(struct _Shap *);

} Shap,* PShap;

/**
 * 方块宽度
 * @return
 */
int shap_getWidth(PShap);
/**
 * 方块高度
 * @return
 */
int shap_getHeight(PShap);
/**
 * 清理空格
 * @return
 */
int shap_trimBlock(PShap);
/**
 * 方块初始化
 */
void shap_init(PShap);
/**
 * 变形
 */
void shap_change(PShap);
/**
 * 移动
 */
void shap_move(PShap,int,int);

/**
 * 获取未处理过的方块
 * @return
 */
int shap_nativeBlock(PShap);
/**
 * 构造函数
 * @return
 */
PShap shap_constructor();
/**
 * 释放函数
 */
void shap_finalize(PShap);