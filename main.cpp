#include <iostream>
#include <vector>
#include <queue>
#include "tracemap.h"
#include "SerialComm.h"

using namespace std;


static CSerialComm serial; //시리얼 통신을 위한 변수 -- 모든 소스에서 사용되므로 전역변수로 선언

//차의 상태를 표현하는 변수
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
왼쪽 방향으로 있을 경우
- [2~5][3, 7] / [2, 10], [5, 10]
오른쪽 방향으로 있을 경우
- [2~5][2, 6]
*/
//정지한 상태에서 차의 방향
enum {
	CAR_DIR_L = 0,
	CAR_DIR_R
};

//차 조종하기 위한 방향
enum {
	GO_L,
	GO_U,
	GO_R,
	GO_D,
	STOP
};


class Car{

private:
	int working_state; //현재 하고 있는 일 상태 표현
	int process = 0; //경로까지 진행상황, 초기상태 0
	int car_dir = 0; //차의 방향

	vector<vector<int>> path; //가야하는 전체 경로를 표현(절대경로)
	int realpath[100]; // 차가 실제로 가는 경로(상대경로)


public:
    Car(){
		working_state = WORK_WAIT;
    }

	//차가 가야하는 경로를 넣음
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

	//경로 중 원하는 부분 반환
	vector<int> getPath(int i) {
		if (i > pathLength()) {
			cout << "size overflow" << endl;
		}
		else {
			//나중에 지워야할 부분!!!-----------------------------------------------------------------------------
			cout << "x : " << path[i][0] << ", y : " << path[i][1] << ", depth : "
				<< path[i][2] << ", id : " << path[i][3] << ", parent : " << path[i][4] << endl;
			return path[i];
		}
	}

	//차를 조종하기 위한 실제 경로를 만들어줌
	bool putRealPath();

	//경로 중 원하는 부분 반환
	int getRealPath(int i) {
		if (i > pathLength()) {
			cout << "size overflow" << endl;
		}
		else {
			return realpath[i];
		}
	}

	//경로의 길이 반환
	int pathLength(){
		return path.size() - 1; // 실제 가는 길이는 처음 시작하는 경로를 빼야하기 때문에...
	}

	//남은 경로 길이 반환
	int remainPathLength() {
		return path.size() - process - 1; // 실제 가는 길이는 처음 시작하는 경로를 빼야하기 때문에...
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
