#include <iostream>
#include <vector>
#include <queue>
#include "tracemap.h"

using namespace std;





class Car{

private:
    //vector<int[]> route;

public:
    Car(){
    }
};

int main()
{
    Map tmp;

	enum {
		NODIR = 0, //0          1
		L, U, R, D, //1~4       4
		LU, UR, RD, DL, //5~8   4
		LUR, URD, RDL, //9~11   3
		LURD //12               1
	};

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

	tmp.BFS(1, 2, 10, 5);

    return 0;
}
