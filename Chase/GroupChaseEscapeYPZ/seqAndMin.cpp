#include "seqAndMin.h"

//��һ���֣�sequance����
//�����������˳��
void randomSeq(int seq[], int total) {
	int* output = new int[total];
	int end;
	int num;
	// initializing
	for (int i = 0; i < total; i++) {
		seq[i] = i;
	}
	end = total;

	// randomizing
	for (int i = 0; i < total; i++) {
		num = rand() % end;
		output[i] = seq[num];
		seq[num] = seq[end - 1];
		end--;
	}
	// copy
	for (int i = 0; i < total; i++) {
		seq[i] = output[i];
	}
	delete[] output;
}

//�ڶ����֣�ѡȡ��Сֵ�Լ���λ��
aMin oneMins(float* arr, int len) {

	aMin minAgent{ 0, 0 };
	float* pMin = arr;
	for (int i = 0; i < len; i++) {
		if (arr[i] != 0) {
			*pMin = arr[i];
			break;
		}
	}
	//	float* pMin = arr;
		//ȡ������Ⱥ����Ŀ���ֵ�����㶯̬����
	int a;
	if (HUNTER_NUM >= PREY_NUM)
		a = HUNTER_NUM;
	else
		a = PREY_NUM;

	//��¼�±꣬����ȷ����Ӧ����
	int* ind = new int[a];
	int j = 0;
	int count = 0;
	for (int i = 0; i < len; i++) {
		if (arr[i] != 0) {
			if (*pMin == arr[i] || (fabs(*pMin - arr[i]) <= 1e-6)) {
				count++;
				ind[j] = i;
				j++;
			}
			else if (*pMin > arr[i]) {
				*pMin = arr[i];
				count = 1;
				while (j != 0) {
					j--;
					ind[j] = 0;
				}
				ind[j] = i;
				j++;
			}
		}

	}
	int rnd = (rand() % count) + 1;

	minAgent.distance = *pMin;
	minAgent.index = ind[rnd - 1];

	delete[]ind;
	return minAgent;
}

//�������֣����couzin�뾶�ڵĸ���
aMin* couzinPrey(float* arr, int len) {
	int count = 0;
	int ind = 0;

	//Ԥѭ�������Ұ��Χ�ڸ�����
	for (int i = 0; i < len; i++) {
		if (arr[i] < PREY_COHERE && (arr[i] != 0 && fabs(arr[i]) > 1e-6)) {
			count++;
		}
	}
	aMin* pAgent = new aMin[count + 1];
	for (int i = 0; i < len; i++) {
		if (arr[i] < PREY_COHERE && (arr[i] != 0 && fabs(arr[i]) > 1e-6)) {
			pAgent[ind].distance = arr[i];
			pAgent[ind].index = i;
			ind++;
		}
	}
	//��־λ�������ж�
	pAgent[count].distance = 0;
	pAgent[count].index = -1;

	return pAgent;

}

aMin* couzinHunter(float* arr, int len) {
	int count = 0;
	int ind = 0;

	//Ԥѭ�������Ұ��Χ�ڸ�����
	for (int i = 0; i < len; i++) {
		if (arr[i] < HUNTER_COHERE && (arr[i] != 0 && fabs(arr[i]) > 1e-6)) {
			count++;
		}
	}
	aMin* pAgent = new aMin[count + 1];
	for (int i = 0; i < len; i++) {
		if (arr[i] < HUNTER_COHERE && (arr[i] != 0 && fabs(arr[i]) > 1e-6)) {
			pAgent[ind].distance = arr[i];
			pAgent[ind].index = i;
			ind++;
		}
	}
	//��־λ�������ж�
	pAgent[count].distance = 0;
	pAgent[count].index = -1;

	return pAgent;

}


//���Ĳ��֣���������н�
float getAngle(agent A, agent B) {
	float angle;
	float xdistance = B.x - A.x, ydistance = B.y - A.y;
	if (fabs(xdistance) < WIDTH / 2 && fabs(ydistance) < WIDTH / 2) {
		angle = atan2(ydistance, xdistance);
	}
	else if (fabs(xdistance) > WIDTH / 2 && fabs(ydistance) < WIDTH / 2) {
		if (xdistance > 0)
			angle = atan2(ydistance, xdistance - WIDTH);
		else
			angle = atan2(ydistance, WIDTH + xdistance);
	}
	else if (fabs(xdistance) < WIDTH / 2 && fabs(ydistance) > WIDTH / 2) {
		if (ydistance > 0)
			angle = atan2(ydistance - WIDTH, xdistance);
		else
			angle = atan2(WIDTH + ydistance, xdistance);
	}
	else {
		if (ydistance > 0) {
			if (xdistance > 0)
				angle = atan2(ydistance - WIDTH, xdistance - WIDTH);
			else
				angle = atan2(ydistance - WIDTH, WIDTH + xdistance);
		}
		else {
			if (xdistance > 0)
				angle = atan2(WIDTH + ydistance, xdistance - WIDTH);
			else
				angle = atan2(WIDTH + ydistance, WIDTH + xdistance);
		}
	}
	return angle;
}

//���岿�֣����Ƕȷŵ�[-PI,PI]��
void amendAngle(float& angle) {
	if (angle > PI) {
		angle -= 2 * PI;
	}
	else if (angle < -PI) {
		angle += 2 * PI;
	}
}

//�������֣�����ʸ����    
vector vectorPlus(vector A, vector B) {
	vector C;
	C.angle = 0;
	C.size = 0;
	float theAngle;
	float judgeAngle = A.angle - B.angle;
	amendAngle(judgeAngle);

	if (A.size == 0 || A.size <= 1e-6) {
		if (B.size == 0 || B.size <= 1e-6) {
			C.size = 0;
			C.angle = 0;
		}
		else {
			C.size = B.size;
			C.angle = B.angle;
		}
	}
	else
	{
		if (B.size == 0 || B.size <= 1e-6) {
			C.size = A.size;
			C.angle = A.angle;
		}
		else {
			float xc = A.size * cos(A.angle) + B.size * cos(B.angle);
			float yc = A.size * sin(A.angle) + B.size * sin(B.angle);
			C.size = sqrt(pow(xc, 2) + pow(yc, 2));
			C.angle = atan2f(yc, xc);
		}

	}
	return C;
};