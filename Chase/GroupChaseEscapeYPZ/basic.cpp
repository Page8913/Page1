
/*������ʵ������Ⱥ��hunter��prey��׷�ӹ��̣�������ͼ��Couzinģ��ʵ�ֶ�Ⱥ��ۼ����Ķ����ơ�    */
/*�����е�Couzinģ�Ͳ���Ŀǰȱ��Couzinģ���е�ä�����ã���ΪĿǰ���Դ���Couzinä������Ұ�Ĺ�ϵ��*/
/*׷�����ǽӽ�����ԭ������жԸ��壬�Ҵ������в�ʳ���ơ�                                        */
//YPage��2021.3.7 

//ÿ����������ķ�Ӧ����Ҫ��ȫ�ּ��㣬û�к��ʵĽ������
//����������Ӧ�ò�����㣬����ʵ�ʼ������ʱΪ�˱�֤�±�һ���ԣ���Ȼ���˾��룬�������ж�ʱ���ų���Ӧ�����Ż��취

//��Ϊ������ӻ����ǱȽ�ֱ�۵ķ�ʽ
//��ҪѡȡҪ�о��Ĳ������н�һ���о�


//�������ļ�

#include<iostream>
#include<fstream>
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

	//�����ļ���
	//outfile1��¼ʵ�����ϸ��Ϣ���ر�ģ��������������Ĵ�����
	//outfile2��¼ÿ��Ⱥ����ʱ��
	ofstream outfile1,outfile2;
	outfile1.open("C:\\Users\\Page\\Desktop\\ProgramC\\02 prey_survivetime.txt", ios::out);
	outfile2.open("C:\\Users\\Page\\Desktop\\ProgramC\\01 prey_lifetime.txt", ios::out|ios::app);

	//outfile1��¼
	outfile1 << "prey��Ŀ: " << PREY_NUM << endl;
	outfile1 << "hunter��Ŀ�� " << HUNTER_NUM << endl;
	outfile1 << "�����������(����): " << RANDOM_ANGLE / PI * (float)180 << endl;
	outfile1 << "ÿһ�����ƫת��: " << MAX_TURN / PI * (float)180 << endl;
	outfile1 << "hunter Couzin�뾶: " << HUNTER_SEPERATE << "  " << HUNTER_ALIGN << "  " << HUNTER_COHERE << endl;
	outfile1 << "prey Couzin�뾶: " << PREY_SEPERATE << "  " << PREY_ALIGN << "  " << PREY_COHERE << endl;
	outfile1 << "ִ�д���: " << RUNTIME << endl;
	

	//ʱ��(�ò�������)
	int life = 1;
	int halftime = 1;
	bool touchIt = 0;

	//��RUNTIME�����������Ĵ���
	int survivetime = 0;
	

	for (int j = 0; j < RUNTIME; j++) {

		//����hunterȺ��preyȺ����ʼ��
		hunter hunter[HUNTER_NUM];
		for (int i = 0; i < HUNTER_NUM; i++) {
			hunter[i].initialize();
		}
		prey prey[PREY_NUM];
		for (int i = 0; i < PREY_NUM; i++) {
			prey[i].initialize();
		}

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
		std:: cout << "�� " << j + 1 << " �Σ�" << life << endl;
		life = 1;
		halftime = 1;
		touchIt = 0;
	}
	outfile1 << "��������: " << survivetime << endl;
	
	outfile1.close();
	outfile2.close();
	return 0;

}