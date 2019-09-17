#include "tracemap.h"

Map::Map() {
	{
		pos[0][0] = D; pos[MAP_ROW][0] = R; pos[0][MAP_COL] = L; pos[MAP_ROW][MAP_COL] = U; //corner - 4
		for (int row = 2; row <= MAP_ROW - 2; row++) {
			pos[row][2] = L; pos[row][3] = R; pos[row][6] = L; pos[row][7] = R; //load -16
			if (row == 2 || row == 5) pos[row][10] = L; //charger - 2
		}

		pos[0][MAP_COL] = NODIR; pos[1][MAP_COL] = NODIR; pos[3][MAP_COL] = NODIR; //charger illustion - 6
		pos[4][MAP_COL] = NODIR; pos[6][MAP_COL] = NODIR; pos[7][MAP_COL] = NODIR; //charger illustion - 6

		//column line 6*6 - 8 = 28
		for (int row = 1; row <= (MAP_ROW - 1); row++) {
			pos[row][0] = RD;
			pos[row][1] = LUR; //except for [1,1], [6,1]
			pos[row][4] = RDL; //except for [1,4], [6,4]
			pos[row][5] = LUR; //except for [1,5], [6,5]
			pos[row][8] = RDL; //except for [1,8], [6,8]
			pos[row][9] = LU; // except for [2,9], [5,10]
		}
		pos[1][1] = R; pos[6][1] = U; pos[1][4] = RD; pos[6][4] = DL; pos[1][5] = UR; pos[6][5] = LU; pos[1][8] = D; pos[6][8] = L;  pos[2][9] = LUR; pos[5][9] = LUR;//exception - 10

		//first row & last row 8*2 - 4 = 12
		for (int col = 1; col <= MAP_COL - 1; col++) {
			pos[0][col] = L; pos[MAP_ROW][col] = R; //except for [0,4], [0,5], [7,4], [7,5]
		}
		pos[0][4] = DL; pos[0][5] = L; pos[7][4] = R; pos[7][5] = UR; //exception -4

		//useless - 8
		pos[1][2] = R; pos[1][3] = R; pos[1][6] = R; pos[1][7] = R;
		pos[6][2] = L; pos[6][3] = L; pos[6][6] = L; pos[6][7] = L;


		//id 0~������ ����,ischeck TRUE�� �ʱ�ȭ
		for (int row = 0; row <= MAP_ROW; row++)
			for (int col = 0; col <= MAP_COL; col++) {
				id[row][col] = row * (col + 1) + col;
				ischecked[row][col] = true;
			}

		}
}

