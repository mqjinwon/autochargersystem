#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <deque>

using namespace std;

#define MAP_ROW 7 // �迭����(0���� ����)
#define MAP_COL 10 // �迭����(0���� ����)

//direction
typedef enum {
	NODIR = 0, //0          1
	L, U, R, D, //1~4       4
	LU, UR, RD, DL, //5~8   4
	LUR, URD, RDL, //9~11   3
	LURD //12               1
} MAP_DIR;

class Map {

private:
	int pos[MAP_ROW + 1][MAP_COL + 1]; // ��ǥ�� ���� �� �� �ִ� �� Ȯ��
	int id[MAP_ROW + 1][MAP_COL + 1]; //���� ��ġ ǥ��
	bool ischecked[MAP_ROW + 1][MAP_COL + 1]; // ���� ���� üũ�س��� ���̻� ������ �ϱ�

public:
	Map();

	int getPos(int x_col, int y_row) {
		return pos[y_row][x_col];
	}

	bool getCheck(int x_col, int y_row) {
		return ischecked[y_row][x_col];
	}

	void putCheck(int x_col, int y_row) {
		ischecked[y_row][x_col] = false;
	}

	void initCheck() {
		for (int row = 0; row <= MAP_ROW; row++)
			for (int col = 0; col <= MAP_COL; col++) {
				ischecked[row][col] = true;
			}
	}

	//��ǥ�� ��ȯ
	vector<vector<int>> BFS(int s_x, int s_y, int e_x, int e_y);

};