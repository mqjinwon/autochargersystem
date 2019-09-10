#include <iostream>
#include <vector>
#include <queue>
#include "tracemap.h"
#include "Linetracer.h"
#include "SerialComm.h"

using namespace std;


static CSerialComm serial; //시리얼 통신을 위한 변수 -- 모든 소스에서 사용되므로 전역변수로 선언


int main(){

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

	int map[MAP_ROW+1][MAP_COL+1] = { 0, };

	for (int i = 0; i < full_map.size(); i++) {
		cout << "x : " << full_map[i][0] << ", y : " << full_map[i][1] << ", depth : "
			<< full_map[i][2] << ", id : " << full_map[i][3] << ", parent : " << full_map[i][4] << endl;
		map[full_map[i][1]][full_map[i][0]] = 1;
	}

	for (int i = 0; i < full_map.size(); i++) {
		car.getPath(i);
	}
	
	car.putRealPath();

	//enum {
	//	GO_F, // forward 앞으로 가는 것
	//	GO_R, // right 오른쪽으로 가는 것
	//	GO_B, // backward 뒤쪽으로 가는 것
	//	GO_L, // left 왼쪽으로 가는 것
	//	L_B, // 빽하면서 왼쪽으로 가는 것	
	//	R_B, // 뺵하면서 오른쪽으로 가는 것
	//	STOP,
	//	ROTATE_180 // 180도 회전 시키기
	//};

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
		case GO_B:
			cout << "GO_B" << "\t";
			break;
		case GO_L:
			cout << "GO_L" << "\t";
			break;
		case L_B:
			cout << "L_B" << "\t";
			break;
		case R_B:
			cout << "R_B" << "\t";
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

    return 0;
}
