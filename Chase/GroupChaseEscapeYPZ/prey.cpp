
/*��¼��prey����ķ���*/
/*���ڵж�Ⱥ�����Ľ��������þͽ�׷�ӣ�����׷������ڵĵ���*/
/*����ͬȺ�����Ľ�����Ӧ��Couzinģ�ͣ����Ƹ������Ϣ������ۼ��벻�ۼ�*/

#include"agents.h"
#include"seqAndMin.h"

//���캯����������������׸��
prey::prey() {
	alive = 1;
	lifetime = 0;
	ES.angle = 0;
	ES.size = 0;
	FL.angle = 0;
	FL.size = 0;

	dFlockmates = new float[PREY_NUM];
	if (dFlockmates == NULL) {
		cout << "flockmates���鴴��ʧ��" << endl;
	}

	dEnemies = new float[HUNTER_NUM];
	if (dEnemies == NULL) {
		cout << "enemies���鴴��ʧ��" << endl;
	}
}
prey::~prey() {
	delete[]dFlockmates;
	delete[]dEnemies;
}

//escape��prey�Բ�ʳ�ߵ�������Ϊ���˺�����������GCEAngle
void prey::escape(hunter h[], int num) {

	//ֻ�л��ŵ�prey�Ż��м���
	if (alive == 1) {
		//�����prey������hunter�ľ���
		for (int i = 0; i < num; i++) {
			float minX = fabs(x - h[i].x) < WIDTH / 2 ? fabs(x - h[i].x) : WIDTH - fabs(x - h[i].x);
			float minY = fabs(y - h[i].y) < WIDTH / 2 ? fabs(y - h[i].y) : WIDTH - fabs(y - h[i].y);
			dEnemies[i] = sqrt(pow(minX, 2) + pow(minY, 2));
		}

		//oneMin���ؽṹ��aMin��ӵ�ж�Ӧ���˶Ե�ǰ�������distance���͵��˵��±���Ϣ
		aMin minE = oneMins(dEnemies, num);

		if (minE.distance < PH_SIGHT) {
			//�����߾���С��EARSPACEʱ��prey��Ϊ��hunter�Ե����Ӵ˲��������
			if (minE.distance < PREY_SIZE + HUNTER_SIZE)
				alive = 0;
			else {
				ES.size = strPH;
				ES.angle = PI + getAngle(*this, h[minE.index]);
				amendAngle(ES.angle);
			}
		}
		else {
			ES.angle = 0;
			ES.size = 0;
		}
	}
}

