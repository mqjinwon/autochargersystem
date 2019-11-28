#include "Comm.h"
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
#define BATADD 10//3.3
#define BATSUB 1
#define CARNUM 4

static CComm serial; //시리얼 통신을 위한 변수 -- 모든 소스에서 사용되므로 전역변수로 선언
Map map; // 전체 맵에 대한 것 선언
static Car lineTracer[CARNUM]; //차 네대 생성


void putRoot(int caridx) {


	if (lineTracer[caridx].relPointer + 1 == lineTracer[caridx].realpathLength()) {
		lineTracer[caridx].route[0] = lineTracer[caridx].realpath[lineTracer[caridx].relPointer];
		lineTracer[caridx].route[1] = STOP;
		
		cout << "route! : " << lineTracer[caridx].route[0] << ", " << lineTracer[caridx].route[1] << endl;

		return;
	}
	else {

		if (lineTracer[caridx].relPointer + 1 != lineTracer[caridx].realpathLength()) {
			int myNextX = lineTracer[caridx].path[lineTracer[caridx].absPointer+1][0];
			int myNextY = lineTracer[caridx].path[lineTracer[caridx].absPointer+1][1];

			//충돌하려 한다면
			for (int i = 0; i < 4; i++) {
				if (i != caridx) {
					if (lineTracer[i].carPos.first == myNextX && lineTracer[i].carPos.second == myNextY) {

						//정지해랏!!!
						lineTracer[caridx].route[0] = STOP;
						lineTracer[caridx].route[1] = STOP;
						//cout << "==============================================================\n===============================STOP===========================\n==============================================================\n";
						cout << "===============================STOP===========================\n";
						cout << "route! : "; map.printRealpath(lineTracer[caridx].route[0]); map.printRealpath(lineTracer[caridx].route[1]); cout << endl;
						return;
					}
				}
			}
		}

		lineTracer[caridx].route[0] = lineTracer[caridx].realpath[lineTracer[caridx].relPointer];
		lineTracer[caridx].route[1] = lineTracer[caridx].realpath[lineTracer[caridx].relPointer + 1];
	}
	
	lineTracer[caridx].addAbsPointer(); //절대 좌표값을 증가시키기!
	lineTracer[caridx].relPointer++; //상대 좌표값을 증가시키기!

	cout << "route! : "; map.printRealpath(lineTracer[caridx].route[0]); map.printRealpath(lineTracer[caridx].route[1]); cout << endl;

}

int bat_order = 0;
int temp_return = 0;
#define B_minus 20	//일의 최대량 최소량 평균의 70% 수치임.	14
#define C_minus 19	//충전소로 복귀하기까지의 최대수치임.  16
static float min_rule[CARNUM] = { C_minus + B_minus, C_minus + B_minus, C_minus + 2 * B_minus ,C_minus + 2 * B_minus };
static float max_rule[CARNUM] = { 90.0, 90.0, 90,90 };
static int working_robot_num = CARNUM;
int getout = 0;

int charge_decision(int idx) {
	//work_sum = work_sum + 1.0;
	bat_order = 0;
	for (int i = 0; i < CARNUM; i++) {                  //몇번째로 배터리가 작은 녀석인가?
		if (lineTracer[idx].remain_bat < lineTracer[i].remain_bat)
			bat_order++;
	}

	//일이 완전히 끝났다면
	int posX = lineTracer[idx].carPos.first;
	int posY = lineTracer[idx].carPos.second;

	if (lineTracer[idx].realpath.size() == 0) {
		return GOING_WORK;
	}

	//일 하나가 완전히 끝난것.	
	if (lineTracer[idx].relPointer == (lineTracer[idx].realpath.size() - 1)  &&  (!(posX == 10 && posY == 2))  &&  (!(posX == 10 && posY == 5))  )
	{
		if (min_rule[bat_order] < lineTracer[idx].remain_bat)  //배터리가 15 15 30 30보다 위에 있는가?
		{
			//float temp = 다음 일하고싶은놈 골라서 BFS 값.;      //한번 일할만큼의 배터리를 뺀다.
			temp_return = GOING_WORK;    //그렇다면 한번 더 GOING_WORK.
			lineTracer[idx].remain_bat -= map.findShortroute(lineTracer[idx].carPos.first, lineTracer[idx].carPos.second) * BATSUB;
											//lineTracer[idx].work_bat = temp;
			return temp_return;
		}
		else
		{
			if ((map.batLoc[0] == false) && (map.batLoc[1] == false)) {
				getout += 1;
			}

			cout << idx << "th AGV is going to charge======================================================================" << endl;
			temp_return = GOING_CHARGE;   //아니라면 충전소로 가라.
										  //lineTracer[idx].remainChrgBat = C_minus;   //충전소까지의 거리에 해당하는 배터리량
			return temp_return;
		}
	}

	//풀충전이면 일해라
	if (lineTracer[idx].remain_bat >= max_rule[bat_order])
	{
		//float temp = 다음 일하고싶은놈 골라서 BFS 값.;      //한번 일할만큼의 배터리를 뺀다.
		temp_return = GOING_WORK;    //그렇다면 한번 더 GOING_WORK.
		lineTracer[idx].remain_bat -= map.findShortroute(lineTracer[idx].carPos.first, lineTracer[idx].carPos.second) * BATSUB;
		return temp_return;                              //lineTracer[idx].work_bat = temp;
	}

	int tmpX, tmpY, tmpBat = 0, highestBat = 0, highestindex = 0;

	for (int i = 0; i < 4; i++) {
			tmpX = lineTracer[i].carPos.first;
			tmpY = lineTracer[i].carPos.second;

			if ((tmpX == 10 && tmpY == 2) || (tmpX == 10 && tmpY == 5)) {
				tmpBat = lineTracer[i].remain_bat;
				if (tmpBat > highestBat) {
					highestBat = tmpBat;
					highestindex = i;
				}
			}
	}

	//일하는 수가 부족하면 일해라(배터리 적어도 강제로 일 시킴)
	if ((getout > 0) && (idx==highestindex))
	{
		getout -= 1;
		temp_return = GOING_WORK;
		return temp_return;
	}
	return GOING_CHARGE;
}


