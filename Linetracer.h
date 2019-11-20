#pragma once
#include "tracemap.h"

static int carID = 0; //차에 아이디 부여하는 변수

//차 조종하기 위한 방향
typedef enum {
	GO_F = 0, // forward 앞으로 가는 것
	GO_R, // right 오른쪽으로 가는 것
	GO_B, // backward 뒤쪽으로 가는 것
	GO_L, // left 왼쪽으로 가는 것
	CHARGE_OUT, //충전하고 나갈 때,
	STOP, //잠시 멈추게 하기
	ROTATE_180, // 180도 회전 시키기
	LIFT_UP,
	LIFT_DOWN,
	F_STOP, // 강제로 멈추게 하기
}CAR_CONTROL_DIR;

class Car {

private:
	const int id = carID++; // 차 아이디 부여하기
	//bool chargeFlag = false; //충전해야하는 놈인지 아닌 지를 표현하는 변수
	//<int, int> carDestin; //차가 마지막에 가려고 하는 곳 -- 쓸 일 없음(carpos사용하면 됌)

public:

	pair<int, int> carPos; // 차가 현재 있는 위치

	vector<vector<int>> path; //가야하는 전체 경로를 표현(절대경로)
	vector<int> realpath; // 차가 실제로 가는 경로(상대경로)

	int routeIdx = 0; //route를 가르키는 index (0,1을 사용)
	int route[2] = { F_STOP, F_STOP }; //두칸씩 저장하고 있는 경로

	int absPointer = 0; //경로까지 진행상황, 절대 좌표에서의 포인터
	int relPointer = 0; //경로까지 진행상황, 상대 좌표에서의 포인터

	float bat; // 배터리 잔량 표시 변수
	float remainWorkBat; //현재 하고있는 일의 거리에 해당하는 배터리가 저장
	float remainChrgBat; //충전소까지 가는 거리에 해당하는 배터리가 저장

	Car() {
		//chargeFlag = false;
	}

	Car(int x, int y) {
		carPos = make_pair(x, y);
	}

	//차가 가야하는 절대 경로를 넣음 - 거꾸로 들어온 경로를 뒤집어 넣음으로써 정상적인 순서가 된다.
	bool putPath(vector<vector<int>> path) {
		if (path.size() == 0)
			return false;
		else {

			for (int i = 0; i < path.size(); i++) {
				this->path.push_back(path[path.size() - 1 - i]);
			}

			putRealPath(); //상대경로를 만드는 소스
			return true;
		}
	}

	//경로 중 원하는 부분 반환(absPointer를 인자로 넣는다.)
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

	//차를 조종하기 위한 실제 경로를 만들어줌(차 입장)
	bool putRealPath();

	//절대 경로의 길이 반환(pc입장)
	int pathLength() {
		return path.size();
	}

	//상대 경로의 명령어길이 반환(차입장)
	int realpathLength() {
		return realpath.size(); // 실제 가는 길이는 처음 시작하는 경로를 빼야하기 때문에...
	}

	//남은 상대 경로 길이 반환(차입장)
	int remainPathLength() {
		return realpathLength() - relPointer - 1; // 실제 가는 길이는 처음 시작하는 경로를 빼야하기 때문에...
	}

	//차의 ID 받기
	int getCarID() {
		return id;
	}

	//route index를 바꿔주는 함수
	void putRouteIdx(int i) {
		routeIdx = i;
	}

	//route index를 받아오는 함수
	int getRouteIdx() {
		return routeIdx;
	}

	int* getRoute() {
		return route;
	}

};

//차 네대 생성
static Car robot[4];