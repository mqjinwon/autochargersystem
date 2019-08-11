#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <deque>

using namespace std;

#define MAP_ROW 7 // 배열기준(0부터 시작)
#define MAP_COL 10 // 배열기준(0부터 시작)

//direction
enum {
	NODIR = 0, //0          1
	L, U, R, D, //1~4       4
	LU, UR, RD, DL, //5~8   4
	LUR, URD, RDL, //9~11   3
	LURD //12               1
};

class Map {

private:
	int pos[MAP_ROW + 1][MAP_COL + 1]; // 좌표에 따른 갈 수 있는 곳 확인
	int id[MAP_ROW + 1][MAP_COL + 1]; //고유 위치 표시
	bool ischecked[MAP_ROW + 1][MAP_COL + 1]; // 갔던 곳은 체크해놔서 더이상 못가게 하기

public:
	Map();

	int getPos(int x, int y) {
		return pos[x][y];
	}

	bool getCheck(int x, int y) {
		return ischecked[x][y];
	}

	void putCheck(int x, int y) {
		ischecked[x][y] = false;
	}

	//좌표값 반환
	vector<vector<int>> BFS(int s_x, int s_y, int e_x, int e_y);
	
};