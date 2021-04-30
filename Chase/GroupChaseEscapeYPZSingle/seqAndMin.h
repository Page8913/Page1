
/*������һЩ�����Ժ������������������seqAndMin.cpp*/

#ifndef _SeqAndMin_H_
#define _SeqAndMin_H_

#include<time.h>
#include<stdlib.h>
#include"agents.h"

//����������еĺ���
void randomSeq(int seq[], int total);

//��¼������Ϣ�������±���Ϣ
struct aMin {
	float distance;
	int index;
};

//�ҵ���С�������
aMin oneMins(float* arr, int len);

//�ҵ�����С����Ұ��ȫ��ͬ��
aMin* couzinPrey(float* arr, int len);
aMin* couzinHunter(float* arr, int len);

//��A����õ�B�����λ�ý�
float getAngle(agent A, agent B);

//�����Ƕȣ�ʹ�ýǶ���[-PI.PI]��
void amendAngle(float& angle);


//ƽ���ı��η����������
vector vectorPlus(vector A, vector B);

void printInf();

#endif