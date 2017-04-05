/***************************************************
 * �ļ�����Shap.c
 * ��  ���� ��״�����Լ������ʵ���ļ�
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
 *
 **************************************************/
#include "Shap.h"
#include <stdlib.h>
#include <memory.h>

void shap_change(PShap this){ // ������ת
    if(this->state_index == 3){
        this->state_index = 0;
    }else{
        this->state_index++;
    }
}

void shap_prev(PShap this){ // ��һ������
    if(this->state_index == 0){
        this->state_index = 3;
    }else{
        this->state_index--;
    }
}

void shap_init(PShap this){ // �����ʼ��
    // ��շ�������ռ�õ��ڴ�
    memset(this->shaps,0,sizeof(int)*4);
    // Ĭ��״̬Ϊ0
    this->state_default = 0;
    // ��ת״̬Ϊ0����һ��
    this->state_index = 0;
    this->x = 4;//
    this->y = 0;
    // ��ɫΪ��ɫ
    this->color = 0x0;
}

/**
 * ������ƶ���x���꣬y����
 * @param this
 * @param x
 * @param y
 */
void shap_move(PShap this,int x,int y){
    this->x = x;
    this->y = y;
}

/**
 * ��ǰ������
 * @param this
 * @return
 */
int shap_getWidth(PShap this){
    int width = 0;  // ���鿴��
    int widthLine = 0; // �ϲ��������Ϣ
    int block = this->shaps[this->state_index]; // ��ȡ��ǰ����
    while(block){ // ���м�����
        int line = block & 0xF; // ��ȡһ��
        widthLine |= line; // ��֮ǰ���кϲ���
        block>>=4; //  �ƶ�����һ��
    }
    while(widthLine){// ���ݺϲ�����м�����
        if(widthLine&0x1){ // �����ǰλ��1 ��ռ��һ�����
            width++;
        }
        widthLine>>=1;// �ƶ�����һλ
    }
    return width;
}

/**
 * ��ǰ����߶�
 * @param this
 * @return
 */
int shap_getHeight(PShap this){
    int height = 0;//
    // ȥ����ǰ��״
    int block = this->shaps[this->state_index];
    while(block){ //
        if(block & 0xF){ // ��ȡһ���ж��Ƿ������ݣ�����ռ��һ��
            height++;
        }
        block>>=4; // �ƶ�����һ��
    }
    return height;
}

/**
 * �����ǰ���������кͶ������
 * @param this
 * @return
 */
int shap_trimBlock(PShap this){
    // �õ���ǰ����״
    unsigned int block = (unsigned int)this->nativeBlock(this);
    int width = this->getWidth(this); // ���
    int height = this->getHeight(this); // �߶�
    unsigned int rblock = 0; // ���յõ�����״
    // ���������
    if(height < 4){ // ����4��
        int lcp = block;   // ������״
        while(lcp){ // ���д���
            int rval = lcp & 0xF; //  ��ȡһ��
            if(!rval){ // �����û�з������
                block >>= 4; // ɾ����ǰ����
            }
            lcp >>= 4; // �ƶ�����һ��
        }
    }
    if(width == 4){ //  ����4��
        rblock = block; //  ���صķ�����ǵ�ǰ����
    }else if(width < 4){ // ���������
        int stdLine = 0; // ��׼��
        int blockcp = block; // ���鿽��
        while(blockcp){ // ��4�����ݺϲ���һ�С�
            int line = blockcp & 0xF; // ȡ��һ��
            stdLine |= line; // �ϲ�����׼����
            blockcp>>=4; // �ƶ�����һ��
        }
        int m_right =0; // �����Ҳ�Ŀհ�
        while(stdLine){
            int v =stdLine&0x1; // ȡ����׼���е�һλ����
            if(!v){ // ���Ϊ0  �Ҳ�հ�������
                m_right++;
            }
            stdLine>>=1; // �ƶ�����һλ
        }
        int i=0;
        while(block){ //  �ƶ�����
            int bline = block & 0xF; // ȡ��һ������
            bline >>= m_right;  // ���ƿհ�
            bline <<= (width*i); // �ѵ�ǰ�����ƶ��� ���ڵ��е�λ��
            rblock = rblock | bline; //  ���������������ݺϲ�
            block >>= 4;  // �ƶ�����һ��
            i++;
        }
        block = rblock;
    }
    return rblock;
}
// ��ȡ��ǰ����
int shap_nativeBlock(PShap this){
    return this->shaps[this->state_index];
}

/**
 * ���鹹����
 * @return
 */
PShap shap_constructor(){
    // ��״ռ�ݵ��ڴ���
    int shapSize = sizeof(Shap);
    PShap this = malloc(shapSize);  // ������״�ڴ�
    memset(this,0,shapSize); // �ÿ��ڴ�
    // ����غ���
    this->init = shap_init;
    this->next = shap_change;
    this->prev = shap_prev;
    this->move = shap_move;
    this->getWidth = shap_getWidth;
    this->getHeight = shap_getHeight;
    this->trimBlock = shap_trimBlock;
    this->nativeBlock = shap_nativeBlock;
    // ��ʼ������
    this->init(this);
    return this;
}

void shap_finalize(PShap this){
    free(this); // �ͷŷ���ռ���ڴ�ռ�
    this = NULL;
}