#include "SerialComm.h"
static CSerialComm serial; //�ø��� ����� ���� ���� -- ��� �ҽ����� ���ǹǷ� ���������� ����

#include <iostream>
#include <vector>
#include <queue>
#include "tracemap.h"
#include "Linetracer.h"

using namespace std;

#define PORTNUM "COM15"


//���� �ϵ��� ó���� �κ�
void processing(vector<Car> carLIst) {

	/////step 1. initilizing part/////

	int carNum = carLIst.size(); // ����� ���� �ִ��� Ȯ���ϱ� ���� ����
	vector<int> batterysize(carNum); // ���͸� ���� �ޱ� ���ؼ� ����ϴ� �༮
	Map map; // ��ü �ʿ� ���� �� ����

	//serial port connection
	BYTE data;
	while (1) {
		
		if (!serial.connect(PORTNUM)) {
			printf("connect faliled\n");
		}
		else {
			printf("connect successed\n");
			break;
		}
		Sleep(500);
	}
	
	//main process
	while (1) {

		/////step 2. battery checking/////
		
		//step 2.1 ������ ���͸� ���¸� ����(uart ����� �Ѵ�.)

		//step 2.2 �˰��� ���� �����ؾ��� ���� ������Ų��(�켱�������� �˷��� ex) [1,0, 0 ,2])
		//if(�����Ⱑ ������ ���) -- state =  CANTCHARGE ���·� ����, charge_flag = true�� ����
		//else -- state =  GOING_CHARGE ���·� ����, charge_flag = true�� ����

		/////step 3. working checking/////



		/////step 4. route checking/////
	}

}






int main(){

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
	
	//BYTE data;
	//while (1) {
	//	// STEP 1. SerialPort Connect
	//	if (!serial.connect(PORTNUM)) {
	//		printf("connect faliled\n");
	//	}
	//	else {
	//		printf("connect successed\n");
	//		break;
	//	}
	//	Sleep(500);
	//}

	//serial.sendCommand('a');
	//while (1) {
	//	if (serial.recvCommand(data)) {
	//		cout << "data : " << data << endl;
	//		
	//		break;
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

	full_map = tmp.BFS(7, 2, 10, 2);
	car.putPath(full_map);
	full_map = tmp.BFS(10, 2, 3, 5);
	car.putPath(full_map);

	full_map = tmp.BFS(3, 5, 6, 3);
	car.putPath(full_map);

	full_map = tmp.BFS(6, 3, 1, 1);
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
		}
	}

	cout << endl << endl;

	for (int row = 0; row <= MAP_ROW; row++) {
		for (int col = 0; col <= MAP_COL; col++) {
			cout << map[row][col] << "\t";
		}
		cout << endl;
	}

	serial.disconnect(); //�������� �ø��� ��� �����ش�.
    return 0;
}