//실제 일들을 처리할 부분
void processing() {



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
	lineTracer[3].carPos.first = 10;
	lineTracer[3].carPos.second = 5;
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
	cout << endl << "===================================================================================" << endl << endl;

	while (1) {

		int lineState = WORK_WAIT;
		int posX, posY = 0;
		vector<vector<int>> tmproute;

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
			cout << "line" << lineIdx << "\t" << "absP : " << lineTracer[lineIdx].absPointer << ", absR : " << lineTracer[lineIdx].relPointer <<", posistion : " << posX << ", " << posY << "\t" << "B : " << lineTracer[lineIdx].bat << ", RB : " << lineTracer[lineIdx].remain_bat << endl;

			//충전기 위치에 있다면(충전 중이라면)
			if ((posX == 10 && posY == 2) || (posX == 10 && posY == 5)) {

				//충전알고리즘 실행 후 어떻게 해야할지 나옴	(GOING_WORK or GOING_CHARGE)-------------------재성이 파트
				lineState = charge_decision(lineIdx);
				//lineState = GOING_WORK;//--------------------------------------------------------실험용으로 우선 넣어놓음


				//if (lineTracer[lineIdx].relPointer > 0) {
				//	putRoot(lineIdx);
				//}
				if (lineState == GOING_WORK) {
					if((posX == 10 && posY == 2))
						map.batLoc[0] = true;
					else if((posX == 10 && posY == 5))
						map.batLoc[1] = true;

					tmproute = map.makeroute(posX, posY, lineState); //일할 경로 생성
					lineTracer[lineIdx].putPath(tmproute);//일할 경로 삽입

					//현재 저장하고 있는 일들을 알려주는 부분
					for (int i = 0; i < 4; i++) {
						cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
					}
					cout << endl;

					//경로 확인용--------------------------------------------------------------------실험용으로 우선 넣어놓음
					//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
					//	lineTracer[lineIdx].getPath(i)[1];
					//}
					//충돌방지 알고리즘 삽입
					putRoot(lineIdx);
				}

				if (lineTracer[lineIdx].bat + BATADD >= 100) {
					lineTracer[lineIdx].bat = 100;
				}
				else {
					lineTracer[lineIdx].bat += BATADD; //배터리 충전
					lineTracer[lineIdx].remain_bat = lineTracer[lineIdx].bat;
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
					lineState = charge_decision(lineIdx);
					//lineState = GOING_WORK;//--------------------------------------------------------실험용으로 우선 넣어놓음
					tmproute = map.makeroute(posX, posY, lineState); //일할 경로 생성
					lineTracer[lineIdx].putPath(tmproute);//일할 경로 삽입

					//현재 저장하고 있는 일들을 알려주는 부분
					for (int i = 0; i < 4; i++) {
						cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
					}
					cout << endl;

					//경로 확인용--------------------------------------------------------------------실험용으로 우선 넣어놓음
					//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
					//	lineTracer[lineIdx].getPath(i)[1];
					//}
					//충돌방지 알고리즘 삽입
					putRoot(lineIdx);
				}
				//첫 동작이 하나 끝났다면!
				else if (lineTracer[lineIdx].routeIdx == 1) {
					
					//마지막 경로까지 간거라면!
					if (lineTracer[lineIdx].relPointer == (lineTracer[lineIdx].realpath.size()-1)) {
						//충전알고리즘 실행 후 어떻게 해야할지 나옴	(GOING_WORK or GOING_CHARGE)-------------------재성이 파트
						lineState = charge_decision(lineIdx);

						if (getout > 0) {
							continue;
						}

						//lineState = GOING_WORK;//--------------------------------------------------------실험용으로 우선 넣어놓음
						tmproute = map.makeroute(posX, posY, lineState); //일할 경로 생성
						lineTracer[lineIdx].putPath(tmproute);//일할 경로 삽입

						//현재 저장하고 있는 일들을 알려주는 부분
						for (int i = 0; i < 4; i++) {
							cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
						}
						cout << endl;

						//경로 확인용--------------------------------------------------------------------실험용으로 우선 넣어놓음
						//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
						//	lineTracer[lineIdx].getPath(i)[1];
						//}
						
					}

					//충돌방지 알고리즘 삽입
					putRoot(lineIdx);

				}
				else {} //첫 동작이 아직 안끝났다면

				if (lineTracer[lineIdx].bat - BATADD <= 0) {
					lineTracer[lineIdx].bat = 0;
				}
				else {
					lineTracer[lineIdx].bat -= BATSUB; //배터리 소모
					lineTracer[lineIdx].work_bat -= BATSUB;
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
