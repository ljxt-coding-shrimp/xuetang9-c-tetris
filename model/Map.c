/***************************************************
 * 文件名：Map.c
 * 描  述： 地图信息，以及相关处理的实现文件
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/
#include <rpc.h>
#include "Map.h"

/**
 * 向地图中放置方块
 * @param this
 * @param shap
 */
void map_putBlock(PMap this,PShap shap){
    // 清空展示区数据
    memset(this->viewData,0,sizeof(this->viewData));
    int block = shap->trimBlock(shap); // 获取清空后的方块
    int b_width = shap->getWidth(shap); // 获取宽度
    int b_height = shap->getHeight(shap); // 获取高度
    int i , j;
    // 拷贝元素据（将固定区数据放置到展示区中）
    for(i=0;i<this->height;i++){
        for(j=0;j<this->width;j++){
            this->viewData[i][j] = this->data[i][j];
        }
    }
    // 计算满值
    int fullbit = 0;
    for(i=0;i<b_width;i++){
        fullbit <<=1;
        fullbit |= 0x1;
    }
    // 放置方块数据
    for(i=0;i<b_height;i++){
        int bline = block & fullbit; // 获取一行数据
        for(j=0;j<b_width;j++){
            if(bline&0x1  ){ // 当前位有数据的时候，放置相应对的方块到地图展示区数据中
                this->viewData[shap->y + i][shap->x+j].Attributes = (WORD)shap->color; // 设置颜色
                this->viewData[shap->y + i][shap->x+j].Char.AsciiChar = 1; // 设置数据
            }
            bline>>=1;  // 移动到下一位
        }
        block>>=b_width; // 移动到下一行
    }

}
/**
 * 判断一个形状是否能够移动到指定位置
 * @param this  // 地图
 * @param shap  方块
 * @return
 */
short map_moveAble(PMap this,PShap shap){
    int b_width = shap->getWidth(shap); // 形状宽度
    int b_height = shap->getHeight(shap); // 形状高度
    int block = shap->trimBlock(shap); // 清掉多余空白后的方块
    // 计算满值
    int fullbit = 0; //
    int i,j;
    for(i=0;i<b_width;i++){
        fullbit <<=1;
        fullbit |= 0x1;
    }
    // 判断方块位置是有还有其他方块
    for(i=0;i<b_height;i++){
        int bline = block & fullbit; // 获取一行数据
        for(j=0;j<b_width;j++){ //
            if(bline&0x1){ // 如果方块位有数据 ，
                // 地图中形状指定位置如果存在数据，则不可移动
                if(this->data[shap->y + i][shap->x+j].Char.AsciiChar){
                    return 0;
                }
            }
            bline>>=1;
        }
        block>>=b_width;
    }
    // 判断形状是否在地图区域内
    if( shap->x >= 0 && shap->x + b_width <= this->width ){
        if(shap->y  >=  0 && shap->y + b_height <= this->height ){
            return 1;
        }
    }
    return 0;
}

/**
 * 判断形状是否已经触底
 * @param this
 * @param shap
 * @return
 */
short map_shapBottom(PMap this,PShap shap){
    int block = shap->trimBlock(shap); // 形状
    int b_width = shap->getWidth(shap);  // 宽度
    int b_height = shap->getHeight(shap); // 高度

    int i,j;
    // 计算满值
    int fullbit = 0;
    for(i=0;i<b_width;i++){
        fullbit <<= 1;
        fullbit |= 0x1;
    }
    // 判断方块位置
    for(i=0;i<b_height;i++){
        int bline = block & fullbit; // 取出一行的数据
        for(j=0;j<b_width;j++){ // 处理每一行数据
            if(bline&0x1){ // 当前位的下一行有数据 则已经到达底部
                if( this->data[shap->y + i + 1][shap->x+j].Char.AsciiChar ){
                    return 1;
                }
            }
            bline>>=1; //  移动到下一位
        }
        block>>=b_width; // 移动到下一行
    }
    // 到达地图底部
    if(shap->y + b_height >= this->height){
        return 1;
    }
    return 0;
}
/**
 * 重置地图
 * @param this
 * @return
 */
short map_reset(PMap this){
    // 清空固定区的数据
    memset(this->data,0,sizeof(this->data));
    // 已经删除的行数置0
    this->delLine = 0;
}

/**
 * 地图备份
 * @param this
 */
void map_backMap(PMap this){
    // 清空固定区数据
    memset(this->data,0,sizeof(this->data));
    int i , j;
    // 将展示区的数据拷贝到固定区
    for(i=0;i<this->height;i++){
        for(j=0;j<this->width;j++){
            this->data[i][j] = this->viewData[i][j];
        }
    }
}
/**
 * 删除满行
 * @param this
 */
void map_removeLine(PMap this){
    int i,j,blanklines[this->height]; // 循环变量以及满行记录数组
    memset(&blanklines,0,sizeof(blanklines)); // 置空数组
    for(i=0;i<this->height;i++){ // 逐行比对
        short block_size = 0; // 记录当前行的方块数量
        for(j=0;j<this->width;j++){ // 对于行方块的处理
            block_size+=this->data[i][j].Char.AsciiChar;
        }
        if(block_size==this->width){ // 如果宽度和地图宽度一直，则表示本行是满行
            // 记录满列行
            blanklines[i]=1;
        }
    }
    int count = 0;
    // 满行处理
    for(i=this->height-1;i>0;i--){
        // 查找满行
        if(blanklines[i]){
            count++; // 记录行数
            for(j=i;j>1;j--){ // 移动前边的行到当前行
                memcpy(&this->data[j],&this->data[j-1],this->width * sizeof(CHAR_INFO));
                blanklines[j]=blanklines[j-1]; // 满行记录数组也需要相应的移动
            }
            i++;
        }
    }
    if(count){
        this->delLine += (count==1?count:count*2) ;  // 如果满行数大于1 ，删除行数翻倍计算
    }

}

/**
 * 地图构造器
 * @return
 */
PMap map_constructor(){
    // 申请内存空间
    PMap this = malloc(sizeof(Map));
    memset(this,0,sizeof(Map));
    // 相关函数绑定
    this->putBlock = map_putBlock;
    this->moveAble = map_moveAble;
    this->shapBottom = map_shapBottom;
    this->backMap = map_backMap;
    this->removeLine = map_removeLine;
    this->reset = map_reset;
    // 初始化地图宽度，高度，以及删除行
    this->width = 10;
    this->height = 20;
    this->delLine = 0;
    return this;
}
/**
 * 释放地图所占用的空间
 * @param this
 */
void map_finalize(PMap this){
    if(NULL!=this){
        free(this);
    }
}