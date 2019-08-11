#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <deque>

using namespace std;

#define MAP_ROW 7 // �迭����(0���� ����)
#define MAP_COL 10 // �迭����(0���� ����)

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
	int pos[MAP_ROW + 1][MAP_COL + 1]; // ��ǥ�� ���� �� �� �ִ� �� Ȯ��
	int id[MAP_ROW + 1][MAP_COL + 1]; //���� ��ġ ǥ��
	bool ischecked[MAP_ROW + 1][MAP_COL + 1]; // ���� ���� üũ�س��� ���̻� ������ �ϱ�

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

	//��ǥ�� ��ȯ
	vector<vector<int>> BFS(int s_x, int s_y, int e_x, int e_y);
	
};