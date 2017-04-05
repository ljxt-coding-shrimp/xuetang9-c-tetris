/***************************************************
 * 文件名：main.c
 * 描  述： 游戏主文件。定义入口函数。启动控制器开始游戏
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/
#include "Contorl.h"

int main() {
    // 创建控制器引用
    PContorl contorl = contorl_constructor();
    // 游戏菜单
    contorl->showMenue(contorl);
    // 释放游戏资源
    contorl_finalize(contorl);

    return 0;
}