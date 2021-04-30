
//两个群体类的父类，定义了 hunter 和 prey 共同的属性和行为

#include"agents.h"
#include"seqAndMin.h"

agent::agent() {
	x = 0;
	y = 0;
	randomAngle = 0;
	towards = 0;
}
agent::~agent() {}

//初始化函数，将个体的位置和初始朝向角随机化（在构造函数中会出现所有个体有相同随机值的情况）
void agent::initialize() {
	x = rand() / (float)RAND_MAX * 100;
	y = rand() / (float)RAND_MAX * 100;
	towards = rand() / (float)RAND_MAX * 2 * PI - PI;
}

//计算每一步的随机角 randomAngle
void agent::randomTurn() {
	randomAngle = rand() / (float)RAND_MAX * RANDOM_ANGLE * 2 - RANDOM_ANGLE;
}

