/***************************************************
 * 文件名：WinApiTool.h
 * 描  述： windows Api 相关操作工具封装定义文件
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/

#ifndef TETRIS_WINAPITOOL_H
#define TETRIS_WINAPITOOL_H
#endif //TETRIS_WINAPITOOL_H
#include <windows.h>

typedef struct _WinApiTool{
    /**
     * 标准输出句柄
     */
    HANDLE h_stdout;
    /**
     * 缓存句柄
     */
    HANDLE h_bufout;
    /**
     * 坐标
     */
    COORD coord;
    /**
     * 填充长度
     */
    DWORD size;
    /**
     * 控制台缓冲数据
     */
    CHAR_INFO * data;
    /**
     * 缓冲的数据长度
     */
     int buffSize;
    /**
     * 缓冲读入区域
     */
     SMALL_RECT sreact_read;
     /**
      * 缓冲写出区域
      */
     SMALL_RECT sreact_write;
    /**
     * 缓冲坐标
     */
     COORD coord_buf_size;
     /**
      * 起始坐标
      */
     COORD coord_buf_coord;
    /**
     * 屏幕缓冲
     */
    PCONSOLE_SCREEN_BUFFER_INFO screenBuf;
    /**
     * 初始化函数
     */
    void (*init)(struct _WinApiTool *);
    /**
     * 回到起点
     */
    void (*toStart)(struct _WinApiTool *);
    /**
     * 输出缓存
     */
    void (*outFlush)(struct _WinApiTool *);
    /**
     * 输出填充
     */
    void (*fillOut)(struct _WinApiTool *);

}WinApiTool,* PWinApiTool;

PWinApiTool winapitool_constructor();

void winapitool_finalize(PWinApiTool);