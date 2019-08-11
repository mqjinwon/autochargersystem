#include "tracemap.h"


Map::Map() {
	{
		//초기화 과정
		//        for(int row =0; row<=MAP_ROW; row++){
		//            for(int col=0; col<=MAP_COL; col++){
		//                pos[row][col] = 15;
		//            }
		//        }

		pos[0][0] = D; pos[MAP_ROW][0] = R; pos[0][MAP_COL] = L; pos[MAP_ROW][MAP_COL] = U; //corner - 4
		for (int row = 2; row <= MAP_ROW - 2; row++) {
			pos[row][2] = L; pos[row][3] = R; pos[row][6] = L; pos[row][7] = R; //load -16
			if (row == 2 || row == 5) pos[row][10] = L; //charger - 2
		}

		pos[0][MAP_COL] = NODIR; pos[1][MAP_COL] = NODIR; pos[3][MAP_COL] = NODIR; //charger illustion - 6
		pos[4][MAP_COL] = NODIR; pos[6][MAP_COL] = NODIR; pos[7][MAP_COL] = NODIR; //charger illustion - 6

		//column line 6*6 -8 = 28
		for (int row = 1; row <= (MAP_ROW - 1); row++) {
			pos[row][0] = RD;
			pos[row][1] = LUR; //except for [1,1], [6,1]
			pos[row][4] = RDL; //except for [1,4], [6,4]
			pos[row][5] = LUR; //except for [1,5], [6,5]
			pos[row][8] = RDL; //except for [1,8], [6,8]
			pos[row][9] = LU;
		}
		pos[1][1] = R; pos[6][1] = U; pos[1][4] = RD; pos[6][4] = DL; pos[1][5] = UR; pos[6][5] = LU; pos[1][8] = D; pos[6][8] = L; //exception -8

		//first row & last row 8*2 - 4 = 12
		for (int col = 1; col <= MAP_COL - 1; col++) {
			pos[0][col] = L; pos[MAP_ROW][col] = R; //except for [0,4], [0,5], [7,4], [7,5]
		}
		pos[0][4] = DL; pos[0][5] = L; pos[7][4] = R; pos[7][5] = UR; //exception -4

																	  //useless - 8
		pos[1][2] = R; pos[1][3] = R; pos[1][6] = R; pos[1][7] = R;
		pos[6][2] = L; pos[6][3] = L; pos[6][6] = L; pos[6][7] = L;


		//id 0~마지막 숫자,ischeck TRUE로 초기화
		for (int row = 0; row <= MAP_ROW; row++)
			for (int col = 0; col <= MAP_COL; col++) {
				id[row][col] = row * (col + 1) + col;
				ischecked[row][col] = true;
			}

	}
}


//enum {
//	NODIR = 0, //0          1
//	L, U, R, D, //1~4       4
//	LU, UR, RD, DL, //5~8   4
//	LUR, URD, RDL, //9~11   3
//	LURD //12               1
//};
vector<vector<int>> Map::BFS(int s_x, int s_y, int e_x, int e_y)
{
	int depth = 0; //깊이
	int dir = 0; // 갈 수 있는 방향을 알려주는 변수
	int parent = -1;

	//초기화 단계
	queue<vector<int>> que; //(x, y, depth, parent)의 정보를 담는다.

	vector<int> data = { s_x, s_y, depth, -1 };
	que.push(data);

	//bfs 실행
	while (!que.empty()) {

		//이전 값 불러오고 빼내기
		data = que.front();
		dir = getPos(data[0], data[1]);
		depth++;
		parent = data[3];

		que.pop();

		//다음  depth의 갈 수 있는 곳들을 추가하기
		if (dir == NODIR) {
			continue;
		}
		else if (dir == L) {

			if (dir == LU) {

				if (dir == LUR) {

				}
			}
		}
		else if (dir == U) {

			if (dir == UR) {

				if (dir == URD) {

				}
			}
		}
		else if (dir == R) {

			if (dir == RD) {

				if (dir == RDL) {

				}
			}
		}
		else if (dir == D) {

			if (dir == DL) {

			}
		}

	}

	return vector<vector<int>>();
}