//couzinģ�͵�ʹ��
void prey::flock(prey p[], int num) {

	if (alive == 1) {
		//����ͬ���ľ���
		for (int i = 0; i < num; i++) {
			if (p[i].alive == 1) {
				float minX = fabs(x - p[i].x) < WIDTH / 2 ? fabs(x - p[i].x) : WIDTH - fabs(x - p[i].x);
				float minY = fabs(y - p[i].y) < WIDTH / 2 ? fabs(y - p[i].y) : WIDTH - fabs(y - p[i].y);
				dFlockmates[i] = sqrt(pow(minX, 2) + pow(minY, 2));
			}
			else
				dFlockmates[i] = 0;
		}

		//���ֻ��һ�����壬������couzin����
		if (num != 1) {

			//ind1��ind2��Ϊ�˱������ѭ��������ָ��
			//count����������Ұ��Χ�ڵĸ�����
			//ncor��ncoo��ncoa�ֱ�Ϊ���ų�㣬����㣬�������ͬ����Ŀ
			int ind1 = 0;
			int count = 0;
			int ncor = 0, ncoo = 0, ncoa = 0;

			//pin��ָ��aMin�����ָ�룬couzinAgent�ṩ�������ڸø�����Ұ��Χ�ڵ�ͬ��
			aMin* pin = couzinPrey(dFlockmates, num);

			vector couzinCor, couzinCoo, couzinCoa;
			couzinCor.size = 0;
			couzinCor.angle = 0;
			couzinCoo.size = 0;
			couzinCoo.angle = 0;
			couzinCoa.size = 0;
			couzinCoa.angle = 0;

			//couzinAgent�޷�ȷ����С�������ڷ�������������ӱ�־λ�����ж�
			//��ѭ�������õ�����ͬ������������ͬѭ���Ĵ��۷��㽨����̬����
			while (pin[count].index != -1) {
				if (pin[count].distance < PREY_SEPERATE) {
					ncor++;
				}
				else if (pin[count].distance < PREY_ALIGN) {
					ncoo++;
				}
				else
					ncoa++;
				count++;
			}


			//�ų��
			if (ncor > 0) {
				vector* cor = new vector[ncor];
				cor[0].size = 0;
				cor[0].angle = 0;
				for (int i = 0; i < count; i++) {
					if (ind1 == ncor)
						break;
					if (pin[i].distance < PREY_SEPERATE) {
						cor[ind1].angle = PI + getAngle(*this, p[pin[i].index]);
						amendAngle(cor[ind1].angle);
						cor[ind1].size = strSE * (1 / (1 + exp((pin[i].distance - dis) / xgm)));
						ind1++;
					}
				}
				if (ncor > 1) {
					for (int i = 0; i < ncor - 1; i++)
						cor[0] = vectorPlus(cor[0], cor[i + 1]);

				}
				couzinCor = cor[0];
				delete[]cor;
			}
			else {
				couzinCor.angle = 0;
				couzinCor.size = 0;
			}

			ind1 = 0;

			//�����
			if (ncoo > 0) {
				vector* coo = new vector[ncoo];
				coo[0].size = 0;
				coo[0].angle = 0;
				for (int i = 0; i < count; i++) {
					if (ind1 == ncoo)
						break;
					if (pin[i].distance < PREY_ALIGN && pin[i].distance >= PREY_SEPERATE) {
						coo[ind1].angle = p[pin[i].index].towards;
						coo[ind1].size = strAL;
						ind1++;
					}
				}
				if (ncoo > 1) {
					for (int i = 0; i < ncoo - 1; i++)
						coo[0] = vectorPlus(coo[0], coo[i + 1]);

				}
				couzinCoo = coo[0];
				delete[]coo;
			}
			else {
				couzinCoo.size = 0;
				couzinCoo.angle = 0;
			}

			ind1 = 0;

			//������
			if (ncoa > 0) {
				vector* coa = new vector[ncoa];
				coa[0].size = 0;
				coa[0].angle = 0;
				for (int i = 0; i < count; i++) {
					if (ind1 == ncoa)
						break;
					if (pin[i].distance < PREY_COHERE && pin[i].distance >= PREY_ALIGN) {
						coa[ind1].angle = getAngle(*this, p[pin[i].index]);
						coa[ind1].size = strCO;
						ind1++;
					}
				}
				if (ncoa > 1) {
					for (int i = 0; i < ncoa - 1; i++)
						coa[0] = vectorPlus(coa[0], coa[i + 1]);

				}
				couzinCoa = coa[0];
				delete[]coa;
			}
			else {
				couzinCoa.angle = 0;
				couzinCoa.size = 0;
			}

			FL = vectorPlus(vectorPlus(couzinCoa, couzinCoo), couzinCor);
			delete[] pin;
		}
	}
};

//ÿһ����λ�ñ仯�ͽǶȱ仯����
void prey::step(float speed) {
	x += cos(towards) * speed;
	if (x >= WIDTH)
		x -= WIDTH;
	else if (x < 0)
		x += WIDTH;
	y += sin(towards) * speed;
	if (y >= WIDTH)
		y -= WIDTH;
	else if (y < 0)
		y += WIDTH;

	vector sumVector = vectorPlus(ES, FL);
	if (sumVector.size == 0 || fabs(sumVector.size) < 1e-6) {
		towards += randomAngle;
		amendAngle(towards);
	}
	else {
		sumVector.angle += randomAngle;
		amendAngle(sumVector.angle);

		//dAΪ�ܵ�ת�Ǵ�С


		float dA = sumVector.angle - towards;
		amendAngle(dA);

		//dA�����ת���ж�
		if (dA > MAX_TURN) {
			dA = MAX_TURN;
		}
		else if (dA < -MAX_TURN) {
			dA = -MAX_TURN;
		}
		towards += dA;
		while (towards<-PI || towards>PI) {
			amendAngle(towards);
		}
	}
}