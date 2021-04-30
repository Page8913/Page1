
//提供 GroupChaseEscape 中的个体类的声明,详细定义见 agent.cpp , prey.cpp 和 hunter.cpp

#ifndef _Agents_H_
#define _Agents_H_

#include <stdlib.h>
#include<time.h>
#include<cmath>
#include<iostream>

using namespace std;

#define PI (float)3.1415927	
#define WIDTH 100							 //场地的边长（默认为正方场地）
#define SPEED 1								 //个体行进速度

#define HUNTER_NUM 10					 //hunter数目
#define PREY_NUM 100					 //prey数目

#define RANDOM_ANGLE 0/(float)180*PI		 //随机噪声幅度
#define MAX_TURN 60/(float)180*PI			 //每一步最大偏转角
#define PH_SIGHT 20

#define PREY_SIZE (float)0.5							//prey的大小（半径）
#define PREY_COHERE 20
#define PREY_ALIGN 13						 //prey对齐层外围半径
#define PREY_SEPERATE 2					 //prey排斥层外围半径

#define HUNTER_SIZE (float)0.5						//hunter的大小（半径）
#define HUNTER_COHERE 20
#define HUNTER_ALIGN 13						 //hunter对齐层外围半径
#define HUNTER_SEPERATE 2					 //hunter排斥层外围半径

#define strPH (float)50                         //追逃强度系数
#define strCO (float)3							    //吸引强度系数
#define strAL (float)4.5								//对齐强度系数
#define strSE (float)20								//排斥强度系数	
#define dis (float)1.5								//排斥尺度
#define xgm (float)0.2								//陡峭程度

#define Lifetime_MAX 10000			 //最大生存时间

#define RUNTIME 1000                        //执行次数

//矢量结构
struct vector {
	float size;
	float angle;
};


//个体的父类函数
class agent {
public:
	float x, y;					//个体位置
	float towards;					//个体行进方向
	float randomAngle;				//个体每一步的随机角

	void initialize();				//为了避免所有个体产生时随机值相同，使用另外的函数进行初始化
	void randomTurn();				//计算每一步随机角randomAngle的函数

	agent();
	~agent();
};


class prey;							//一个提前声明



//hunter类，继承 agent
/*GCEAngle 是表达 hunter 与 prey 之间的角度，在 hunter 类中，GCEAngle 为靠近最近邻的 prey 所需转角
											 在 prey 类中，GCEAngle 为远离最近邻的 hunter 所需转角*/
											 //couzinAngle 是 couzin 模型里计算同类之间的吸引，对齐，排斥所需转角
											 //dFlockmates 和 dEnemies 分别计算该个体与敌对个体的距离和邻近同类的距离，每个个体对其他所有个体都进行了计算（不太满意但是没有好的解决办法）
											 //chase 用来计算 GCEAngle，flock 用来计算 couzinAngle

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




//prey类，继承 agent
//alive 用来标识 prey 是否存活
//lifetime用来表示 prey 存活的时间
//escape 用来计算 GCEAngle

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