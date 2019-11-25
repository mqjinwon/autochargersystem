#include "Comm.h"
static CComm serial; //시리얼 통신을 위한 변수 -- 모든 소스에서 사용되므로 전역변수로 선언

#include <iostream>
#include <vector>
#include <queue>
#include <time.h>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "tracemap.h"
#include "Linetracer.h"

using namespace std;

#define PORTNUM "COM15"


///////////////////////////////////////////////////충전알고리즘에서 사용하는 부분
#define BATADD 5
#define BATSUB 0.3

static float min_rule[4] = { 15.0, 15.0, 30,30 };   //부여받을 수 있는 일의 최대량보다는 커야 함.
													//두 값의 차이도 위 값보다는 커야 함.
													//안그러면 값이 음수로 내려가버릴 때가 생김.
													//rule의 모든 값을 A만큼 올려주면 배터리가 절대로 A이하로 안내려감.
													//static float max_rule[N] = { 60.0, 70.0, 80,90 };
static float max_rule[4] = { 90.0, 90.0, 90,90 };

bool crash_flag[4] = { 0,0 };

void putRoot(int caridx) {

	//충돌하려 한다면
	if (false/*충돌하려고한다면*/) {
		//현재 경로 index 저장
		crash_flag[caridx] = 1;
	}
	else if (false/*충돌하려는 상황이 끝났다면*/) {
		//인덱스 복구
		crash_flag[caridx] = 0;
	}
	else {

	}
}
//bool crash_flag[4] = { 0,0 }
//void 충돌방지 및 경로전송(i)
//{
//	if (i가 충돌하려함)
//		현재 경로 index 저장
//		crash_flag[i] = 1;
//	stop의 경로 2개를 전송
//	else if (crash_flag[i] == 1)
//		index 복구
//		crash_flag[i] = 0;
//	경로 2개 전송
//	else
//	{
//		index 유지
//	}
//	경로 2개 전송
//}

