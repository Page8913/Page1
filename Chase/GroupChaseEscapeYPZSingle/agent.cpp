
//����Ⱥ����ĸ��࣬������ hunter �� prey ��ͬ�����Ժ���Ϊ

#include"agents.h"
#include"seqAndMin.h"

agent::agent() {
	x = 0;
	y = 0;
	randomAngle = 0;
	towards = 0;
}
agent::~agent() {}

//��ʼ���������������λ�úͳ�ʼ�������������ڹ��캯���л�������и�������ͬ���ֵ�������
void agent::initialize() {
	x = rand() / (float)RAND_MAX * 100;
	y = rand() / (float)RAND_MAX * 100;
	towards = rand() / (float)RAND_MAX * 2 * PI - PI;
}

//����ÿһ��������� randomAngle
void agent::randomTurn() {
	randomAngle = rand() / (float)RAND_MAX * RANDOM_ANGLE * 2 - RANDOM_ANGLE;
}

