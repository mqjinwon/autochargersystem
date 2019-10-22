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
	CHARGE_OUT, //충전하고 나갈 때,
	STOP,
	ROTATE_180, // 180도 회전 시키기
	LIFT_UP,
	LIFT_DOWN,
}CAR_CONTROL_DIR;

class Car {

private:
	const int id = carID++; // 차 아이디 부여하기

	float bat; // 배터리 잔량 표시 변수
	float remain_work_bat;   //현재 하고있는 일의 거리에 해당하는 배터리가 저장
	float remain_chrg_bat;   //충전소까지 가는 거리에 해당하는 배터리가 저장

	//bool chargeFlag = false; //충전해야하는 놈인지 아닌 지를 표현하는 변수
	int workingState; //현재 상태 표현, CAR_STATUS
	long long int process = 0; //경로까지 진행상황, 초기상태 0 - realpath가 움직일 때 마다 증가 시킨다.
	long long int prePathLen = 0; // 가지고 있었던 절대 경로의 길이
	long long int preRealPathLen = 0; //가지고 있었던 실제 경로의 길이
	pair<int, int> carPos; // 차가 현재 있는 위치
	pair<int, int> carDestin; //차가 마지막에 가려고 하는 곳

	vector<vector<int>> path; //가야하는 전체 경로를 표현(절대경로)
	vector<int> realpath; // 차가 실제로 가는 경로(상대경로)

public:
	Car() {
		workingState = WORK_WAIT;
		//chargeFlag = false;
	}
	Car(int x, int y) {
		carPos = make_pair(x, y);
	}

	//차가 가야하는 경로를 넣음
	bool putPath(vector<vector<int>> path) {
		if (path.size() == 0)
			return false;
		else {
			putPrePathLen(pathLength()); // 지금까지의 절대 경로를 담는다.
			putPreRealPathLen(realpathLength()); // 지금까지의 절대 경로를 담는다.

			for (int i = 0; i < path.size(); i++) {
				this->path.push_back(path[path.size() - 1 - i]);
			}


			putRealPath();
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
		return path.size();
	}

	//실제경로의 명령어길이 반환
	int realpathLength() {
		return realpath.size(); // 실제 가는 길이는 처음 시작하는 경로를 빼야하기 때문에...
	}

	//남은 실제 경로 길이 반환
	int remainPathLength() {
		return realpathLength() - process - 1; // 실제 가는 길이는 처음 시작하는 경로를 빼야하기 때문에...
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

	bool putLastCarPos(int x, int y) {
		if (x >= 0 && x < MAP_COL && y >= 0 && y < MAP_ROW) {
			carPos.first = x;
			carPos.second = y;

			return true;
		}
		else {
			return false;
		}
	}
	pair<int, int> getLastCarPos() {
		return carPos;
	}

	//void putChargeFlag(bool flag) {
	//	chargeFlag = flag;
	//}

	//bool getChargeFlag() {
	//	return chargeFlag;
	//}

	void putPrePathLen(int len) {
		prePathLen = len;
	}

	int getPrePathLen() {
		return prePathLen;
	}

	void putPreRealPathLen(int len) {
		prePathLen = len;
	}

	int getPreRealPathLen() {
		return prePathLen;
	}
};

static Car robot[4];