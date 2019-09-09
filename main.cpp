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

	full_map = tmp.BFS(10, 2, 5, 3);
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

	cout << endl;

	for (int row = 0; row <= MAP_ROW; row++) {
		for (int col = 0; col <= MAP_COL; col++) {
			cout << map[row][col] << "\t";
		}
		cout << endl;
	}

    return 0;
}
