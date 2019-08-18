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

    for(int row =0; row<=MAP_ROW; row++){
        for(int col=0; col<=MAP_COL; col++){
            cout << tmp.getPos(col, row) << "\t";
        }
        cout << endl;
    }

	tmp.BFS(1, 2, 7, 5);

    return 0;
}
