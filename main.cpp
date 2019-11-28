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

///////////////////////////////////////////////////�����˰��򿡼� ����ϴ� �κ�
#define BATADD 10//3.3
#define BATSUB 1
#define CARNUM 4

static CComm serial; //�ø��� ����� ���� ���� -- ��� �ҽ����� ���ǹǷ� ���������� ����
Map map; // ��ü �ʿ� ���� �� ����
static Car lineTracer[CARNUM]; //�� �״� ����


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

			//�浹�Ϸ� �Ѵٸ�
			for (int i = 0; i < 4; i++) {
				if (i != caridx) {
					if (lineTracer[i].carPos.first == myNextX && lineTracer[i].carPos.second == myNextY) {

						//�����ض�!!!
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
	
	lineTracer[caridx].addAbsPointer(); //���� ��ǥ���� ������Ű��!
	lineTracer[caridx].relPointer++; //��� ��ǥ���� ������Ű��!

	cout << "route! : "; map.printRealpath(lineTracer[caridx].route[0]); map.printRealpath(lineTracer[caridx].route[1]); cout << endl;

}

int bat_order = 0;
int temp_return = 0;
#define B_minus 20	//���� �ִ뷮 �ּҷ� ����� 70% ��ġ��.	14
#define C_minus 19	//�����ҷ� �����ϱ������ �ִ��ġ��.  16
static float min_rule[CARNUM] = { C_minus + B_minus, C_minus + B_minus, C_minus + 2 * B_minus ,C_minus + 2 * B_minus };
static float max_rule[CARNUM] = { 90.0, 90.0, 90,90 };
static int working_robot_num = CARNUM;
int getout = 0;

int charge_decision(int idx) {
	//work_sum = work_sum + 1.0;
	bat_order = 0;
	for (int i = 0; i < CARNUM; i++) {                  //���°�� ���͸��� ���� �༮�ΰ�?
		if (lineTracer[idx].remain_bat < lineTracer[i].remain_bat)
			bat_order++;
	}

	//���� ������ �����ٸ�
	int posX = lineTracer[idx].carPos.first;
	int posY = lineTracer[idx].carPos.second;

	if (lineTracer[idx].realpath.size() == 0) {
		return GOING_WORK;
	}

	//�� �ϳ��� ������ ������.	
	if (lineTracer[idx].relPointer == (lineTracer[idx].realpath.size() - 1)  &&  (!(posX == 10 && posY == 2))  &&  (!(posX == 10 && posY == 5))  )
	{
		if (min_rule[bat_order] < lineTracer[idx].remain_bat)  //���͸��� 15 15 30 30���� ���� �ִ°�?
		{
			//float temp = ���� ���ϰ������ ��� BFS ��.;      //�ѹ� ���Ҹ�ŭ�� ���͸��� ����.
			temp_return = GOING_WORK;    //�׷��ٸ� �ѹ� �� GOING_WORK.
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
			temp_return = GOING_CHARGE;   //�ƴ϶�� �����ҷ� ����.
										  //lineTracer[idx].remainChrgBat = C_minus;   //�����ұ����� �Ÿ��� �ش��ϴ� ���͸���
			return temp_return;
		}
	}

	//Ǯ�����̸� ���ض�
	if (lineTracer[idx].remain_bat >= max_rule[bat_order])
	{
		//float temp = ���� ���ϰ������ ��� BFS ��.;      //�ѹ� ���Ҹ�ŭ�� ���͸��� ����.
		temp_return = GOING_WORK;    //�׷��ٸ� �ѹ� �� GOING_WORK.
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

	//���ϴ� ���� �����ϸ� ���ض�(���͸� ��� ������ �� ��Ŵ)
	if ((getout > 0) && (idx==highestindex))
	{
		getout -= 1;
		temp_return = GOING_WORK;
		return temp_return;
	}
	return GOING_CHARGE;
}


//���� �ϵ��� ó���� �κ�
void processing() {



	//����Ʈ���̼� �ʱ���ġ �� ���͸� �ܷ� ����
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

	const char *data; //������ ���۽� ����� ����

	time_t start, end; //�ð��� �˷��ִ� ����

	/* ��źκ�
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

	//���� �����ϰ� �ִ� �ϵ��� �˷��ִ� �κ�
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

			//���ǵ� ��ġ �����ϴ� �κ�
			if (!(lineTracer[lineIdx].realpath.empty())) {
				if (lineTracer[lineIdx].realpath[lineTracer[lineIdx].relPointer] == LIFT_DOWN) {
					int axis = map.tranRealLocTOStuffLoc(make_pair(posX, posY)); //������ǥ���� ���� ǥ���ϴ� ���ڷ� ��ȯ
					map.stuffLoc[axis] = YES;
				}
				else if (lineTracer[lineIdx].realpath[lineTracer[lineIdx].relPointer] == LIFT_UP) {
					int axis = map.tranRealLocTOStuffLoc(make_pair(posX, posY));//������ǥ���� ���� ǥ���ϴ� ���ڷ� ��ȯ
					map.stuffLoc[axis] = NO;
				}
			}


			//������ print�ϴ� �κ�
			cout << "line" << lineIdx << "\t" << "absP : " << lineTracer[lineIdx].absPointer << ", absR : " << lineTracer[lineIdx].relPointer <<", posistion : " << posX << ", " << posY << "\t" << "B : " << lineTracer[lineIdx].bat << ", RB : " << lineTracer[lineIdx].remain_bat << endl;

			//������ ��ġ�� �ִٸ�(���� ���̶��)
			if ((posX == 10 && posY == 2) || (posX == 10 && posY == 5)) {

				//�����˰��� ���� �� ��� �ؾ����� ����	(GOING_WORK or GOING_CHARGE)-------------------�缺�� ��Ʈ
				lineState = charge_decision(lineIdx);
				//lineState = GOING_WORK;//--------------------------------------------------------��������� �켱 �־����


				//if (lineTracer[lineIdx].relPointer > 0) {
				//	putRoot(lineIdx);
				//}
				if (lineState == GOING_WORK) {
					if((posX == 10 && posY == 2))
						map.batLoc[0] = true;
					else if((posX == 10 && posY == 5))
						map.batLoc[1] = true;

					tmproute = map.makeroute(posX, posY, lineState); //���� ��� ����
					lineTracer[lineIdx].putPath(tmproute);//���� ��� ����

					//���� �����ϰ� �ִ� �ϵ��� �˷��ִ� �κ�
					for (int i = 0; i < 4; i++) {
						cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
					}
					cout << endl;

					//��� Ȯ�ο�--------------------------------------------------------------------��������� �켱 �־����
					//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
					//	lineTracer[lineIdx].getPath(i)[1];
					//}
					//�浹���� �˰��� ����
					putRoot(lineIdx);
				}

				if (lineTracer[lineIdx].bat + BATADD >= 100) {
					lineTracer[lineIdx].bat = 100;
				}
				else {
					lineTracer[lineIdx].bat += BATADD; //���͸� ����
					lineTracer[lineIdx].remain_bat = lineTracer[lineIdx].bat;
				}
				
			}
			//���ϴ� ���̶��
			else {

				//����� ���� �ϰ� �޴°� ������... �ƴϸ� �״밡 �� ������ �Ʒ��� ������ �����ϴ°� ������... �߸𸣰ڽ�!
				//��ư �޾Ҵٰ� ġ��...!
				lineTracer[lineIdx].routeIdx = 1;//------------------------------------------------------��������� �켱 �־����!


				//�ʱ⿡ ��θ� �޴� ���¶��
				if (lineTracer[lineIdx].realpath.size() == 0) {
					//�����˰��� ���� �� ��� �ؾ����� ����	(GOING_WORK or GOING_CHARGE)-------------------�缺�� ��Ʈ
					lineState = charge_decision(lineIdx);
					//lineState = GOING_WORK;//--------------------------------------------------------��������� �켱 �־����
					tmproute = map.makeroute(posX, posY, lineState); //���� ��� ����
					lineTracer[lineIdx].putPath(tmproute);//���� ��� ����

					//���� �����ϰ� �ִ� �ϵ��� �˷��ִ� �κ�
					for (int i = 0; i < 4; i++) {
						cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
					}
					cout << endl;

					//��� Ȯ�ο�--------------------------------------------------------------------��������� �켱 �־����
					//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
					//	lineTracer[lineIdx].getPath(i)[1];
					//}
					//�浹���� �˰��� ����
					putRoot(lineIdx);
				}
				//ù ������ �ϳ� �����ٸ�!
				else if (lineTracer[lineIdx].routeIdx == 1) {
					
					//������ ��α��� ���Ŷ��!
					if (lineTracer[lineIdx].relPointer == (lineTracer[lineIdx].realpath.size()-1)) {
						//�����˰��� ���� �� ��� �ؾ����� ����	(GOING_WORK or GOING_CHARGE)-------------------�缺�� ��Ʈ
						lineState = charge_decision(lineIdx);

						if (getout > 0) {
							continue;
						}

						//lineState = GOING_WORK;//--------------------------------------------------------��������� �켱 �־����
						tmproute = map.makeroute(posX, posY, lineState); //���� ��� ����
						lineTracer[lineIdx].putPath(tmproute);//���� ��� ����

						//���� �����ϰ� �ִ� �ϵ��� �˷��ִ� �κ�
						for (int i = 0; i < 4; i++) {
							cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
						}
						cout << endl;

						//��� Ȯ�ο�--------------------------------------------------------------------��������� �켱 �־����
						//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
						//	lineTracer[lineIdx].getPath(i)[1];
						//}
						
					}

					//�浹���� �˰��� ����
					putRoot(lineIdx);

				}
				else {} //ù ������ ���� �ȳ����ٸ�

				if (lineTracer[lineIdx].bat - BATADD <= 0) {
					lineTracer[lineIdx].bat = 0;
				}
				else {
					lineTracer[lineIdx].bat -= BATSUB; //���͸� �Ҹ�
					lineTracer[lineIdx].work_bat -= BATSUB;
				}				
			}
		}
		cout << endl;
		//�� 4�뿡�� ����� �޾� ������ �����Ѵ�.
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
	void �浹���� �� �������(i)
	{
		if(i�� �浹�Ϸ���)
			���� ��� index ����
			crash_flag[i] = 1;
			stop�� ��� 2���� ����
		else if(crash_flag[i] == 1)
			index ����
			crash_flag[i] = 0;
			��� 2�� ����
		else
			{index ����}
			��� 2�� ����
	}


		for(4){
			if(�������ΰ�?){
				�����˰��� ����
				"i�� ���Ϸ����ߵǳ�, ��� �����ؾߵǳ�"�� ����
				if(���Ϸ����ߵǸ�)
					map.makeroute(����x,y,��) -> ��� ��ȯ, �̰� �����ص�
					�浹���� �� �������
				else if(��� �����ؾߵǸ�)
				{}

				���͸� ���������ֱ�;
			}
			else if(���ϴ����ΰ�?){
				i���� ���� �ϳ� �����İ� �������
				recv
				if (i�� ���� �ϳ� ���³� ? )
				{
					i�� ���, ������ǥ ����
					if (i�� ���� �ƿ� ������ ? )
					{
						�����˰��� ����
						"i�� ���ؾߵǳ�, �����ؾߵǳ�"�� ����.
						map.makeroute(����x,y,��/����) -> ��� ��ȯ, �̰� �����ص�
					}
					else{}						
					�浹���� �� �������
				}
				else
				{}
				���͸� ���ҽ����ֱ�;
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
