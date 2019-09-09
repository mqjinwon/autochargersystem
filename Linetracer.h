#pragma once
#include "tracemap.h"

//차의 상태를 표현하는 변수
enum {
	GOING_PICK_STUFF = 0,
	GOING_DELIVER_STUFF,
	LIFT_UP,
	LIFT_DOWN,
	WORK_WAIT,
	COMPLETE_CHARGE_WAIT,
	GOING_CHARGE,
	CHARGING,
	CANTCHARGE // 충전하러 가야하지만 충전소가 꽉찬상태
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
	CAR_DIR_U,
	CAR_DIR_R,
	CAR_DIR_D
};

//차 조종하기 위한 방향
enum {
	STOP,
	GO_L, // left 왼쪽으로 가는 것
	GO_F, // forward 앞으로 가는 것
	GO_R, // right 오른쪽으로 가는 것
	GO_B, // backward 뒤쪽으로 가는 것
	L_B, //	
	R_B,
	ROTATE_180 // 180도 회전 시키기
};

class Car {

private:
	int working_state; //현재 하고 있는 일 상태 표현
	int process = 0; //경로까지 진행상황, 초기상태 0
	int car_dir = 0; //차의 방향

	vector<vector<int>> path; //가야하는 전체 경로를 표현(절대경로)
	int realpath[100]; // 차가 실제로 가는 경로(상대경로)

public:
	Car() {
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
			cerr << "wrong size input!!!" << endl;
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
			cerr << "wrong size input!!!" << endl;
		}
		else {
			return realpath[i];
		}
	}

	//경로의 길이 반환
	int pathLength() {
		return path.size() - 1; // 실제 가는 길이는 처음 시작하는 경로를 빼야하기 때문에...
	}

	//남은 경로 길이 반환
	int remainPathLength() {
		return path.size() - process - 1; // 실제 가는 길이는 처음 시작하는 경로를 빼야하기 때문에...
	}

};
