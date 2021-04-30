
/*本代码实现了两群体hunter和prey的追逃过程，其中意图以Couzin模型实现对群体聚集与否的而控制。    */
/*代码中的Couzin模型部分目前缺少Couzin模型中的盲区设置，因为目前难以处理Couzin盲区与视野的关系。*/
/*追与逃是接近或者原理最近敌对个体，且代码中有捕食机制。                                        */
//YPage，2021.3.7 

//每个个体对外界的反应都需要做全局计算，没有合适的解决方法
//个体死亡后不应该参与计算，但是实际计算距离时为了保证下标一致性，依然算了距离，但是在判定时被排除。应该有优化办法

//认为加入可视化还是比较直观的方式
//需要选取要研究的参数进行进一步研究


//主函数文件
#include <graphics.h>		
#include <conio.h>
#include <iostream>
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

	//时间(用步数代替)
	int life = 1;

	//创建场地
	initgraph(500, 500);
	setorigin(0,500);
	setaspectratio(1, -1);


	//建立hunter群和prey群并初始化
	hunter hunter[HUNTER_NUM];
	for (int i = 0; i < HUNTER_NUM; i++) {
		hunter[i].initialize();
		setfillcolor(RED);
		solidcircle(hunter[i].x*5, hunter[i].y*5, 2*HUNTER_SIZE);
		
	}
	prey prey[PREY_NUM];
	for (int i = 0; i < PREY_NUM; i++) {
		prey[i].initialize();
		setfillcolor(WHITE);
		solidcircle(prey[i].x*5, prey[i].y*5, 2*PREY_SIZE);
	}
	_getch();
	BeginBatchDraw();

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
			clearcircle(hunter[seqHunter[i]].x*5 , hunter[seqHunter[i]].y*5 , 2*HUNTER_SIZE);
			hunter[seqHunter[i]].step(SPEED);
			setfillcolor(RED);
			solidcircle(hunter[seqHunter[i]].x*5, hunter[seqHunter[i]].y*5, 2 * HUNTER_SIZE);
			FlushBatchDraw();
			//Sleep(1);
		}
		//prey
		randomSeq(seqPrey, PREY_NUM);
		for (int i = 0; i < PREY_NUM; i++) {
			if (prey[seqPrey[i]].alive == 1) {
				prey[seqPrey[i]].lifetime++;
				prey[seqPrey[i]].randomTurn();
				prey[seqPrey[i]].escape(hunter, HUNTER_NUM);
				prey[seqPrey[i]].flock(prey, PREY_NUM);
				clearcircle(prey[seqPrey[i]].x*5 , prey[seqPrey[i]].y*5 , 2*PREY_SIZE);
				prey[seqPrey[i]].step(SPEED);
				setfillcolor(WHITE);
				solidcircle(prey[seqPrey[i]].x *5, prey[seqPrey[i]].y *5, 2* PREY_SIZE);
				FlushBatchDraw();
				//Sleep(1);
			}
			else {
				clearcircle(prey[seqPrey[i]].x * 5, prey[seqPrey[i]].y * 5, 2 * PREY_SIZE);
				FlushBatchDraw();
			}
		}
		life++;
	}

	EndBatchDraw();
	if (preyAlive(prey, PREY_NUM) == 0) {
		saveimage(_T("C:\\Users\\Page\\Desktop\\trace.jpg"));
	}
	_getch();				// 按任意键继续
	closegraph();			// 关闭绘图窗口


	return 0;

}