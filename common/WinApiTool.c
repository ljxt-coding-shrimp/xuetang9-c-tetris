/***************************************************
 * �ļ�����WinApiTool.c
 * ��  ���� windows Api ��ز������߷�װʵ��
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
 *
 **************************************************/
#include "WinApiTool.h"
/**
 * ��ʼ������
 * @param this �������߶���
 */
void winapitool_init(PWinApiTool this){
    // �õ���Ļ��׼���
    this->h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    // �����������
    this->coord.X = this->coord.Y = 0;
    // ����������棬������ʵ����Ļ���
    this->h_bufout = CreateConsoleScreenBuffer(
            GENERIC_READ|GENERIC_WRITE,
            FILE_SHARE_READ|FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
    );
    // ���������Ļ���档
    SetConsoleActiveScreenBuffer(this->h_bufout);
    // �����Ϣ
    CONSOLE_CURSOR_INFO cursor_info;
    cursor_info.bVisible = 0; // ����
    cursor_info.dwSize = 1; // �����Ϊ1
    // ���ù����Ϣ
    SetConsoleCursorInfo(this->h_stdout,&cursor_info);
    SetConsoleCursorInfo(this->h_bufout,&cursor_info);
    // Ĭ�ϲ����Ĵ�С��0
    this->size = 0;
    // ��ȡ��Ļ����
    GetConsoleScreenBufferInfo(this->h_stdout,this->screenBuf);
    // ��Ļ��������
    SMALL_RECT read;
    // ��Ļ�������
    SMALL_RECT write;
    // ���ö�д����Ϊ������Ļ
    read = this->screenBuf->srWindow;
    write = this->screenBuf->srWindow;
    this->sreact_read = read;
    this->sreact_write = write;
    // ���û�������
    this->coord_buf_size.X = this->screenBuf->dwSize.X;
    this->coord_buf_size.Y = (SHORT)(read.Bottom * 1 + 1);
    // ���û����������
    this->coord_buf_coord.X = 0;
    this->coord_buf_coord.Y = 0;
    // ������������ݿ��
    this->buffSize = this->coord_buf_size.X * this->coord_buf_size.Y;
    // ���������ַ���Ϣ�ڴ�
    this->data = malloc(sizeof(CHAR_INFO) * this->buffSize);
    // �ڴ���0
    memset(this->data,0,sizeof(CHAR_INFO)*this->buffSize);
    // ���ù�굽���
    this->toStart(this);
}

/**
 * ���ù�굽���
 * @param this
 */
void winapitool_tostart(PWinApiTool this){
    // ��׼����
    SetConsoleCursorPosition(this->h_stdout,this->coord);
    // �������
    SetConsoleCursorPosition(this->h_bufout,this->coord);
}
/**
 * �������
 * @param this
 */
void winapitool_outFill(PWinApiTool this){
    // ���ñ���ɫ����ɫ
    FillConsoleOutputAttribute(this->h_stdout,0xF0,this->buffSize,this->coord,&this->size);
    // ���ո�
    FillConsoleOutputCharacterA(this->h_stdout,' ',this->buffSize,this->coord,&this->size);
    // ���ù�굽���
    this->toStart(this);
}
/**
 * �����Ļ����
 * @param this
 */
void winapitool_outflush(PWinApiTool this){
    // ��ȡ���嵽������
    ReadConsoleOutputA(this->h_stdout,this->data,this->coord_buf_size,this->coord_buf_coord,&this->sreact_read);
    // ����ȡ���Ļ���д������Ļ
    WriteConsoleOutputA(this->h_bufout,this->data,this->coord_buf_size,this->coord_buf_coord,&this->sreact_write);
}

/**
 * api �������߹�����
 * @return
 */
PWinApiTool winapitool_constructor(){
    // �����ڴ�ռ�
    PWinApiTool this = malloc(sizeof(WinApiTool));
    // ��ʼ���ڴ�
    memset(this,0,sizeof(WinApiTool));
    // ������غ���
    this->init = winapitool_init;
    this->toStart = winapitool_tostart;
    this->outFlush = winapitool_outflush;
    this->fillOut = winapitool_outFill;
    // ����
    this->screenBuf = malloc(sizeof(CONSOLE_SCREEN_BUFFER_INFO));
    memset(this->screenBuf,0,sizeof(CONSOLE_SCREEN_BUFFER_INFO));
    this->init(this);
    return this;
}

void winapitool_finalize(PWinApiTool this){
    if(NULL!=this->data){  // �ͷſ���̨��������
        free(this->data);
        this->data = NULL;
    }
    if(NULL!=this->screenBuf){ // �ͷ���Ļ����
        free(this->screenBuf);
        this->screenBuf = NULL;
    }
    free(this); // �ͷŵ�ǰ����
}