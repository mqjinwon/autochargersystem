#include "Linetracer.h"

bool Car::putRealPath() {
	//차의 입장에서 보는 경로를 표현해야함
	int pre_x, pre_y, now_x, now_y, next_x, next_y;
	int gap_x, gap_y; // 다음값 - 현재값

/*
	왼쪽 방향으로 있을 경우
	- [2~5][3, 7] / [2, 10], [5, 10]
	오른쪽 방향으로 있을 경우
	- [2~5][2, 6]

	1, 0 -> 왼쪽으로 가기
	0, -1 -> 아래로 가기
	0, 1 -> 위로 가기
	-1, 0 -> 오른쪽으로 가기

	차가 오른쪽 방향으로 있을 경우
		위로가면 좌회전
		아래로가면 우회전

		차가 왼쪽 방향으로 있을 경우
		위로 가면 우회전
		아래로 가면 좌회전
*/
	for (int i = 0; i < pathLength(); i++) {


		now_x = path[i][0]; next_x = path[i + 1][0];
		now_y = path[i][1]; next_y = path[i + 1][1];
		gap_x = next_x - now_x; gap_y = next_y = now_y;

		if ((now_x == 2) || (now_x == 3) || (now_x == 4) || (now_x == 5)) {

			//화물쪽에 있을 때
			if ((now_y == 2) || (now_y == 3) || (now_y == 6) || (now_y == 7)) {
				
			}

			//충전기쪽에 있을 때
			else if ((now_y == 10)) {

			}
		}

	}

	return true;
}