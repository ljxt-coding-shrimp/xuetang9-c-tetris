/***************************************************
 * �ļ�����WinApiTool.h
 * ��  ���� windows Api ��ز������߷�װ�����ļ�
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
 *
 **************************************************/

#ifndef TETRIS_WINAPITOOL_H
#define TETRIS_WINAPITOOL_H
#endif //TETRIS_WINAPITOOL_H
#include <windows.h>

typedef struct _WinApiTool{
    /**
     * ��׼������
     */
    HANDLE h_stdout;
    /**
     * ������
     */
    HANDLE h_bufout;
    /**
     * ����
     */
    COORD coord;
    /**
     * ��䳤��
     */
    DWORD size;
    /**
     * ����̨��������
     */
    CHAR_INFO * data;
    /**
     * ��������ݳ���
     */
     int buffSize;
    /**
     * �����������
     */
     SMALL_RECT sreact_read;
     /**
      * ����д������
      */
     SMALL_RECT sreact_write;
    /**
     * ��������
     */
     COORD coord_buf_size;
     /**
      * ��ʼ����
      */
     COORD coord_buf_coord;
    /**
     * ��Ļ����
     */
    PCONSOLE_SCREEN_BUFFER_INFO screenBuf;
    /**
     * ��ʼ������
     */
    void (*init)(struct _WinApiTool *);
    /**
     * �ص����
     */
    void (*toStart)(struct _WinApiTool *);
    /**
     * �������
     */
    void (*outFlush)(struct _WinApiTool *);
    /**
     * ������
     */
    void (*fillOut)(struct _WinApiTool *);

}WinApiTool,* PWinApiTool;

PWinApiTool winapitool_constructor();

void winapitool_finalize(PWinApiTool);