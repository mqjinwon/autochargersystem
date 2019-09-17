#pragma once
#include "tracemap.h"

static int carID = 0; //차에 아이디 부여하는 변수

//차의 상태를 표현하는 변수
typedef enum {
	GOING_PICK_STUFF = 0,
	GOING_DELIVER_STUFF,
	WORK_WAIT,
	//COMPLETE_CHARGE_WAIT, //왜 필요했었는지 의문(그냥 WORK_WAIT으로 대체)
	GOING_CHARGE,
	CHARGING,
	CANTCHARGE // 충전하러 가야하지만 충전소가 꽉찬상태
}CAR_STATUS;

//차 조종하기 위한 방향
typedef enum {
	GO_F, // forward 앞으로 가는 것
	GO_R, // right 오른쪽으로 가는 것
	GO_B, // backward 뒤쪽으로 가는 것
	GO_L, // left 왼쪽으로 가는 것
	L_B, // 빽하면서 왼쪽으로 가는 것	
	R_B, // 뺵하면서 오른쪽으로 가는 것
	STOP,
	ROTATE_180, // 180도 회전 시키기
	LIFT_UP,
	LIFT_DOWN,
}CAR_CONTROL_DIR;

class Car {

private:
	const int id = carID++; // 차 아이디 부여하기
	bool chargeFlag = false; //충전해야하는 놈인지 아닌 지를 표현하는 변수
	int working_state; //현재 하고 있는 일 상태 표현
	int process = 0; //경로까지 진행상황, 초기상태 0
	int car_dir = 0; //차의 방향
	int car_pos[2] = { 0, }; // 차가 현재 있는 위치

	vector<vector<int>> path; //가야하는 전체 경로를 표현(절대경로)
	vector<int> realpath; // 차가 실제로 가는 경로(상대경로)

public:
	Car() {
		working_state = WORK_WAIT;
		chargeFlag = false;
	}
	Car(int x, int y) {
		car_pos[0] = x;
		car_pos[1] = y;
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
		if (i > realpath.size()) {
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

	//실제경로의 명령어길이 반환
	int realpathLength() {
		return realpath.size(); // 실제 가는 길이는 처음 시작하는 경로를 빼야하기 때문에...
	}

	//남은 경로 길이 반환
	int remainPathLength() {
		return path.size() - process - 1; // 실제 가는 길이는 처음 시작하는 경로를 빼야하기 때문에...
	}

	int getCarID() {
		return id;
	}
		
	bool putCarPos(int x, int y) {
		if (x >= 0 && x < MAP_COL && y >=0 && y < MAP_ROW) {
			car_pos[0] = x;
			car_pos[1] = y;

			return true;
		}
		else {
			return false;
		}
	}
	int* getCarPos() {
		return car_pos;
	}

	void putChargeFlag(bool flag) {
		chargeFlag = flag;
	}

	bool getChargeFlag() {
		return chargeFlag;
	}
};
