
/*记录了prey个体的方法*/
/*对于敌对群体个体的交互，采用就近追逃，个体追逃最近邻的敌人*/
/*对于同群体个体的交互，应用Couzin模型，控制个体的信息传递与聚集与不聚集*/

#include"agents.h"
#include"seqAndMin.h"

//构造函数和析构函数不再赘述
prey::prey() {
	alive = 1;
	lifetime = 0;
	ES.angle = 0;
	ES.size = 0;
	FL.angle = 0;
	FL.size = 0;

	dFlockmates = new float[PREY_NUM];
	if (dFlockmates == NULL) {
		cout << "flockmates数组创建失败" << endl;
	}

	dEnemies = new float[HUNTER_NUM];
	if (dEnemies == NULL) {
		cout << "enemies数组创建失败" << endl;
	}
}
prey::~prey() {
	delete[]dFlockmates;
	delete[]dEnemies;
}

//escape是prey对捕食者的逃跑行为，此函数用来计算GCEAngle
void prey::escape(hunter h[], int num) {

	//只有活着的prey才会有计算
	if (alive == 1) {
		//计算该prey与所有hunter的距离
		for (int i = 0; i < num; i++) {
			float minX = fabs(x - h[i].x) < WIDTH / 2 ? fabs(x - h[i].x) : WIDTH - fabs(x - h[i].x);
			float minY = fabs(y - h[i].y) < WIDTH / 2 ? fabs(y - h[i].y) : WIDTH - fabs(y - h[i].y);
			dEnemies[i] = sqrt(pow(minX, 2) + pow(minY, 2));
		}

		//oneMin返回结构体aMin，拥有对应敌人对当前个体距离distance，和敌人的下标信息
		aMin minE = oneMins(dEnemies, num);

		if (minE.distance < PH_SIGHT) {
			//当二者距离小于EARSPACE时，prey认为被hunter吃掉，从此不参与计算
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

//couzin模型的使用
void prey::flock(prey p[], int num) {

	if (alive == 1) {
		//计算同类间的距离
		for (int i = 0; i < num; i++) {
			if (p[i].alive == 1) {
				float minX = fabs(x - p[i].x) < WIDTH / 2 ? fabs(x - p[i].x) : WIDTH - fabs(x - p[i].x);
				float minY = fabs(y - p[i].y) < WIDTH / 2 ? fabs(y - p[i].y) : WIDTH - fabs(y - p[i].y);
				dFlockmates[i] = sqrt(pow(minX, 2) + pow(minY, 2));
			}
			else
				dFlockmates[i] = 0;
		}

		//如果只有一个个体，不存在couzin作用
		if (num != 1) {

			//ind1和ind2是为了避免二重循环设立的指针
			//count用来计算视野范围内的个体数
			//ncor，ncoo，ncoa分别为在排斥层，对齐层，吸引层的同类数目
			int ind1 = 0;
			int count = 0;
			int ncor = 0, ncoo = 0, ncoa = 0;

			//pin是指向aMin数组的指针，couzinAgent提供了所有在该个体视野范围内的同类
			aMin* pin = couzinPrey(dFlockmates, num);

			vector couzinCor, couzinCoo, couzinCoa;
			couzinCor.size = 0;
			couzinCor.angle = 0;
			couzinCoo.size = 0;
			couzinCoo.angle = 0;
			couzinCoa.size = 0;
			couzinCoa.angle = 0;

			//couzinAgent无法确定大小，所以在返回数组的最后添加标志位用以判断
			//该循环用来得到三层同类数，用两个同循环的代价方便建立动态数组
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


			//排斥层
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

			//对齐层
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

			//吸引层
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

//每一步的位置变化和角度变化计算
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

		//dA为总的转角大小


		float dA = sumVector.angle - towards;
		amendAngle(dA);

		//dA与最大转角判断
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