
/*������ʵ������Ⱥ��hunter��prey��׷�ӹ��̣�������ͼ��Couzinģ��ʵ�ֶ�Ⱥ��ۼ����Ķ����ơ�    */
/*�����е�Couzinģ�Ͳ���Ŀǰȱ��Couzinģ���е�ä�����ã���ΪĿǰ���Դ���Couzinä������Ұ�Ĺ�ϵ��*/
/*׷�����ǽӽ�����ԭ������жԸ��壬�Ҵ������в�ʳ���ơ�                                        */
//YPage��2021.3.7 

//ÿ����������ķ�Ӧ����Ҫ��ȫ�ּ��㣬û�к��ʵĽ������
//����������Ӧ�ò�����㣬����ʵ�ʼ������ʱΪ�˱�֤�±�һ���ԣ���Ȼ���˾��룬�������ж�ʱ���ų���Ӧ�����Ż��취

//��Ϊ������ӻ����ǱȽ�ֱ�۵ķ�ʽ
//��ҪѡȡҪ�о��Ĳ������н�һ���о�


//�������ļ�
#include <graphics.h>		
#include <conio.h>
#include <iostream>
#include"agents.h"
#include"seqAndMin.h"
using namespace std;

//�����������
int preyAlive(prey p[],int preynum) {
	int num = 0;
	for (int i = 0; i < preynum; i++) {
		if (p[i].alive == 1)
			num++;
	}
	return num;
}

int main() {

	//���������
	srand((unsigned)time(NULL));

	//ʱ��(�ò�������)
	int life = 1;

	//��������
	initgraph(500, 500);
	setorigin(0,500);
	setaspectratio(1, -1);


	//����hunterȺ��preyȺ����ʼ��
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

	//����Ⱥ��ÿһ����ִ��˳��
	int seqHunter[HUNTER_NUM];
	int seqPrey[PREY_NUM];

	//��������
	while (preyAlive(prey, PREY_NUM) != 0 && life <= Lifetime_MAX) {

		//����Ⱥ������ж�˳�����
		//ÿ���ж�����ĸı��������ֹ��ɣ�randomAngle�����������GCEAngle�Ǹ���Եж�Ⱥ��ķ�Ӧ��couzinAngle��Ⱥ���ڲ��ĵ���
		//�����Ƕȶ��ǶԵ�ǰλ����˵������ת�ǣ����ǳ���Ƕ�
		//randomTurn����randomAngle��chase&escape����GCEAngle��flock����couzinAngle
		//step���ж�������������������ĸı��Լ��Ƕȵı仯

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
	_getch();				// �����������
	closegraph();			// �رջ�ͼ����


	return 0;

}