/***************************************************
 * �ļ�����main.c
 * ��  ���� ��Ϸ���ļ���������ں�����������������ʼ��Ϸ
 * �����ˣ��Ͼ�ѧ�á�Ϻ��
 * ��  �ڣ�2017-01-09
 * ��  ����1.0
 * ��  ����http://www.xuetang9.com
 * �ٷ�QQ��614940318���Ͼž���
 *
 **************************************************/
#include "Contorl.h"

int main() {
    // ��������������
    PContorl contorl = contorl_constructor();
    // ��Ϸ�˵�
    contorl->showMenue(contorl);
    // �ͷ���Ϸ��Դ
    contorl_finalize(contorl);

    return 0;
}