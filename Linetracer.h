#pragma once
#include "tracemap.h"

static int carID = 0; //���� ���̵� �ο��ϴ� ����

//���� ���¸� ǥ���ϴ� ����
typedef enum {
	GOING_PICK_STUFF = 0,
	GOING_DELIVER_STUFF,
	WORK_WAIT,
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
	CHARGE_OUT, //�����ϰ� ���� ��,
	STOP, //��� ���߰� �ϱ�
	ROTATE_180, // 180�� ȸ�� ��Ű��
	LIFT_UP,
	LIFT_DOWN,
	F_STOP, // ������ ���߰� �ϱ�
	FLAG //�� ����� ���� �� CAR_STATUS�� ���� �� �ְ� �����.(������ ������ ���� �� �ߵ�)
}CAR_CONTROL_DIR;

class Car {

private:
	const int id = carID++; // �� ���̵� �ο��ϱ�
	//bool chargeFlag = false; //�����ؾ��ϴ� ������ �ƴ� ���� ǥ���ϴ� ����
	long long int prePathLen = 0; // ������ �־��� ���� ����� ����
	long long int preRealPathLen = 0; //������ �־��� ���� ����� ����
	pair<int, int> carPos; // ���� ���� �ִ� ��ġ
	pair<int, int> carDestin; //���� �������� ������ �ϴ� ��
	vector<vector<int>> path; //�����ϴ� ��ü ��θ� ǥ��(������)
	vector<int> realpath; // ���� ������ ���� ���(�����)

	int routeIdx = 0; //route�� ����Ű�� index (0,1�� ���)
	int route[2] = { F_STOP, F_STOP }; //��ĭ�� �����ϰ� �ִ� ���

public:
	int workingState; //���� ���� ǥ��, CAR_STATUS
	float bat; // ���͸� �ܷ� ǥ�� ����
	long long int process = 0; //��α��� �����Ȳ, �ʱ���� 0 - realpath�� ������ �� ���� ���� ��Ų��.
	float remain_work_bat; //���� �ϰ��ִ� ���� �Ÿ��� �ش��ϴ� ���͸��� ����
	float remain_chrg_bat; //�����ұ��� ���� �Ÿ��� �ش��ϴ� ���͸��� ����

	Car() {
		workingState = WORK_WAIT;
		//chargeFlag = false;
	}

	Car(int x, int y) {
		carPos = make_pair(x, y);
	}

	//���� �����ϴ� ��θ� ����
	bool putPath(vector<vector<int>> path) {
		if (path.size() == 0)
			return false;
		else {
			putPrePathLen(pathLength()); // ���ݱ����� ���� ��θ� ��´�.
			putPreRealPathLen(realpathLength()); // ���ݱ����� ��� ��θ� ��´�.

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

	//���� �����ϱ� ���� ���� ��θ� �������(������)
	bool putRealPath();

	//��� �� ���ϴ� �κ� ��ȯ(������)
	int getRealPath(int i) {
		if (i > realpath.size()) {
			cerr << "wrong size input!!!" << endl;
		}
		else {
			return realpath[i];
		}
	}

	//����� ���� ��ȯ(pc����)
	int pathLength() {
		return path.size();
	}

	//��������� ��ɾ���� ��ȯ(������)
	int realpathLength() {
		return realpath.size(); // ���� ���� ���̴� ó�� �����ϴ� ��θ� �����ϱ� ������...
	}

	//���� ���� ��� ���� ��ȯ(������)
	int remainPathLength() {
		return realpathLength() - process - 1; // ���� ���� ���̴� ó�� �����ϴ� ��θ� �����ϱ� ������...
	}

	//���� ID �ޱ�
	int getCarID() {
		return id;
	}
	
	//���� ���� ��ġ ����ֱ�
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
		
	//���� ���� ��ġ �޾Ƴ���
	pair<int,int> getCarPos() {
		return carPos;
	}

	//������ ��� �����Է�(pc����)
	void putPrePathLen(int len) {
		prePathLen = len;
	}

	//������ ��� ����(pc����)
	int getPrePathLen() {
		return prePathLen;
	}

	//������ ������� �����Է�(������)
	void putPreRealPathLen(int len) {
		preRealPathLen = len;
	}

	//������ ������� ����(������)
	int getPreRealPathLen() {
		return prePathLen;
	}

	//route index�� �ٲ��ִ� �Լ�
	void putRouteIdx(int i) {
		routeIdx = i;
	}

	//route index�� �޾ƿ��� �Լ�
	int getRouteIdx() {
		return routeIdx;
	}

	//route�� ���� �־��ִ� �Լ�
	bool putRoute();

	int* getRoute() {
		return route;
	}

};

//�� �״� ����
static Car robot[4];