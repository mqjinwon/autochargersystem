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

		/////step 3. working checking/////

		/////step 4. route checking/////
	}

}






int main(){

	vector<Car> linetracer;

	linetracer.push_back(Car(2, 2));
	linetracer.push_back(Car(3, 4));
	linetracer.push_back(Car(7, 5));
	linetracer.push_back(Car(10, 2));

	processing(linetracer);

	cout << carID << endl;

	//vector<vector<int>> full_map;
	//Car car;
 //   Map tmp;
	//
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

 //   for(int row =0; row<=MAP_ROW; row++){
 //       for(int col=0; col<=MAP_COL; col++){
	//		switch (tmp.getPos(col, row)) {
	//		case NODIR:
	//			cout << "NODIR" << "\t";
	//			break;
	//		case L:
	//			cout << "L" << "\t";
	//			break;
	//		case U:
	//			cout << "U" << "\t";
	//			break;
	//		case R:
	//			cout << "R" << "\t";
	//			break;
	//		case D:
	//			cout << "D" << "\t";
	//			break;
	//		case LU:
	//			cout << "LU" << "\t";
	//			break;
	//		case UR:
	//			cout << "UR" << "\t";
	//			break;
	//		case RD:
	//			cout << "RD" << "\t";
	//			break;
	//		case DL:
	//			cout << "DL" << "\t";
	//			break;
	//		case LUR:
	//			cout << "LUR" << "\t";
	//			break;
	//		case URD:
	//			cout << "URD" << "\t";
	//			break;
	//		case RDL:
	//			cout << "RDL" << "\t";
	//			break;
	//		case LURD:
	//			cout << "NODIR" << "\t";
	//			break;
	//		}

 //           
 //       }
 //       cout << endl;
 //   }

	//full_map = tmp.BFS(7, 2, 10, 2);
	//car.putPath(full_map);

	//int map[MAP_ROW+1][MAP_COL+1] = { 0, };

	//for (int i = 0; i < full_map.size(); i++) {
	//	cout << "x : " << full_map[i][0] << ", y : " << full_map[i][1] << ", depth : "
	//		<< full_map[i][2] << ", id : " << full_map[i][3] << ", parent : " << full_map[i][4] << endl;
	//	map[full_map[i][1]][full_map[i][0]] = 1;
	//}

	//for (int i = 0; i < full_map.size(); i++) {
	//	car.getPath(i);
	//}
	//
	//car.putRealPath();

	////enum {
	////	GO_F, // forward ������ ���� ��
	////	GO_R, // right ���������� ���� ��
	////	GO_B, // backward �������� ���� ��
	////	GO_L, // left �������� ���� ��
	////	L_B, // ���ϸ鼭 �������� ���� ��	
	////	R_B, // ���ϸ鼭 ���������� ���� ��
	////	STOP,
	////	ROTATE_180 // 180�� ȸ�� ��Ű��
	////};

	//cout << endl;

	//for (int i = 0; i < car.realpathLength(); i++) {
	//	if (i % 10 == 0 && i != 0)
	//		cout << endl;

	//	switch (car.getRealPath(i)) {
	//	case GO_F:
	//		cout << "GO_F" << "\t";
	//		break;
	//	case GO_R:
	//		cout << "GO_R" << "\t";
	//		break;
	//	case GO_B:
	//		cout << "GO_B" << "\t";
	//		break;
	//	case GO_L:
	//		cout << "GO_L" << "\t";
	//		break;
	//	case L_B:
	//		cout << "L_B" << "\t";
	//		break;
	//	case R_B:
	//		cout << "R_B" << "\t";
	//		break;
	//	case STOP:
	//		cout << "STOP" << "\t";
	//		break;
	//	case ROTATE_180:
	//		cout << "ROTATE_180" << "\t";
	//		break;
	//	}
	//}

	//cout << endl << endl;

	//for (int row = 0; row <= MAP_ROW; row++) {
	//	for (int col = 0; col <= MAP_COL; col++) {
	//		cout << map[row][col] << "\t";
	//	}
	//	cout << endl;
	//}

	//serial.disconnect(); //�������� �ø��� ��� �����ش�.
    return 0;
}
