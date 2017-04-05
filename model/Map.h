/***************************************************
 * �ļ�����Map.h
 * ��  ���� ��ͼ��Ϣ���Լ���ش���Ķ����ļ�
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
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
     * ��ͼ����
     */
    CHAR_INFO data[20][10];
    /**
     * ������ʱ����ʱ����ͼ������
     */
    CHAR_INFO viewData[20][10];
    /**
     * ��ͼ���
     */
    int width;
    /**
     * ��ͼ�߶�
     */
    int height;
    /**
     * ��ɾ�����У�(ʵ��ɾ�������뽱����)
     */
    int delLine;
    /**
     * �ѷ�����õ���ͼ��
     */
    void (*putBlock)(struct _Map *,PShap);
    /**
     * �����Ƿ���Լ����ƶ�
     * @return
     */
    short (*moveAble)(struct _Map *,PShap);
    /**
     * �����Ѿ�����
     * @return
     */
    short (*shapBottom)(struct _Map *,PShap);
    /**
     * ���ݵ�ͼ
     */
    void (*backMap)(struct _Map *);
    /**
     * ���õ�ͼ
     */
    void (*reset)(struct _Map *);
    /**
     * ɾ������
     */
    void (*removeLine)(struct _Map *);

}Map,* PMap;

short map_shapBottom(PMap this,PShap shap);

PMap map_constructor();

void map_finalize(PMap this);