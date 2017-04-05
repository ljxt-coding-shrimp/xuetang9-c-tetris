/***************************************************
 * �ļ�����Draw.h
 * ��  ���� ���ƵĶ����ļ�����Ҫ���������Ƶ���غ����Լ��ṹ��
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
 *
 **************************************************/

#include "../model/Map.h"
#include "../common/WinApiTool.h"

#ifndef TETRIS_DRAW_H
#define TETRIS_DRAW_H

#endif //TETRIS_DRAW_H



typedef struct _Draw{
    /**
     * windows API ���ߵ���
     */
    PWinApiTool apiTool;
    /**
     * ��ʼ�˵�
     */
    void (*startMenu)(struct _Draw *);
    /**
     * ��Ϸ������Ϣ.
     */
    void (*overInfo)(struct _Draw *);
    /**
     * ���Ʒ���
     */
    void (*drawShap)(struct _Draw *,PShap);
    /**
     * ���Ƶ�ͼ
     */
    void (*drawMap)(struct _Draw *,PMap,int);
} Draw , * PDraw;


/**
 * ��ͼ���췽��
 * @return
 */
PDraw draw_constructor();
/**
 * ��ͼ�ͷŷ���
 */
void draw_finalize(PDraw);