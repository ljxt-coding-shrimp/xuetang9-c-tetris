/***************************************************
 * �ļ�����Map.c
 * ��  ���� ��ͼ��Ϣ���Լ���ش����ʵ���ļ�
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
 *
 **************************************************/
#include <rpc.h>
#include "Map.h"

/**
 * ���ͼ�з��÷���
 * @param this
 * @param shap
 */
void map_putBlock(PMap this,PShap shap){
    // ���չʾ������
    memset(this->viewData,0,sizeof(this->viewData));
    int block = shap->trimBlock(shap); // ��ȡ��պ�ķ���
    int b_width = shap->getWidth(shap); // ��ȡ���
    int b_height = shap->getHeight(shap); // ��ȡ�߶�
    int i , j;
    // ����Ԫ�ؾݣ����̶������ݷ��õ�չʾ���У�
    for(i=0;i<this->height;i++){
        for(j=0;j<this->width;j++){
            this->viewData[i][j] = this->data[i][j];
        }
    }
    // ������ֵ
    int fullbit = 0;
    for(i=0;i<b_width;i++){
        fullbit <<=1;
        fullbit |= 0x1;
    }
    // ���÷�������
    for(i=0;i<b_height;i++){
        int bline = block & fullbit; // ��ȡһ������
        for(j=0;j<b_width;j++){
            if(bline&0x1  ){ // ��ǰλ�����ݵ�ʱ�򣬷�����Ӧ�Եķ��鵽��ͼչʾ��������
                this->viewData[shap->y + i][shap->x+j].Attributes = (WORD)shap->color; // ������ɫ
                this->viewData[shap->y + i][shap->x+j].Char.AsciiChar = 1; // ��������
            }
            bline>>=1;  // �ƶ�����һλ
        }
        block>>=b_width; // �ƶ�����һ��
    }

}
/**
 * �ж�һ����״�Ƿ��ܹ��ƶ���ָ��λ��
 * @param this  // ��ͼ
 * @param shap  ����
 * @return
 */
short map_moveAble(PMap this,PShap shap){
    int b_width = shap->getWidth(shap); // ��״���
    int b_height = shap->getHeight(shap); // ��״�߶�
    int block = shap->trimBlock(shap); // �������հ׺�ķ���
    // ������ֵ
    int fullbit = 0; //
    int i,j;
    for(i=0;i<b_width;i++){
        fullbit <<=1;
        fullbit |= 0x1;
    }
    // �жϷ���λ�����л�����������
    for(i=0;i<b_height;i++){
        int bline = block & fullbit; // ��ȡһ������
        for(j=0;j<b_width;j++){ //
            if(bline&0x1){ // �������λ������ ��
                // ��ͼ����״ָ��λ������������ݣ��򲻿��ƶ�
                if(this->data[shap->y + i][shap->x+j].Char.AsciiChar){
                    return 0;
                }
            }
            bline>>=1;
        }
        block>>=b_width;
    }
    // �ж���״�Ƿ��ڵ�ͼ������
    if( shap->x >= 0 && shap->x + b_width <= this->width ){
        if(shap->y  >=  0 && shap->y + b_height <= this->height ){
            return 1;
        }
    }
    return 0;
}

/**
 * �ж���״�Ƿ��Ѿ�����
 * @param this
 * @param shap
 * @return
 */
short map_shapBottom(PMap this,PShap shap){
    int block = shap->trimBlock(shap); // ��״
    int b_width = shap->getWidth(shap);  // ���
    int b_height = shap->getHeight(shap); // �߶�

    int i,j;
    // ������ֵ
    int fullbit = 0;
    for(i=0;i<b_width;i++){
        fullbit <<= 1;
        fullbit |= 0x1;
    }
    // �жϷ���λ��
    for(i=0;i<b_height;i++){
        int bline = block & fullbit; // ȡ��һ�е�����
        for(j=0;j<b_width;j++){ // ����ÿһ������
            if(bline&0x1){ // ��ǰλ����һ�������� ���Ѿ�����ײ�
                if( this->data[shap->y + i + 1][shap->x+j].Char.AsciiChar ){
                    return 1;
                }
            }
            bline>>=1; //  �ƶ�����һλ
        }
        block>>=b_width; // �ƶ�����һ��
    }
    // �����ͼ�ײ�
    if(shap->y + b_height >= this->height){
        return 1;
    }
    return 0;
}
/**
 * ���õ�ͼ
 * @param this
 * @return
 */
short map_reset(PMap this){
    // ��չ̶���������
    memset(this->data,0,sizeof(this->data));
    // �Ѿ�ɾ����������0
    this->delLine = 0;
}

/**
 * ��ͼ����
 * @param this
 */
void map_backMap(PMap this){
    // ��չ̶�������
    memset(this->data,0,sizeof(this->data));
    int i , j;
    // ��չʾ�������ݿ������̶���
    for(i=0;i<this->height;i++){
        for(j=0;j<this->width;j++){
            this->data[i][j] = this->viewData[i][j];
        }
    }
}
/**
 * ɾ������
 * @param this
 */
void map_removeLine(PMap this){
    int i,j,blanklines[this->height]; // ѭ�������Լ����м�¼����
    memset(&blanklines,0,sizeof(blanklines)); // �ÿ�����
    for(i=0;i<this->height;i++){ // ���бȶ�
        short block_size = 0; // ��¼��ǰ�еķ�������
        for(j=0;j<this->width;j++){ // �����з���Ĵ���
            block_size+=this->data[i][j].Char.AsciiChar;
        }
        if(block_size==this->width){ // �����Ⱥ͵�ͼ���һֱ�����ʾ����������
            // ��¼������
            blanklines[i]=1;
        }
    }
    int count = 0;
    // ���д���
    for(i=this->height-1;i>0;i--){
        // ��������
        if(blanklines[i]){
            count++; // ��¼����
            for(j=i;j>1;j--){ // �ƶ�ǰ�ߵ��е���ǰ��
                memcpy(&this->data[j],&this->data[j-1],this->width * sizeof(CHAR_INFO));
                blanklines[j]=blanklines[j-1]; // ���м�¼����Ҳ��Ҫ��Ӧ���ƶ�
            }
            i++;
        }
    }
    if(count){
        this->delLine += (count==1?count:count*2) ;  // �������������1 ��ɾ��������������
    }

}

/**
 * ��ͼ������
 * @return
 */
PMap map_constructor(){
    // �����ڴ�ռ�
    PMap this = malloc(sizeof(Map));
    memset(this,0,sizeof(Map));
    // ��غ�����
    this->putBlock = map_putBlock;
    this->moveAble = map_moveAble;
    this->shapBottom = map_shapBottom;
    this->backMap = map_backMap;
    this->removeLine = map_removeLine;
    this->reset = map_reset;
    // ��ʼ����ͼ��ȣ��߶ȣ��Լ�ɾ����
    this->width = 10;
    this->height = 20;
    this->delLine = 0;
    return this;
}
/**
 * �ͷŵ�ͼ��ռ�õĿռ�
 * @param this
 */
void map_finalize(PMap this){
    if(NULL!=this){
        free(this);
    }
}