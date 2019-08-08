#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define MAP_ROW 7 // 0부터 시작하니깐
#define MAP_COL 10 // 0부터 시작하니깐

//direction
enum{
    NODIR = 0, //0          1
    L, U, R, D, //1~4       4
    LU, UR, RD, DL, //5~8   4
    LUR, URD, RDL, //9~11   3
    LURD //12               1
};

class Map{

private:
    int pos[MAP_ROW+1][MAP_COL+1];

public:
    Map(){
        //검증용
//        for(int row =0; row<=MAP_ROW; row++){
//            for(int col=0; col<=MAP_COL; col++){
//                pos[row][col] = 15;
//            }
//        }

        pos[0][0] = D; pos[MAP_ROW][0] = R; pos[0][MAP_COL] = L; pos[MAP_ROW][MAP_COL] = U; //corner - 4
        for(int row =2; row <= MAP_ROW-2; row++){
            pos[row][2] = L; pos[row][3] = R; pos[row][6] = L; pos[row][7] =R; //load -16
            if(row == 2 || row == 5) pos[row][10] =L; //charger - 2
        }

        pos[0][MAP_COL] = NODIR; pos[1][MAP_COL] = NODIR; pos[3][MAP_COL] = NODIR; //charger illustion - 6
        pos[4][MAP_COL] = NODIR; pos[6][MAP_COL] = NODIR; pos[7][MAP_COL] = NODIR; //charger illustion - 6

        //column line 6*6 -8 = 28
        for(int row=1; row <= (MAP_ROW-1); row++){
            pos[row][0] = RD;
            pos[row][1] = LUR; //except for [1,1], [6,1]
            pos[row][4] = RDL; //except for [1,4], [6,4]
            pos[row][5] = LUR; //except for [1,5], [6,5]
            pos[row][8] = RDL; //except for [1,8], [6,8]
            pos[row][9] = LU;
        }
        pos[1][1]=R; pos[6][1]=U; pos[1][4]=RD; pos[6][4]=DL; pos[1][5]=UR; pos[6][5]=LU; pos[1][8]=D; pos[6][8]=L; //exception -8

        //first row & last row 8*2 - 4 = 12
        for(int col=1; col<=MAP_COL-1; col++){
            pos[0][col] = L; pos[MAP_ROW][col] = R; //except for [0,4], [0,5], [7,4], [7,5]
        }
        pos[0][4]=DL; pos[0][5]=L; pos[7][4]=R; pos[7][5]=UR; //exception -4

        //useless - 8
        pos[1][2]=R; pos[1][3]=R; pos[1][6]=R; pos[1][7]=R;
        pos[6][2]=L; pos[6][3]=L; pos[6][6]=L; pos[6][7]=L;
    }



    int getPos(int x, int y){
        return pos[x][y];
    }
};

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
            cout << tmp.getPos(row, col) << "\t";
        }
        cout << endl;
    }
    return 0;
}

