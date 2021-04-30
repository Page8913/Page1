
//�ṩ GroupChaseEscape �еĸ����������,��ϸ����� agent.cpp , prey.cpp �� hunter.cpp

#ifndef _Agents_H_
#define _Agents_H_

#include <stdlib.h>
#include<time.h>
#include<cmath>
#include<iostream>

using namespace std;

#define PI (float)3.1415927	
#define WIDTH 100							 //���صı߳���Ĭ��Ϊ�������أ�
#define SPEED 1								 //�����н��ٶ�

#define HUNTER_NUM 10					 //hunter��Ŀ
#define PREY_NUM 100					 //prey��Ŀ

#define RANDOM_ANGLE 0/(float)180*PI		 //�����������
#define MAX_TURN 60/(float)180*PI			 //ÿһ�����ƫת��
#define PH_SIGHT 20

#define PREY_SIZE (float)0.5							//prey�Ĵ�С���뾶��
#define PREY_COHERE 20
#define PREY_ALIGN 13						 //prey�������Χ�뾶
#define PREY_SEPERATE 2					 //prey�ų����Χ�뾶

#define HUNTER_SIZE (float)0.5						//hunter�Ĵ�С���뾶��
#define HUNTER_COHERE 20
#define HUNTER_ALIGN 13						 //hunter�������Χ�뾶
#define HUNTER_SEPERATE 2					 //hunter�ų����Χ�뾶

#define strPH (float)50                         //׷��ǿ��ϵ��
#define strCO (float)3							    //����ǿ��ϵ��
#define strAL (float)4.5								//����ǿ��ϵ��
#define strSE (float)20								//�ų�ǿ��ϵ��	
#define dis (float)1.5								//�ų�߶�
#define xgm (float)0.2								//���ͳ̶�

#define Lifetime_MAX 10000			 //�������ʱ��

#define RUNTIME 1000                        //ִ�д���

//ʸ���ṹ
struct vector {
	float size;
	float angle;
};


//����ĸ��ຯ��
class agent {
public:
	float x, y;					//����λ��
	float towards;					//�����н�����
	float randomAngle;				//����ÿһ���������

	void initialize();				//Ϊ�˱������и������ʱ���ֵ��ͬ��ʹ������ĺ������г�ʼ��
	void randomTurn();				//����ÿһ�������randomAngle�ĺ���

	agent();
	~agent();
};


class prey;							//һ����ǰ����



//hunter�࣬�̳� agent
/*GCEAngle �Ǳ�� hunter �� prey ֮��ĽǶȣ��� hunter ���У�GCEAngle Ϊ��������ڵ� prey ����ת��
											 �� prey ���У�GCEAngle ΪԶ������ڵ� hunter ����ת��*/
											 //couzinAngle �� couzin ģ�������ͬ��֮������������룬�ų�����ת��
											 //dFlockmates �� dEnemies �ֱ����ø�����жԸ���ľ�����ڽ�ͬ��ľ��룬ÿ��������������и��嶼�����˼��㣨��̫���⵫��û�кõĽ���취��
											 //chase �������� GCEAngle��flock �������� couzinAngle

class hunter :public agent {
public:
	vector HU;
	vector FL;
	float* dFlockmates;
	float* dEnemies;


	void step(float speed);
	void chase(prey[], int num);
	void flock(hunter[], int num);

	hunter();
	~hunter();
};




//prey�࣬�̳� agent
//alive ������ʶ prey �Ƿ���
//lifetime������ʾ prey ����ʱ��
//escape �������� GCEAngle

class prey :public agent {
public:
	bool alive;
	int lifetime;
	vector ES;
	vector FL;
	float* dFlockmates;
	float* dEnemies;

	void step(float speed);
	void escape(hunter[], int num);
	void flock(prey[], int num);

	prey();
	~prey();
};
#endif 