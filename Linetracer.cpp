#include "Linetracer.h"

bool Car::putRealPath() {
	//���� ���忡�� ���� ��θ� ǥ���ؾ���
	int pre_x, pre_y, now_x, now_y, next_x, next_y;
	int gap_x, gap_y; // ������ - ���簪

/*
	���� �������� ���� ���
	- [2~5][3, 7] / [2, 10], [5, 10]
	������ �������� ���� ���
	- [2~5][2, 6]

	1, 0 -> �������� ����
	0, -1 -> �Ʒ��� ����
	0, 1 -> ���� ����
	-1, 0 -> ���������� ����

	���� ������ �������� ���� ���
		���ΰ��� ��ȸ��
		�Ʒ��ΰ��� ��ȸ��

		���� ���� �������� ���� ���
		���� ���� ��ȸ��
		�Ʒ��� ���� ��ȸ��
*/
	for (int i = 0; i < pathLength(); i++) {


		now_x = path[i][0]; next_x = path[i + 1][0];
		now_y = path[i][1]; next_y = path[i + 1][1];
		gap_x = next_x - now_x; gap_y = next_y = now_y;

		if ((now_x == 2) || (now_x == 3) || (now_x == 4) || (now_x == 5)) {

			//ȭ���ʿ� ���� ��
			if ((now_y == 2) || (now_y == 3) || (now_y == 6) || (now_y == 7)) {
				
			}

			//�������ʿ� ���� ��
			else if ((now_y == 10)) {

			}
		}

	}

	return true;
}