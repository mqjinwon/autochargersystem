#include <iostream>
#include <vector>
#include <queue>
#include "tracemap.h"
#include "SerialComm.h"

using namespace std;


static CSerialComm serial; //�ø��� ����� ���� ���� -- ��� �ҽ����� ���ǹǷ� ���������� ����

//���� ���¸� ǥ���ϴ� ����
enum {
	GOING_PICK_STUFF = 0,
	GOING_LEAVE_STUFF,
	LIFT_UP,
	LIFT_DOWN,
	WORK_WAIT,
	COMPLETE_CHARGE_WAIT,
	GOING_CHARGE,
	CHARGING
};

/*
���� �������� ���� ���
- [2~5][3, 7] / [2, 10], [5, 10]
������ �������� ���� ���
- [2~5][2, 6]
*/
//������ ���¿��� ���� ����
enum {
	CAR_DIR_L = 0,
	CAR_DIR_R
};

//�� �����ϱ� ���� ����
enum {
	GO_L,
	GO_U,
	GO_R,
	GO_D,
	STOP
};


class Car{

private:
	int working_state; //���� �ϰ� �ִ� �� ���� ǥ��
	int process = 0; //��α��� �����Ȳ, �ʱ���� 0
	int car_dir = 0; //���� ����

	vector<vector<int>> path; //�����ϴ� ��ü ��θ� ǥ��(������)
	int realpath[100]; // ���� ������ ���� ���(�����)


public:
    Car(){
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
			cout << "size overflow" << endl;
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
		if (i > pathLength()) {
			cout << "size overflow" << endl;
		}
		else {
			return realpath[i];
		}
	}

	//����� ���� ��ȯ
	int pathLength(){
		return path.size() - 1; // ���� ���� ���̴� ó�� �����ϴ� ��θ� �����ϱ� ������...
	}

	//���� ��� ���� ��ȯ
	int remainPathLength() {
		return path.size() - process - 1; // ���� ���� ���̴� ó�� �����ϴ� ��θ� �����ϱ� ������...
	}

};

int main(){

	vector<vector<int>> full_map;
	Car car;
    Map tmp;


    for(int row =0; row<=MAP_ROW; row++){
        for(int col=0; col<=MAP_COL; col++){
			switch (tmp.getPos(col, row)) {
			case NODIR:
				cout << "NODIR" << "\t";
				break;
			case L:
				cout << "L" << "\t";
				break;
			case U:
				cout << "U" << "\t";
				break;
			case R:
				cout << "R" << "\t";
				break;
			case D:
				cout << "D" << "\t";
				break;
			case LU:
				cout << "LU" << "\t";
				break;
			case UR:
				cout << "UR" << "\t";
				break;
			case RD:
				cout << "RD" << "\t";
				break;
			case DL:
				cout << "DL" << "\t";
				break;
			case LUR:
				cout << "LUR" << "\t";
				break;
			case URD:
				cout << "URD" << "\t";
				break;
			case RDL:
				cout << "RDL" << "\t";
				break;
			case LURD:
				cout << "NODIR" << "\t";
				break;
			}

            
        }
        cout << endl;
    }

	full_map = tmp.BFS(10, 2, 5, 3);
	car.putPath(full_map);

	int map[MAP_ROW+1][MAP_COL+1] = { 0, };

	for (int i = 0; i < full_map.size(); i++) {
		cout << "x : " << full_map[i][0] << ", y : " << full_map[i][1] << ", depth : "
			<< full_map[i][2] << ", id : " << full_map[i][3] << ", parent : " << full_map[i][4] << endl;
		map[full_map[i][1]][full_map[i][0]] = 1;
	}

	for (int i = 0; i < full_map.size(); i++) {
		car.getPath(i);
	}

	cout << endl;

	for (int row = 0; row <= MAP_ROW; row++) {
		for (int col = 0; col <= MAP_COL; col++) {
			cout << map[row][col] << "\t";
		}
		cout << endl;
	}

    return 0;
}
