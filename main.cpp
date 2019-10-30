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

static float min_rule[4] = { 15.0, 15.0, 30,30 };   //부여받을 수 있는 일의 최대량보다는 커야 함.
													//두 값의 차이도 위 값보다는 커야 함.
													//안그러면 값이 음수로 내려가버릴 때가 생김.
													//rule의 모든 값을 A만큼 올려주면 배터리가 절대로 A이하로 안내려감.
													//static float max_rule[N] = { 60.0, 70.0, 80,90 };
static float max_rule[4] = { 90.0, 90.0, 90,90 };



//실제 일들을 처리할 부분
void processing(vector<Car> carLIst) {

	/////step 1. initilizing part/////
	
	int carNum = carLIst.size(); // 몇대의 차가 있는지 확인하기 위한 변수
	vector<int> batterySize(carNum); // 배터리 값을 받기 위해서 사용하는 녀석

	Map map; // 전체 맵에 대한 것 선언

	//serial port connection
	const char *data;

	time_t start, end; //시간을 알려주는 변수

	while (1) {
		
		if (!serial.Open(PORTNUM, 115200)) {
			printf("connect faliled\n");
		}
		else {
			printf("connect successed\n");
			break;
		}
	}


	
	//main process
	while (1) {

		/////step 2. battery checking/////
		
		//step 2.1 각각의 배터리 상태를 갱신(uart 통신을 한다.)

		//step 2.2 알고리즘에 따라 충전해야할 놈을 충전시킨다(우선순위부터 알려줌 ex) [1,0, 0 ,2])
		//if(충전기가 부족할 경우) -- state =  CANTCHARGE 상태로 변경, charge_flag = true로 변경
		//else -- state =  GOING_CHARGE 상태로 변경, charge_flag = true로 변경

		/////step 3. working checking/////



		/////step 4. route checking/////
		//통신은 #id@를 보내게 된다.

		//모든 차들에게 물어봐야하므로 이렇게 ㄱㄱ
		for (int _id = 0; _id < carNum; _id++) {
			data = "#id@";
			serial.Write(data, 256);

			start = time(NULL);

			while (1) {

				if (time(NULL) - start > 1000) {
					cerr << "timeout!!" << endl;
					break;
				}
			}
		}


	}

	serial.Close();

}

float battery[4] = { 83.7, 76.5, 92.5, 57.7 };



int main(){

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

	

	
	//vector<Car> linetracer;

	//linetracer.push_back(Car(2, 2));
	//linetracer.push_back(Car(3, 4));
	//linetracer.push_back(Car(7, 5));
	//linetracer.push_back(Car(10, 2));

	//processing(linetracer);

	//cout << carID << endl;

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

	full_map = tmp.BFS(7, 2, 10, 2);
	car.putPath(full_map);
	full_map = tmp.BFS(10, 2, 3, 5);
	car.putPath(full_map);

	//full_map = tmp.BFS(3, 5, 6, 3);
	//car.putPath(full_map);

	//full_map = tmp.BFS(6, 3, 1, 1);
	//car.putPath(full_map);

	int map[MAP_ROW+1][MAP_COL+1] = { 0, };
	
	cout << "linetracer path!!!" << endl;

	for (int i = 0; i < car.pathLength(); i++) {
		map[car.getPath(i)[1]][car.getPath(i)[0]] = 1;
	}

	cout << endl;

	for (int i = 0; i < car.realpathLength(); i++) {
		if (i % 10 == 0 && i != 0)
			cout << endl;

		switch (car.getRealPath(i)) {
		case GO_F:
			cout << "GO_F" << "\t";
			break;
		case GO_R:
			cout << "GO_R" << "\t";
			break;

		case GO_L:
			cout << "GO_L" << "\t";
			break;
		case STOP:
			cout << "STOP" << "\t";
			break;
		case ROTATE_180:
			cout << "ROTATE_180" << "\t";
			break;
		case F_STOP:
			cout << "F_STOP" << "\t";
			break;
		case FLAG:
			cout << "FLAG" << "\t";
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