//실제 일들을 처리할 부분
void processing() {

	Map map; // 전체 맵에 대한 것 선언

	//라인트레이서 초기위치 및 배터리 잔량 설정
	lineTracer[0].carPos.first = 2;
	lineTracer[0].carPos.second = 4;
	lineTracer[0].bat = 83.7;
	lineTracer[1].carPos.first = 3;
	lineTracer[1].carPos.second = 2;
	lineTracer[1].bat = 76.5;
	lineTracer[2].carPos.first = 7;
	lineTracer[2].carPos.second = 3;
	lineTracer[2].bat = 92.5;
	lineTracer[3].carPos.first = 7;//10;
	lineTracer[3].carPos.second = 2;//5;
	lineTracer[3].bat = 57.7;


	//serial port connection

	const char *data; //데이터 전송시 사용할 변수

	time_t start, end; //시간을 알려주는 변수

	/* 통신부분
	while (1) {	
		if (!serial.Open(PORTNUM, 115200)) {
			printf("connect faliled\n");
		}
		else {
			printf("connect successed\n");
			break;
		}
	}
	*/

	//main process

	//현재 저장하고 있는 일들을 알려주는 부분
	cout << "First allowed work!" << endl;
	for (int i = 0; i < 4; i++) {
		cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
	}
	cout << endl;

	while (1) {

		int lineState = WORK_WAIT;
		int posX, posY = 0;
		vector<vector<int>> route;

		for (int lineIdx = 0; lineIdx < CARNUM; lineIdx++) {

			posX = lineTracer[lineIdx].carPos.first;
			posY = lineTracer[lineIdx].carPos.second;

			//물건들 위치 갱신하는 부분
			if (!(lineTracer[lineIdx].realpath.empty())) {
				if (lineTracer[lineIdx].realpath[lineTracer[lineIdx].relPointer] == LIFT_DOWN) {
					int axis = map.tranRealLocTOStuffLoc(make_pair(posX, posY)); //실제좌표에서 물건 표시하는 숫자로 변환
					map.stuffLoc[axis] = YES;
				}
				else if (lineTracer[lineIdx].realpath[lineTracer[lineIdx].relPointer] == LIFT_UP) {
					int axis = map.tranRealLocTOStuffLoc(make_pair(posX, posY));//실제좌표에서 물건 표시하는 숫자로 변환
					map.stuffLoc[axis] = NO;
				}
			}


			//정보들 print하는 부분
			cout << "line" << lineIdx << "\t" << "absP : " << lineTracer[lineIdx].absPointer << ", absR : " << lineTracer[lineIdx].relPointer <<", posistion : " << posX << ", " << posY << "\t" << "battery : " << lineTracer[lineIdx].bat << endl;

			//충전기 위치에 있다면(충전 중이라면)
			if ((posX == 10 && posY == 2) || (posX == 10 && posY == 5)) {

				//충전알고리즘 실행 후 어떻게 해야할지 나옴	(GOING_WORK or GOING_CHARGE)-------------------재성이 파트
				//linestate = 충전알고리즘()
				lineState = GOING_WORK;//--------------------------------------------------------실험용으로 우선 넣어놓음

				if (lineState == GOING_WORK) {
					route = map.makeroute(posX, posY, lineState); //일할 경로 생성
					lineTracer[lineIdx].putPath(route);//일할 경로 삽입

					//현재 저장하고 있는 일들을 알려주는 부분
					for (int i = 0; i < 4; i++) {
						cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
					}
					cout << endl;

					//경로 확인용--------------------------------------------------------------------실험용으로 우선 넣어놓음
					//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
					//	lineTracer[lineIdx].getPath(i)[1];
					//}
					//충돌방지 알고리즘 삽입---------------------------------------------------------------아직 안함

				}
				else if (lineState == GOING_CHARGE) {
					//empty
				}
				if (lineTracer[lineIdx].bat + BATADD >= 100) {
					lineTracer[lineIdx].bat = 100;
				}
				else {
					lineTracer[lineIdx].bat += BATADD; //배터리 충전
				}
				
			}
			//일하는 중이라면
			else {

				//통신을 지금 하고 받는게 좋을지... 아니면 네대가 다 돌고나서 아래서 정보를 갱신하는게 좋은지... 잘모르겠슴!
				//암튼 받았다고 치고...!
				lineTracer[lineIdx].routeIdx = 1;//------------------------------------------------------실험용으로 우선 넣어놓음!


				//초기에 경로를 받는 상태라면
				if (lineTracer[lineIdx].realpath.size() == 0) {
					//충전알고리즘 실행 후 어떻게 해야할지 나옴	(GOING_WORK or GOING_CHARGE)-------------------재성이 파트
					//linestate = 충전알고리즘()
					lineState = GOING_WORK;//--------------------------------------------------------실험용으로 우선 넣어놓음
					route = map.makeroute(posX, posY, lineState); //일할 경로 생성
					lineTracer[lineIdx].putPath(route);//일할 경로 삽입

					//현재 저장하고 있는 일들을 알려주는 부분
					for (int i = 0; i < 4; i++) {
						cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
					}
					cout << endl;

					//경로 확인용--------------------------------------------------------------------실험용으로 우선 넣어놓음
					//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
					//	lineTracer[lineIdx].getPath(i)[1];
					//}
					//충돌방지 알고리즘 삽입---------------------------------------------------------------아직 안함
				}

				//첫 동작이 하나 끝났다면!
				if (lineTracer[lineIdx].routeIdx == 1) {
					
					//마지막 경로까지 간거라면!
					if (lineTracer[lineIdx].relPointer == (lineTracer[lineIdx].realpath.size() - 1) && lineTracer[lineIdx].realpath.size() > 0) {
						//충전알고리즘 실행 후 어떻게 해야할지 나옴	(GOING_WORK or GOING_CHARGE)-------------------재성이 파트
						//linestate = 충전알고리즘()
						lineState = GOING_WORK;//--------------------------------------------------------실험용으로 우선 넣어놓음
						route = map.makeroute(posX, posY, lineState); //일할 경로 생성
						lineTracer[lineIdx].putPath(route);//일할 경로 삽입

						//현재 저장하고 있는 일들을 알려주는 부분
						for (int i = 0; i < 4; i++) {
							cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
						}
						cout << endl;

						//경로 확인용--------------------------------------------------------------------실험용으로 우선 넣어놓음
						//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
						//	lineTracer[lineIdx].getPath(i)[1];
						//}
						//충돌방지 알고리즘 삽입---------------------------------------------------------------아직 안함
					}

					lineTracer[lineIdx].addAbsPointer(); //절대좌표값을 증가시키기!
					lineTracer[lineIdx].relPointer++; //상대좌표값을 증가시키기!

				}
				else {} //첫 동작이 아직 안끝났다면

				if (lineTracer[lineIdx].bat - BATADD <= 0) {
					lineTracer[lineIdx].bat = 0;
				}
				else {
					lineTracer[lineIdx].bat -= BATSUB; //배터리 소모
				}				
			}
		}
		cout << endl;
		//차 4대에게 통신을 받아 정보를 갱신한다.
		//for (int _id = 0; _id < CARNUM; _id++) {
		//	data = "#id@";
		//	serial.Write(data, 256);

		//	start = time(NULL);

		//	while (1) {

		//		if (time(NULL) - start > 1000) {
		//			cerr << "timeout!!" << endl;
		//			break;
		//		}
		//	}
		//}
/*
	bool crash_flag[4] = {0,0}
	void 충돌방지 및 경로전송(i)
	{
		if(i가 충돌하려함)
			현재 경로 index 저장
			crash_flag[i] = 1;
			stop의 경로 2개를 전송
		else if(crash_flag[i] == 1)
			index 복구
			crash_flag[i] = 0;
			경로 2개 전송
		else
			{index 유지}
			경로 2개 전송
	}


		for(4){
			if(충전중인가?){
				충전알고리즘 실행
				"i가 일하러가야되나, 계속 충전해야되나"가 나옴
				if(일하러가야되면)
					map.makeroute(현재x,y,일) -> 경로 반환, 이걸 저장해둠
					충돌방지 및 경로전송
				else if(계속 충전해야되면)
				{}

				배터리 증가시켜주기;
			}
			else if(일하는중인가?){
				i한테 동작 하나 끝났냐고 물어봐라
				recv
				if (i가 동작 하나 끝냈냐 ? )
				{
					i의 상대, 절대좌표 갱신
					if (i가 일이 아예 끝났나 ? )
					{
						충전알고리즘 실행
						"i가 일해야되나, 충전해야되나"가 나옴.
						map.makeroute(현재x,y,일/충전) -> 경로 반환, 이걸 저장해둠
					}
					else{}						
					충돌방지 및 경로전송
				}
				else
				{}
				배터리 감소시켜주기;
			}
		}

*/


	}

	serial.Close();

}

