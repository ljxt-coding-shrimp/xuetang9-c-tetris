/***************************************************
 * 文件名：WinApiTool.c
 * 描  述： windows Api 相关操作工具封装实现
 * 创建人：老九学堂·虾米
 * 日  期：2017-01-09
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/
#include "WinApiTool.h"
/**
 * 初始化函数
 * @param this 帮助工具对象
 */
void winapitool_init(PWinApiTool this){
    // 得到屏幕标准输出
    this->h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    // 设置起点坐标
    this->coord.X = this->coord.Y = 0;
    // 创建输出缓存，用于真实的屏幕输出
    this->h_bufout = CreateConsoleScreenBuffer(
            GENERIC_READ|GENERIC_WRITE,
            FILE_SHARE_READ|FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
    );
    // 激活创建的屏幕缓存。
    SetConsoleActiveScreenBuffer(this->h_bufout);
    // 光标信息
    CONSOLE_CURSOR_INFO cursor_info;
    cursor_info.bVisible = 0; // 隐藏
    cursor_info.dwSize = 1; // 光标宽度为1
    // 设置光标信息
    SetConsoleCursorInfo(this->h_stdout,&cursor_info);
    SetConsoleCursorInfo(this->h_bufout,&cursor_info);
    // 默认产生的大小是0
    this->size = 0;
    // 获取屏幕缓存
    GetConsoleScreenBufferInfo(this->h_stdout,this->screenBuf);
    // 屏幕读入区域
    SMALL_RECT read;
    // 屏幕输出区域
    SMALL_RECT write;
    // 设置读写区域为整个屏幕
    read = this->screenBuf->srWindow;
    write = this->screenBuf->srWindow;
    this->sreact_read = read;
    this->sreact_write = write;
    // 设置缓存坐标
    this->coord_buf_size.X = this->screenBuf->dwSize.X;
    this->coord_buf_size.Y = (SHORT)(read.Bottom * 1 + 1);
    // 设置缓存起点坐标
    this->coord_buf_coord.X = 0;
    this->coord_buf_coord.Y = 0;
    // 计算操作的数据宽度
    this->buffSize = this->coord_buf_size.X * this->coord_buf_size.Y;
    // 申请整屏字符信息内存
    this->data = malloc(sizeof(CHAR_INFO) * this->buffSize);
    // 内存置0
    memset(this->data,0,sizeof(CHAR_INFO)*this->buffSize);
    // 设置光标到起点
    this->toStart(this);
}

/**
 * 设置光标到起点
 * @param this
 */
void winapitool_tostart(PWinApiTool this){
    // 标准缓存
    SetConsoleCursorPosition(this->h_stdout,this->coord);
    // 输出缓存
    SetConsoleCursorPosition(this->h_bufout,this->coord);
}
/**
 * 背景填充
 * @param this
 */
void winapitool_outFill(PWinApiTool this){
    // 设置背景色，白色
    FillConsoleOutputAttribute(this->h_stdout,0xF0,this->buffSize,this->coord,&this->size);
    // 填充空格
    FillConsoleOutputCharacterA(this->h_stdout,' ',this->buffSize,this->coord,&this->size);
    // 设置光标到起点
    this->toStart(this);
}
/**
 * 输出屏幕缓冲
 * @param this
 */
void winapitool_outflush(PWinApiTool this){
    // 读取缓冲到数据区
    ReadConsoleOutputA(this->h_stdout,this->data,this->coord_buf_size,this->coord_buf_coord,&this->sreact_read);
    // 将读取到的缓冲写出到屏幕
    WriteConsoleOutputA(this->h_bufout,this->data,this->coord_buf_size,this->coord_buf_coord,&this->sreact_write);
}

/**
 * api 帮助工具构造器
 * @return
 */
PWinApiTool winapitool_constructor(){
    // 申请内存空间
    PWinApiTool this = malloc(sizeof(WinApiTool));
    // 初始化内存
    memset(this,0,sizeof(WinApiTool));
    // 关联相关函数
    this->init = winapitool_init;
    this->toStart = winapitool_tostart;
    this->outFlush = winapitool_outflush;
    this->fillOut = winapitool_outFill;
    // 申请
    this->screenBuf = malloc(sizeof(CONSOLE_SCREEN_BUFFER_INFO));
    memset(this->screenBuf,0,sizeof(CONSOLE_SCREEN_BUFFER_INFO));
    this->init(this);
    return this;
}

void winapitool_finalize(PWinApiTool this){
    if(NULL!=this->data){  // 释放控制台缓冲数据
        free(this->data);
        this->data = NULL;
    }
    if(NULL!=this->screenBuf){ // 释放屏幕缓存
        free(this->screenBuf);
        this->screenBuf = NULL;
    }
    free(this); // 释放当前对象
}