vector<vector<int>> Map::BFS(int s_x, int s_y, int e_x, int e_y)
{
	int depth = 0; //����
	int dir = 0; // �� �� �ִ� ������ �˷��ִ� ����
	int parent = -1;
	int id = 0;
	

	//�ʱ�ȭ �ܰ�
	queue<vector<int>> que; //(x, y, depth, parent)�� ������ ��´�.

	vector<int> pre_data = { s_x, s_y, depth, id, parent}; //���� ������ �ǵ帮�� �κ�
	vector<int> now_data; // ���� �����͵� �ǵ帮�� �κ�

	vector<vector<int>> all_data; // ��� ��� ����
	vector<vector<int>> path; // ���� ��� ǥ��
	
	que.push(pre_data);

	//bfs ����
	while (!que.empty()) {

		//���� �� �ҷ����� ������
		pre_data = que.front();
		putCheck(pre_data[0], pre_data[1]);
		dir = getPos(pre_data[0], pre_data[1]);
		depth = pre_data[2] + 1;
		parent = pre_data[3];

		////�ȳ��� --------------------------------------------------------------------------------------------���߿� �������� ������!!
		//cout << "x : " << pre_data[0] << ", y : " << pre_data[1] << ", depth : "
		//	<< pre_data[2] << ", id : " << pre_data[3] << ", parent : " << pre_data[4] << endl;

		//��ǥ��ġ�� ã���� Ż��
		if (pre_data[0] == e_x && pre_data[1] == e_y) {
			path.push_back(pre_data); // ��ó�� ������ ����

			//���� ��� ��� ���� �κ�
			vector<int> tmp = pre_data;
			for (int i = depth-1; i > 0; i--) {
				for (int j = 0; j < all_data.size(); j++) {
					if (all_data[j][3] == tmp[4]) {
						tmp = all_data[j];
						path.push_back(tmp);
					}
				}
			}
			initCheck();
			cout << "============================================" << endl;
			return path;
		}

		//��� ��� ��ϵ��� �����ϱ�
		all_data.push_back(pre_data);

		que.pop();

		//����  depth�� �� �� �ִ� ������ �߰��ϱ�
		if (dir == NODIR) {
			continue;
		}
		else if (dir == L || dir == LU || dir == LUR) {

			//�� �� �ִ� ��� �߰��ϱ�
			if (getCheck(pre_data[0] - 1, pre_data[1])) {
				//putCheck(pre_data[0] - 1, pre_data[1]);
				now_data = { pre_data[0] - 1, pre_data[1], depth, ++id, parent };
				que.push(now_data);
			}


			if (dir == LU || dir == LUR) {
				if (getCheck(pre_data[0], pre_data[1] - 1)) {
					//putCheck(pre_data[0], pre_data[1] - 1);
					now_data = { pre_data[0], pre_data[1] - 1, depth, ++id, parent };
					que.push(now_data);
				}

				if (dir == LUR) {
					if (getCheck(pre_data[0] + 1, pre_data[1])) {
						//putCheck(pre_data[0] + 1, pre_data[1]);
						now_data = { pre_data[0] + 1, pre_data[1], depth, ++id, parent };
						que.push(now_data);

					}
				}
			}
		}
		else if (dir == U || dir == UR || dir == URD) {
			if (getCheck(pre_data[0], pre_data[1] - 1)) {
				//putCheck(pre_data[0], pre_data[1] - 1);
				now_data = { pre_data[0], pre_data[1] - 1, depth, ++id, parent };
				que.push(now_data);

			}

			if (dir == UR || dir == URD) {
				if (getCheck(pre_data[0] + 1, pre_data[1])) {
					//putCheck(pre_data[0] + 1, pre_data[1]);
					now_data = { pre_data[0] + 1, pre_data[1], depth, ++id, parent };
					que.push(now_data);

				}

				if (dir == URD) {
					if (getCheck(pre_data[0], pre_data[1] + 1)) {
						//putCheck(pre_data[0], pre_data[1] + 1);
						now_data = { pre_data[0], pre_data[1] + 1, depth, ++id, parent };
						que.push(now_data);

					}

				}
			}
		}
		else if (dir == R || dir == RD || dir == RDL) {
			if (getCheck(pre_data[0] + 1, pre_data[1])) {
				//putCheck(pre_data[0] + 1, pre_data[1]);
				now_data = { pre_data[0] + 1, pre_data[1], depth, ++id, parent };
				que.push(now_data);


			}

			if (dir == RD || dir == RDL) {
				if (getCheck(pre_data[0], pre_data[1] + 1)) {
					//putCheck(pre_data[0], pre_data[1] + 1);
					now_data = { pre_data[0], pre_data[1] + 1, depth, ++id, parent };
					que.push(now_data);

				}

				if (dir == RDL) {
					if (getCheck(pre_data[0] - 1, pre_data[1])) {
						//putCheck(pre_data[0] - 1, pre_data[1]);
						now_data = { pre_data[0] - 1, pre_data[1], depth, ++id, parent };
						que.push(now_data);

					}

				}
			}
		}
		else if (dir == D || dir == DL) {
			if (getCheck(pre_data[0], pre_data[1] + 1)) {
				//putCheck(pre_data[0], pre_data[1] + 1);
				now_data = { pre_data[0], pre_data[1] + 1, depth, ++id, parent };
				que.push(now_data);


			}

			if (dir == DL) {
				if (getCheck(pre_data[0] - 1, pre_data[1])) {
					//putCheck(pre_data[0] - 1, pre_data[1]);
					now_data = { pre_data[0] - 1, pre_data[1], depth, ++id, parent };
					que.push(now_data);


				}

			}
		}

	}



	return vector<vector<int>>();
}


