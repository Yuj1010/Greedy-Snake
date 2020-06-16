#pragma once

#include <windows.h>
#include <stdio.h>
#include "Model.h"


void SetPos(int X, int Y)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = X;
	coord.Y = Y;

	SetConsoleCursorPosition(hStdout, coord);
}

// ��ʾ��ǽ
void DisplayWall(int width, int height)
{
	int i;

	// �ϱߵ�ǽ
	SetPos(0, 0);
	for (i = 0; i < width + 2; i++) {
		printf("��");
	}

	// �±ߵ�ǽ
	SetPos(0, height + 1);
	for (i = 0; i < width + 2; i++) {
		printf("��");
	}

	// ��ߵ�ǽ
	for (i = 0; i < height + 2; i++) {
		SetPos(0, i);
		printf("��");
	}

	// �ұߵ�ǽ
	for (i = 0; i < height + 2; i++) {
		SetPos(2 * (width + 1), i);
		printf("��");
	}
}

void DisplaySnakeNode(Position pos)
{
	SetPos(2 * (pos.x + 1), pos.y + 1);
	printf("��");
}

void CleanSnakeNode(Position pos)
{
	SetPos(2 * (pos.x + 1), pos.y + 1);
	printf("  ");
}


void DisplayFood(Position pos)
{
	SetPos(2 * (pos.x + 1), pos.y + 1);
	printf("��");
}

void DisplaySnake(const Snake *pSnake)
{
	Node *pNode = pSnake->pHead;
	while (pNode) {
		DisplaySnakeNode(pNode->data);
		pNode = pNode->pNext;
	}
}