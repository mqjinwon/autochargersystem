#pragma once
#include "tracemap.h"

static int carID = 0; //���� ���̵� �ο��ϴ� ����

//�� �����ϱ� ���� ����
typedef enum {
	GO_F = 0, // forward ������ ���� ��
	GO_R, // right ���������� ���� ��
	GO_B, // backward �������� ���� ��
	GO_L, // left �������� ���� ��
	CHARGE_OUT, //�����ϰ� ���� ��,
	ROTATE_180, // 180�� ȸ���ϰ� ������ �ͱ���
	LIFT_UP,
	LIFT_DOWN,
	STOP // ����
}CAR_CONTROL_DIR;

class Car {

private:
	const int id = carID++; // �� ���̵� �ο��ϱ�
	//bool chargeFlag = false; //�����ؾ��ϴ� ������ �ƴ� ���� ǥ���ϴ� ����
	//<int, int> carDestin; //���� �������� ������ �ϴ� �� -- �� �� ����(carpos����ϸ� ��)

public:

	pair<int, int> carPos; // ���� ���� �ִ� ��ġ

	vector<vector<int>> path; //�����ϴ� ��ü ��θ� ǥ��(������)
	vector<int> realpath; // ���� ������ ���� ���(�����)

	int routeIdx = 1; //route�� ����Ű�� index (0,1�� ���) -- �ʱ⿡ 1�̾�� ���� ���� �� ����
	int route[2] = { STOP, STOP }; //��ĭ�� �����ϰ� �ִ� ���

	int absPointer = 0; //��α��� �����Ȳ, ���� ��ǥ������ ������
	int relPointer = 0; //��α��� �����Ȳ, ��� ��ǥ������ ������

	float bat; // ���͸� �ܷ� ǥ�� ����
	float remainWorkBat; //���� �ϰ��ִ� ���� �Ÿ��� �ش��ϴ� ���͸��� ����
	float remainChrgBat; //�����ұ��� ���� �Ÿ��� �ش��ϴ� ���͸��� ����

	Car() {
		//chargeFlag = false;
	}

	Car(int x, int y) {
		carPos = make_pair(x, y);
	}

	//relPointer���� ��Ű�� ���� ����ؾ��Ѵ�! -- �׻� �����ϴ� ���� �ƴϱ⿡ �Լ��� ������Ű���� ����
	void addAbsPointer() {

		int carDir = realpath[relPointer];

		if ((carDir == LIFT_DOWN) || (carDir == STOP)) {}
		else {
			absPointer++;
			carPos.first = path[absPointer][0];
			carPos.second =path[absPointer][1];
		}
	}

	//���� �����ϴ� ���� ��θ� ���� - �Ųٷ� ���� ��θ� ������ �������ν� �������� ������ �ȴ�.
	bool putPath(vector<vector<int>> path) {

		//������ ����Ǿ��� ��θ� �� �ʱ�ȭ�����ش�.
		this->path.clear();
		this->realpath.clear();
		this->absPointer = 0;
		this->relPointer = 0;

		if (path.size() == 0)
			return false;
		else {

			for (int i = 0; i < path.size(); i++) {
				this->path.push_back(path[path.size() - 1 - i]);
			}

			putRealPath(); //����θ� ����� �ҽ�
			return true;
		}
	}

	//��� �� ���ϴ� �κ� ��ȯ(absPointer�� ���ڷ� �ִ´�.)
	vector<int> getPath(int i) {
		if (i > pathLength()) {
			cerr << "wrong size input!!!" << endl;
		}
		else {
			//���߿� �������� �κ�!!!-----------------------------------------------------------------------------
			cout << "x : " << path[i][0] << ", y : " << path[i][1] << ", depth : "
				<< path[i][2] << ", id : " << path[i][3] << ", parent : " << path[i][4] << endl;
			return path[i];
		}

	}

	//���� �����ϱ� ���� ���� ��θ� �������(�� ����)
	bool putRealPath();

	//���� ����� ���� ��ȯ(pc����)
	int pathLength() {
		return path.size();
	}

	//��� ����� ��ɾ���� ��ȯ(������)
	int realpathLength() {
		return realpath.size(); // ���� ���� ���̴� ó�� �����ϴ� ��θ� �����ϱ� ������...
	}

	//���� ��� ��� ���� ��ȯ(������)
	int remainPathLength() {
		return realpathLength() - relPointer - 1; // ���� ���� ���̴� ó�� �����ϴ� ��θ� �����ϱ� ������...
	}

	//���� ID �ޱ�
	int getCarID() {
		return id;
	}

	//route index�� �ٲ��ִ� �Լ�
	void putRouteIdx(int i) {
		routeIdx = i;
	}

	//route index�� �޾ƿ��� �Լ�
	int getRouteIdx() {
		return routeIdx;
	}

	//route ������ �޾ƿ´�.
	int* getRoute() {
		return route;
	}

};


