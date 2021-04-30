
/*本代码实现了两群体hunter和prey的追逃过程，其中意图以Couzin模型实现对群体聚集与否的而控制。    */
/*代码中的Couzin模型部分目前缺少Couzin模型中的盲区设置，因为目前难以处理Couzin盲区与视野的关系。*/
/*追与逃是接近或者原理最近敌对个体，且代码中有捕食机制。                                        */
//YPage，2021.3.7 

//每个个体对外界的反应都需要做全局计算，没有合适的解决方法
//个体死亡后不应该参与计算，但是实际计算距离时为了保证下标一致性，依然算了距离，但是在判定时被排除。应该有优化办法

//认为加入可视化还是比较直观的方式
//需要选取要研究的参数进行进一步研究


//主函数文件

#include<iostream>
#include<fstream>
#include"agents.h"
#include"seqAndMin.h"
using namespace std;

//计算存活个体数
int preyAlive(prey p[],int preynum) {
	int num = 0;
	for (int i = 0; i < preynum; i++) {
		if (p[i].alive == 1)
			num++;
	}
	return num;
}

int main() {

	//随机数设置
	srand((unsigned)time(NULL));

	//创建文件流
	//outfile1记录实验的详细信息（特别的：包括出现永生的次数）
	//outfile2记录每次群体存活时间
	ofstream outfile1,outfile2;
	outfile1.open("C:\\Users\\Page\\Desktop\\ProgramC\\02 prey_survivetime.txt", ios::out);
	outfile2.open("C:\\Users\\Page\\Desktop\\ProgramC\\01 prey_lifetime.txt", ios::out|ios::app);

	//outfile1记录
	outfile1 << "prey数目: " << PREY_NUM << endl;
	outfile1 << "hunter数目： " << HUNTER_NUM << endl;
	outfile1 << "随机噪声幅度(度数): " << RANDOM_ANGLE / PI * (float)180 << endl;
	outfile1 << "每一步最大偏转角: " << MAX_TURN / PI * (float)180 << endl;
	outfile1 << "hunter Couzin半径: " << HUNTER_SEPERATE << "  " << HUNTER_ALIGN << "  " << HUNTER_COHERE << endl;
	outfile1 << "prey Couzin半径: " << PREY_SEPERATE << "  " << PREY_ALIGN << "  " << PREY_COHERE << endl;
	outfile1 << "执行次数: " << RUNTIME << endl;
	

	//时间(用步数代替)
	int life = 1;
	int halftime = 1;
	bool touchIt = 0;

	//在RUNTIME次数中永生的次数
	int survivetime = 0;
	

	for (int j = 0; j < RUNTIME; j++) {

		//建立hunter群和prey群并初始化
		hunter hunter[HUNTER_NUM];
		for (int i = 0; i < HUNTER_NUM; i++) {
			hunter[i].initialize();
		}
		prey prey[PREY_NUM];
		for (int i = 0; i < PREY_NUM; i++) {
			prey[i].initialize();
		}

		//两个群体每一步的执行顺序
		int seqHunter[HUNTER_NUM];
		int seqPrey[PREY_NUM];

		//单次运行
		while (preyAlive(prey, PREY_NUM) != 0 && life <= Lifetime_MAX) {

			//两个群体各自行动顺序随机
			//每次行动方向的改变由三部分构成：randomAngle是随机噪声，GCEAngle是个体对敌对群体的反应，couzinAngle是群体内部的调整
			//三个角度都是对当前位置来说的所需转角，并非朝向角度
			//randomTurn计算randomAngle，chase&escape计算GCEAngle，flock计算couzinAngle
			//step是行动函数，包括个体坐标的改变以及角度的变化

			//hunter
			randomSeq(seqHunter, HUNTER_NUM);
			for (int i = 0; i < HUNTER_NUM; i++) {
				hunter[seqHunter[i]].randomTurn();
				hunter[seqHunter[i]].chase(prey, PREY_NUM);
				hunter[seqHunter[i]].flock(hunter, HUNTER_NUM);
				hunter[seqHunter[i]].step(SPEED);
			}
			//prey
			randomSeq(seqPrey, PREY_NUM);
			for (int i = 0; i < PREY_NUM; i++) {
				if (prey[seqPrey[i]].alive == 1) {
					prey[seqPrey[i]].lifetime++;
					prey[seqPrey[i]].randomTurn();
					prey[seqPrey[i]].escape(hunter, HUNTER_NUM);
					prey[seqPrey[i]].flock(prey, PREY_NUM);
					prey[seqPrey[i]].step(SPEED);
				}
			}
			if (preyAlive(prey, PREY_NUM) == PREY_NUM / 2 && touchIt == 0) {
				halftime = life;
				touchIt = 1;
			}
			life++;
		}
		if (life > Lifetime_MAX)
			survivetime++;

		outfile2  << life<<"    " << halftime<< endl;
		std:: cout << "第 " << j + 1 << " 次：" << life << endl;
		life = 1;
		halftime = 1;
		touchIt = 0;
	}
	outfile1 << "永生次数: " << survivetime << endl;
	
	outfile1.close();
	outfile2.close();
	return 0;

}