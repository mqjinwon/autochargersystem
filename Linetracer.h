#pragma once
#include "tracemap.h"

static int carID = 0; //���� ���̵� �ο��ϴ� ����

//���� ���¸� ǥ���ϴ� ����
typedef enum {
	GOING_PICK_STUFF = 0,
	GOING_DELIVER_STUFF,
	WORK_WAIT,
	//COMPLETE_CHARGE_WAIT, //�� �ʿ��߾����� �ǹ�(�׳� WORK_WAIT���� ��ü)
	GOING_CHARGE,
	CHARGING,
	CANTCHARGE // �����Ϸ� ���������� �����Ұ� ��������
}CAR_STATUS;

//�� �����ϱ� ���� ����
typedef enum {
	GO_F, // forward ������ ���� ��
	GO_R, // right ���������� ���� ��
	GO_B, // backward �������� ���� ��
	GO_L, // left �������� ���� ��
	L_B, // ���ϸ鼭 �������� ���� ��	
	R_B, // ���ϸ鼭 ���������� ���� ��
	STOP,
	ROTATE_180, // 180�� ȸ�� ��Ű��
	LIFT_UP,
	LIFT_DOWN,
}CAR_CONTROL_DIR;

class Car {

private:
	const int id = carID++; // �� ���̵� �ο��ϱ�
	bool chargeFlag = false; //�����ؾ��ϴ� ������ �ƴ� ���� ǥ���ϴ� ����
	int workingState; //���� �ϰ� �ִ� �� ���� ǥ��
	long process = 0; //��α��� �����Ȳ, �ʱ���� 0
	long prePathLen = 0; // ������ �־��� ����� ����
	pair<int, int> carPos; // ���� ���� �ִ� ��ġ

	vector<vector<int>> path; //�����ϴ� ��ü ��θ� ǥ��(������)
	vector<int> realpath; // ���� ������ ���� ���(�����)

public:
	Car() {
		workingState = WORK_WAIT;
		chargeFlag = false;
	}
	Car(int x, int y) {
		carPos = make_pair(x, y);
	}

	//���� �����ϴ� ��θ� ����
	bool putPath(vector<vector<int>> path) {
		if (path.size() == 0)
			return false;
		else {
			putPrePathLen(pathLength()); // ���ݱ����� path ��θ� ��´�.
			for (int i = 0; i < path.size(); i++) {
				this->path.push_back(path[path.size() - 1 - i]);
			}

			putRealPath();
			return true;
		}
	}

	//��� �� ���ϴ� �κ� ��ȯ
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

	//���� �����ϱ� ���� ���� ��θ� �������
	bool putRealPath();

	//��� �� ���ϴ� �κ� ��ȯ
	int getRealPath(int i) {
		if (i > realpath.size()) {
			cerr << "wrong size input!!!" << endl;
		}
		else {
			return realpath[i];
		}
	}

	//����� ���� ��ȯ
	int pathLength() {
		return path.size();
	}

	//��������� ��ɾ���� ��ȯ
	int realpathLength() {
		return realpath.size(); // ���� ���� ���̴� ó�� �����ϴ� ��θ� �����ϱ� ������...
	}

	//���� ��� ���� ��ȯ
	int remainPathLength() {
		return path.size() - process - 1; // ���� ���� ���̴� ó�� �����ϴ� ��θ� �����ϱ� ������...
	}

	int getCarID() {
		return id;
	}
		
	bool putCarPos(int x, int y) {
		if (x >= 0 && x < MAP_COL && y >=0 && y < MAP_ROW) {
			carPos.first = x;
			carPos.second = y;

			return true;
		}
		else {
			return false;
		}
	}
	pair<int,int> getCarPos() {
		return carPos;
	}

	void putChargeFlag(bool flag) {
		chargeFlag = flag;
	}

	bool getChargeFlag() {
		return chargeFlag;
	}

	void putPrePathLen(int len) {
		prePathLen = len;
	}

	int getPrePathLen() {
		return prePathLen;
	}
};
