/***************************************************
 * �ļ�����Shap.h
 * ��  ���� ��״����ͷ�ļ�
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
 *
 **************************************************/

#ifndef TETRIS_SHAP_H
#define TETRIS_SHAP_H
#endif //TETRIS_SHAP_H
/**
 *
 */
typedef struct _Shap{
    /**
     * ��״����
     */
    int shaps[4];
    /**
     * Ĭ��״̬
     */
    short state_default;
    /**
     * ��ǰ״̬
     */
    short state_index;
    /**
     * x����
     */
    int x;
    /**
     * y����
     */
    int y;

    /**
     * ��ɫ
     */
    int color;
    /**
     * ԭ���ķ���
     * @return
     */
    int (*nativeBlock)(struct _Shap *);
    /**
     * ������
     * @return
     */
    int (*getWidth)(struct _Shap *);
    /**
     * ����߶�
     * @return
     */
    int (*getHeight)(struct _Shap *);
    /**
     * ����ո��ķ���
     * @return
     */
    int (*trimBlock)(struct _Shap *);
    /**
     * ����
     */
    void (*next)(struct _Shap *);
    /**
     * ��һ����״
     */
    void (*prev)(struct _Shap *);
    /**
     * ��ʼ��
     */
    void (*init)(struct _Shap *);
    /**
     * �ƶ�
     */
    void (*move)(struct _Shap *);

} Shap,* PShap;

/**
 * ������
 * @return
 */
int shap_getWidth(PShap);
/**
 * ����߶�
 * @return
 */
int shap_getHeight(PShap);
/**
 * ����ո�
 * @return
 */
int shap_trimBlock(PShap);
/**
 * �����ʼ��
 */
void shap_init(PShap);
/**
 * ����
 */
void shap_change(PShap);
/**
 * �ƶ�
 */
void shap_move(PShap,int,int);

/**
 * ��ȡδ������ķ���
 * @return
 */
int shap_nativeBlock(PShap);
/**
 * ���캯��
 * @return
 */
PShap shap_constructor();
/**
 * �ͷź���
 */
void shap_finalize(PShap);