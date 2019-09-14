#pragma once
#include "tracemap.h"

static int carID = 0;

//���� ���¸� ǥ���ϴ� ����
typedef enum {
	GOING_PICK_STUFF = 0,
	GOING_DELIVER_STUFF,
	LIFT_UP,
	LIFT_DOWN,
	WORK_WAIT,
	COMPLETE_CHARGE_WAIT,
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
	ROTATE_180 // 180�� ȸ�� ��Ű��
}CAR_CONTROL_DIR;

class Car {

private:
	int id = carID++; // �� ���̵� �ο��ϱ�
	int working_state; //���� �ϰ� �ִ� �� ���� ǥ��
	int process = 0; //��α��� �����Ȳ, �ʱ���� 0
	int car_dir = 0; //���� ����

	vector<vector<int>> path; //�����ϴ� ��ü ��θ� ǥ��(������)
	vector<int> realpath; // ���� ������ ���� ���(�����)

public:
	Car() {
		working_state = WORK_WAIT;
	}

	//���� �����ϴ� ��θ� ����
	bool putPath(vector<vector<int>> path) {
		if (path.size() == 0)
			return false;
		else {
			for (int i = 0; i < path.size(); i++) {
				this->path.push_back(path[path.size() - 1 - i]);
			}
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
		return path.size() - 1; // ���� ���� ���̴� ó�� �����ϴ� ��θ� �����ϱ� ������...
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
		
};
