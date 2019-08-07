#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define MAP_ROW 8-1 // 0부터 시작하니깐
#define MAP_COL 11-1 // 0부터 시작하니깐

//direction
enum{
    NODIR = 0, //1
    L, U, R, D, //4
    LU, LR, LD, UR, UD, RD, //6
    LUR, LUD, LRD, URD, //4
    LURD //1
};

class Map{

private:
    int pos[MAP_ROW][MAP_COL];

public:
    Map(){
        pos[0][0] = D;, pos[MAP_ROW][0] = R; pos[0][MAP_COL] = L; pos[MAP_ROW][MAP_COL] = U; //corner - 4
        for(int row =2; row<=5; row++){
            pos[row][2] = L; pos[row][3] = R; pos[row][6] = L; pos[row][7] =R; //load -16
            if(row == 2 || row == 5) pos[row][10] =L; //charger - 2
        }
        pos[0][10], pos[1][10], pos[3][10], pos[4][10], pos[6][10], pos[7][10] = NODIR; //charger illustion - 6


    }
};

class Car{

private:
    vector<int[]> route;

public:
    Car(){
        route
    }
};

int main()
{

    return 0;
}