int main(){

	srand(time(NULL));

	/*srand((unsigned int)time(NULL));
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	Sleep(500);

	cout << "==================================================================" << endl;
	cout << "success connecting to COM17" << endl;
	cout << "==================================================================" << endl << endl;
	Sleep(100);
	cout << "give path to 1 AGV" << endl;
	Sleep(200);
	cout << "give path to 2 AGV" << endl;
	Sleep(200);
	cout << "give path to 3 AGV" << endl;
	Sleep(200);
	cout << "give path to 4 AGV" << endl << endl;
	Sleep(200);
	int count = 0;

	while (1) {

		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

		std::chrono::milliseconds mill = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		cout << "==================================================================" << endl;
		cout << "running time : ";
		if(mill.count() / 60000 >0)
			cout << mill.count() / 60000 << "min ";

		cout << (mill.count() % 60000) / 1000.0 << "sec" << endl;
		cout << "==================================================================" << endl << endl;


		for (int i = 0; i < 4; i++) {
			cout << "checking battery status" << endl;
			cout << "AGV" << i << " : " << battery[i] << endl;;

			Sleep(500);

			if(count >= 1)
				battery[i] -= (rand() % 100) / 1000.0;
		}


		count++;
		cout << endl;


		Sleep(1000);
	}
*/

	

	
	

	vector<vector<int>> full_map;
	Car car;
    Map tmp;

	//for (int k = 0; k < 4; k++) {
	//	for (int i = 0; i < 16; i++) {
	//		cout << tmp.stuffLoc[i] << "  ";
	//	}

	//	cout << endl;

	//	for (int i = 0; i < 4; i++) {
	//		cout << "s : " << tmp.storedWork[i].first << ", e : " << tmp.storedWork[i].second << endl;;
	//	}

	//	cout << endl;

	//	vector<vector<int>> hi = tmp.makeroute(rand()%10, rand()%6, GOING_WORK);

	//	for (int i = 0; i < hi.size(); i++) {
	//		cout << "x : " << hi[i][0] << ". y : " << hi[i][1] << endl;
	//	}
	//}


	
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

	processing();


	full_map = tmp.makeroute(10, 5, GOING_WORK);
	car.putPath(full_map);

	int map[MAP_ROW+1][MAP_COL+1] = { 0, };
	
	cout << "linetracer path!!!" << endl;

	for (int i = 0; i < car.pathLength(); i++) {
		map[car.getPath(i)[1]][car.getPath(i)[0]] = 1;
	}

	cout << endl;

	for (int i = 0; i < car.realpathLength(); i++) {
		if (i % 10 == 0 && i != 0)
			cout << endl;

		switch (car.realpath[i]) {
		case GO_F:
			cout << "GO_F" << "\t";
			break;
		case GO_R:
			cout << "GO_R" << "\t";
			break;
		case GO_L:
			cout << "GO_L" << "\t";
			break;
		case CHARGE_OUT:
			cout << "CHARGE_OUT" << "\t";
			break;
		case STOP:
			cout << "STOP" << "\t";
			break;
		case ROTATE_180:
			cout << "ROTATE_180" << "\t";
			break;
		case LIFT_UP:
			cout << "LIFT_UP" << "\t";
			break;
		case LIFT_DOWN:
			cout << "LIFT_DOWN" << "\t";
			break;
		}
	}

	cout << endl << endl;

	for (int row = 0; row <= MAP_ROW; row++) {
		for (int col = 0; col <= MAP_COL; col++) {
			cout << map[row][col] << "\t";
		}
		cout << endl;
	}
    return 0;
}
