
/*记录了hunter个体的方法，详情与prey基本相同，见prey.cpp*/

#include"agents.h"
#include"seqAndMin.h"

hunter::hunter() {
	HU.angle = 0;
	HU.size = 0;
	FL.angle = 0;
	FL.size = 0;
	dFlockmates = new float[HUNTER_NUM];
	if (dFlockmates == NULL) {
		cout << "flockmates数组创建失败" << endl;
	}
	dEnemies = new float[PREY_NUM];
	if (dEnemies == NULL) {
		cout << "enemies数组创建失败" << endl;
	}
}
hunter::~hunter() {
	delete[]dFlockmates;
	delete[]dEnemies;
}



void hunter::chase(prey p[], int num) {

	for (int i = 0; i < num; i++) {
		if (p[i].alive == 1) {
			float minX = fabs(x - p[i].x) < WIDTH / 2 ? fabs(x - p[i].x) : WIDTH - fabs(x - p[i].x);
			float minY = fabs(y - p[i].y) < WIDTH / 2 ? fabs(y - p[i].y) : WIDTH - fabs(y - p[i].y);
			dEnemies[i] = sqrt(pow(minX, 2) + pow(minY, 2));
		}
		else
			dEnemies[i] = 0;
	}

	aMin minE = oneMins(dEnemies, num);

	if (minE.distance < PH_SIGHT) {
		HU.angle = getAngle(*this, p[minE.index]);
		HU.size = strPH;
		amendAngle(HU.angle);
	}
}



void hunter::flock(hunter h[], int num) {

	for (int i = 0; i < num; i++) {
		float minX = fabs(x - h[i].x) < WIDTH / 2 ? fabs(x - h[i].x) : WIDTH - fabs(x - h[i].x);
		float minY = fabs(y - h[i].y) < WIDTH / 2 ? fabs(y - h[i].y) : WIDTH - fabs(y - h[i].y);
		dFlockmates[i] = sqrt(pow(minX, 2) + pow(minY, 2));
	}
	if (num != 1) {
		int ind1 = 0;
		int count = 0;
		int ncor = 0, ncoo = 0, ncoa = 0;
		aMin* pin = couzinHunter(dFlockmates, num);
		vector couzinCor, couzinCoo, couzinCoa;
		couzinCor.size = 0;
		couzinCor.angle = 0;
		couzinCoo.size = 0;
		couzinCoo.angle = 0;
		couzinCoa.size = 0;
		couzinCoa.angle = 0;

		while (pin[count].index != -1) {
			if (pin[count].distance < HUNTER_SEPERATE) {
				ncor++;
			}
			else if (pin[count].distance < HUNTER_ALIGN) {
				ncoo++;
			}
			else
				ncoa++;
			count++;
		}

		//排斥层
		if (ncor > 0) {
			vector* cor = new vector[ncor];
			cor[0].angle = 0;
			cor[0].size = 0;
			for (int i = 0; i < count; i++) {
				if (ind1 == ncor)
					break;
				else if (pin[i].distance < HUNTER_SEPERATE) {
					cor[ind1].angle = PI + getAngle(*this, h[pin[i].index]);
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


		//对齐层
		if (ncoo > 0) {
			vector* coo = new vector[ncoo];
			coo[0].size = 0;
			coo[0].angle = 0;
			for (int i = 0; i < count; i++) {
				if (ind1 == ncoo)
					break;
				if (pin[i].distance < HUNTER_ALIGN && pin[i].distance >= HUNTER_SEPERATE) {
					coo[ind1].angle = h[pin[i].index].towards;
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


		//吸引层
		if (ncoa > 0) {
			vector* coa = new vector[ncoa];
			coa[0].size = 0;
			coa[0].angle = 0;
			for (int i = 0; i < count; i++) {
				if (ind1 == ncoa)
					break;
				if (pin[i].distance < HUNTER_COHERE && pin[i].distance >= HUNTER_ALIGN) {
					coa[ind1].angle = getAngle(*this, h[pin[i].index]);
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
			couzinCoa.size = 0;
			couzinCoa.angle = 0;
		}


		FL = vectorPlus(vectorPlus(couzinCoa, couzinCoo), couzinCor);
		delete[] pin;
	}
}


void hunter::step(float speed) {
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


	vector sumVector = vectorPlus(HU, FL);
	if (sumVector.size == 0 || fabs(sumVector.size) < 1e-6) {
		towards += randomAngle;
		amendAngle(towards);
	}
	else {
		sumVector.angle += randomAngle;
		amendAngle(sumVector.angle);

		//dA为总的转角大小
		float dA = sumVector.angle - towards;
		amendAngle(dA);

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