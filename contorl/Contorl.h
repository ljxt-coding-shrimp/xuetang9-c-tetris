/***************************************************
 * �ļ�����Contorl.h
 * ��  ���� �����������ļ�����Ҫ���������Ϸ��������
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
 *
 **************************************************/

#ifndef TETRIS_CONTORL_H
#define TETRIS_CONTORL_H

#endif //TETRIS_CONTORL_H

#include "../view/Draw.h"

#include <stdlib.h>
#include <memory.h>

#define BASE_BLOCK_SIZE 7

typedef struct _Contorl{
    /**
     * windows API ���ߵ���
     */
    PWinApiTool apiTool;
    /**
     * ��������
     * I / J / L /  O / S / T / Z
     */
    PShap shaps[BASE_BLOCK_SIZE];
    /**
     * ���Ʋ�
     */
    PDraw draw;
    /**
     * ��Ϸ��ͼ
     */
    PMap map;
    /**
     * ��ǰ��������
     */
    PShap nowShap;
    /**
     * ��һ������
     */
    PShap nextShap;
    /**
     * ��ǰ�û�����ļ�λ
     */
    int inputKey;
    /**
     * ��Ϸ״̬
     * 0 δ��ʼ / 1 / ��Ϸ�� / 2 ��ͣ / 3 ��Ϸ����
     */
    int gameState;
    /**
     * ��ǰ�ȼ�
     */
    int leave;
    /**
     * չʾ�˵�
     */
    void (*showMenue)(struct _Contorl *);
    /**
     * �����ȡһ������
     * @return
     */
    PShap (*randShap)(struct _Control *);
    /**
     * ������Ϸ
     */
    void (*startGame)(struct _Contorl *);
    /**
     * ��������̺߳���
     */
    void (*blockDropDownThread)(struct _Contorl *);
    /**
     * ���������̺߳���
     */
    void (*reciveInput)(struct _Control *);
    /**
     * ���������������
     */
    void (*operatorByInput)(struct _Control *);
    /**
     * ��ʼ������
     */
    void (*init)(struct _Control *);
} Contorl ,* PContorl;

PContorl contorl_constructor();

void contorl_finalize(PContorl);