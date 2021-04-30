
/*这里存放一些功能性函数的声明，详情请见seqAndMin.cpp*/

#ifndef _SeqAndMin_H_
#define _SeqAndMin_H_

#include<time.h>
#include<stdlib.h>
#include"agents.h"

//随机打乱序列的函数
void randomSeq(int seq[], int total);

//记录距离信息和数组下标信息
struct aMin {
	float distance;
	int index;
};

//找到最小距离个体
aMin oneMins(float* arr, int len);

//找到距离小于视野的全部同类
aMin* couzinPrey(float* arr, int len);
aMin* couzinHunter(float* arr, int len);

//对A个体得到B个体的位置角
float getAngle(agent A, agent B);

//修正角度，使得角度在[-PI.PI]内
void amendAngle(float& angle);


//平行四边形法则计算向量
vector vectorPlus(vector A, vector B);

void printInf();

#endif