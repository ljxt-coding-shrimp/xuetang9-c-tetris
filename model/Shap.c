/***************************************************
 * 文件名：Shap.c
 * 描  述： 形状描述以及处理的实现文件
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/
#include "Shap.h"
#include <stdlib.h>
#include <memory.h>

void shap_change(PShap this){ // 方块旋转
    if(this->state_index == 3){
        this->state_index = 0;
    }else{
        this->state_index++;
    }
}

void shap_prev(PShap this){ // 上一个方块
    if(this->state_index == 0){
        this->state_index = 3;
    }else{
        this->state_index--;
    }
}

void shap_init(PShap this){ // 方块初始化
    // 清空方块区域占用的内存
    memset(this->shaps,0,sizeof(int)*4);
    // 默认状态为0
    this->state_default = 0;
    // 旋转状态为0，第一个
    this->state_index = 0;
    this->x = 4;//
    this->y = 0;
    // 颜色为黑色
    this->color = 0x0;
}

/**
 * 方块的移动。x坐标，y坐标
 * @param this
 * @param x
 * @param y
 */
void shap_move(PShap this,int x,int y){
    this->x = x;
    this->y = y;
}

/**
 * 当前方块宽度
 * @param this
 * @return
 */
int shap_getWidth(PShap this){
    int width = 0;  // 方块看都
    int widthLine = 0; // 合并后的行信息
    int block = this->shaps[this->state_index]; // 获取当前方块
    while(block){ // 逐行计算宽度
        int line = block & 0xF; // 获取一行
        widthLine |= line; // 与之前的行合并。
        block>>=4; //  移动到下一行
    }
    while(widthLine){// 根据合并后的行计算宽度
        if(widthLine&0x1){ // 如果当前位是1 则占据一个宽度
            width++;
        }
        widthLine>>=1;// 移动到下一位
    }
    return width;
}

/**
 * 当前方块高度
 * @param this
 * @return
 */
int shap_getHeight(PShap this){
    int height = 0;//
    // 去除当前形状
    int block = this->shaps[this->state_index];
    while(block){ //
        if(block & 0xF){ // 获取一行判断是否有数据，有则占据一行
            height++;
        }
        block>>=4; // 移动到下一行
    }
    return height;
}

/**
 * 清除当前方块多余的行和多余的列
 * @param this
 * @return
 */
int shap_trimBlock(PShap this){
    // 得到当前的形状
    unsigned int block = (unsigned int)this->nativeBlock(this);
    int width = this->getWidth(this); // 宽度
    int height = this->getHeight(this); // 高度
    unsigned int rblock = 0; // 最终得到的形状
    // 清理多余行
    if(height < 4){ // 少于4行
        int lcp = block;   // 拷贝形状
        while(lcp){ // 单行存在
            int rval = lcp & 0xF; //  获取一行
            if(!rval){ // 如果行没有方块存在
                block >>= 4; // 删除当前方块
            }
            lcp >>= 4; // 移动到下一行
        }
    }
    if(width == 4){ //  等于4列
        rblock = block; //  返回的方块就是当前方块
    }else if(width < 4){ // 清理多余列
        int stdLine = 0; // 标准行
        int blockcp = block; // 方块拷贝
        while(blockcp){ // 把4行数据合并成一行。
            int line = blockcp & 0xF; // 取出一行
            stdLine |= line; // 合并到标准行中
            blockcp>>=4; // 移动到下一行
        }
        int m_right =0; // 计算右侧的空白
        while(stdLine){
            int v =stdLine&0x1; // 取出标准行中的一位数据
            if(!v){ // 如果为0  右侧空白数递增
                m_right++;
            }
            stdLine>>=1; // 移动到下一位
        }
        int i=0;
        while(block){ //  移动方块
            int bline = block & 0xF; // 取出一行数据
            bline >>= m_right;  // 右移空白
            bline <<= (width*i); // 把当前数据移动到 所在的行的位置
            rblock = rblock | bline; //  行数据与现有数据合并
            block >>= 4;  // 移动到下一行
            i++;
        }
        block = rblock;
    }
    return rblock;
}
// 获取当前方块
int shap_nativeBlock(PShap this){
    return this->shaps[this->state_index];
}

/**
 * 方块构造器
 * @return
 */
PShap shap_constructor(){
    // 形状占据的内存宽度
    int shapSize = sizeof(Shap);
    PShap this = malloc(shapSize);  // 申请形状内存
    memset(this,0,shapSize); // 置空内存
    // 绑定相关函数
    this->init = shap_init;
    this->next = shap_change;
    this->prev = shap_prev;
    this->move = shap_move;
    this->getWidth = shap_getWidth;
    this->getHeight = shap_getHeight;
    this->trimBlock = shap_trimBlock;
    this->nativeBlock = shap_nativeBlock;
    // 初始化方块
    this->init(this);
    return this;
}

void shap_finalize(PShap this){
    free(this); // 释放方块占用内存空间
    this = NULL;
}