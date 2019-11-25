#include "Comm.h"
static CComm serial; //�ø��� ����� ���� ���� -- ��� �ҽ����� ���ǹǷ� ���������� ����

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
#define BATADD 5
#define BATSUB 0.3

static float min_rule[4] = { 15.0, 15.0, 30,30 };   //�ο����� �� �ִ� ���� �ִ뷮���ٴ� Ŀ�� ��.
													//�� ���� ���̵� �� �����ٴ� Ŀ�� ��.
													//�ȱ׷��� ���� ������ ���������� ���� ����.
													//rule�� ��� ���� A��ŭ �÷��ָ� ���͸��� ����� A���Ϸ� �ȳ�����.
													//static float max_rule[N] = { 60.0, 70.0, 80,90 };
static float max_rule[4] = { 90.0, 90.0, 90,90 };

bool crash_flag[4] = { 0,0 };

void putRoot(int caridx) {

	//�浹�Ϸ� �Ѵٸ�
	if (false/*�浹�Ϸ����Ѵٸ�*/) {
		//���� ��� index ����
		crash_flag[caridx] = 1;
	}
	else if (false/*�浹�Ϸ��� ��Ȳ�� �����ٸ�*/) {
		//�ε��� ����
		crash_flag[caridx] = 0;
	}
	else {

	}
}
//bool crash_flag[4] = { 0,0 }
//void �浹���� �� �������(i)
//{
//	if (i�� �浹�Ϸ���)
//		���� ��� index ����
//		crash_flag[i] = 1;
//	stop�� ��� 2���� ����
//	else if (crash_flag[i] == 1)
//		index ����
//		crash_flag[i] = 0;
//	��� 2�� ����
//	else
//	{
//		index ����
//	}
//	��� 2�� ����
//}

//���� �ϵ��� ó���� �κ�
void processing() {

	Map map; // ��ü �ʿ� ���� �� ����

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
	lineTracer[3].carPos.first = 7;//10;
	lineTracer[3].carPos.second = 2;//5;
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
	cout << endl;

	while (1) {

		int lineState = WORK_WAIT;
		int posX, posY = 0;
		vector<vector<int>> route;

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
			cout << "line" << lineIdx << "\t" << "absP : " << lineTracer[lineIdx].absPointer << ", absR : " << lineTracer[lineIdx].relPointer <<", posistion : " << posX << ", " << posY << "\t" << "battery : " << lineTracer[lineIdx].bat << endl;

			//������ ��ġ�� �ִٸ�(���� ���̶��)
			if ((posX == 10 && posY == 2) || (posX == 10 && posY == 5)) {

				//�����˰��� ���� �� ��� �ؾ����� ����	(GOING_WORK or GOING_CHARGE)-------------------�缺�� ��Ʈ
				//linestate = �����˰���()
				lineState = GOING_WORK;//--------------------------------------------------------��������� �켱 �־����

				if (lineState == GOING_WORK) {
					route = map.makeroute(posX, posY, lineState); //���� ��� ����
					lineTracer[lineIdx].putPath(route);//���� ��� ����

					//���� �����ϰ� �ִ� �ϵ��� �˷��ִ� �κ�
					for (int i = 0; i < 4; i++) {
						cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
					}
					cout << endl;

					//��� Ȯ�ο�--------------------------------------------------------------------��������� �켱 �־����
					//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
					//	lineTracer[lineIdx].getPath(i)[1];
					//}
					//�浹���� �˰��� ����---------------------------------------------------------------���� ����

				}
				else if (lineState == GOING_CHARGE) {
					//empty
				}
				if (lineTracer[lineIdx].bat + BATADD >= 100) {
					lineTracer[lineIdx].bat = 100;
				}
				else {
					lineTracer[lineIdx].bat += BATADD; //���͸� ����
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
					//linestate = �����˰���()
					lineState = GOING_WORK;//--------------------------------------------------------��������� �켱 �־����
					route = map.makeroute(posX, posY, lineState); //���� ��� ����
					lineTracer[lineIdx].putPath(route);//���� ��� ����

					//���� �����ϰ� �ִ� �ϵ��� �˷��ִ� �κ�
					for (int i = 0; i < 4; i++) {
						cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
					}
					cout << endl;

					//��� Ȯ�ο�--------------------------------------------------------------------��������� �켱 �־����
					//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
					//	lineTracer[lineIdx].getPath(i)[1];
					//}
					//�浹���� �˰��� ����---------------------------------------------------------------���� ����
				}

				//ù ������ �ϳ� �����ٸ�!
				if (lineTracer[lineIdx].routeIdx == 1) {
					
					//������ ��α��� ���Ŷ��!
					if (lineTracer[lineIdx].relPointer == (lineTracer[lineIdx].realpath.size() - 1) && lineTracer[lineIdx].realpath.size() > 0) {
						//�����˰��� ���� �� ��� �ؾ����� ����	(GOING_WORK or GOING_CHARGE)-------------------�缺�� ��Ʈ
						//linestate = �����˰���()
						lineState = GOING_WORK;//--------------------------------------------------------��������� �켱 �־����
						route = map.makeroute(posX, posY, lineState); //���� ��� ����
						lineTracer[lineIdx].putPath(route);//���� ��� ����

						//���� �����ϰ� �ִ� �ϵ��� �˷��ִ� �κ�
						for (int i = 0; i < 4; i++) {
							cout << i << "th: s : " << map.storedWork[i].first << ", e : " << map.storedWork[i].second << "\t";
						}
						cout << endl;

						//��� Ȯ�ο�--------------------------------------------------------------------��������� �켱 �־����
						//for (int i = 0; i < lineTracer[lineIdx].pathLength(); i++) {
						//	lineTracer[lineIdx].getPath(i)[1];
						//}
						//�浹���� �˰��� ����---------------------------------------------------------------���� ����
					}

					lineTracer[lineIdx].addAbsPointer(); //������ǥ���� ������Ű��!
					lineTracer[lineIdx].relPointer++; //�����ǥ���� ������Ű��!

				}
				else {} //ù ������ ���� �ȳ����ٸ�

				if (lineTracer[lineIdx].bat - BATADD <= 0) {
					lineTracer[lineIdx].bat = 0;
				}
				else {
					lineTracer[lineIdx].bat -= BATSUB; //���͸� �Ҹ�
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
