#pragma once
#include <queue>
#include "tracemap.h"



class Node
{
private:
	int pos[2] = { 0,0 };

public:
	Node *left = nullptr;
	Node *up = nullptr;
	Node *right = nullptr;
	Node *down = nullptr;

	int* getPos() { return pos; }
	void putPos(int _pos[2]) { pos[0] = _pos[0]; pos[1] = _pos[1];}
	
};


class mapTree
{
public:
	Node *root = nullptr;

	Map map; //맵을 알기 위해서

	mapTree(int x, int y);

	bool add(int data);

	bool isEmpty()
	{
		if (root == nullptr)
			return true;
		else
			return false;
	}
};


