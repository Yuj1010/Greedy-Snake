#pragma once

#include "Model.h"
#include "View.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


/*
typedef struct Game {
	Snake		snake;
	Position	foodPosition;

	int	score;	// ��ǰ�÷�
	int scorePerFood;	// ÿ��һ��ʳ��÷�
i	nt speed;	//	�ߵ�ǰ���ٶ�
}	Game;
*/
void SnakeInit(Snake *pSnake)
{
	int i;

	pSnake->pHead = NULL;
	for (i = 0; i < 3; i++) {
		Position pos;
		pos.x = i + 5;
		pos.y = 5;

		Node *pNewNode = (Node *)malloc(sizeof(Node));
		assert(pNewNode);
		pNewNode->data = pos;
		pNewNode->pNext = pSnake->pHead;
		pSnake->pHead = pNewNode;
	}

	pSnake->direction = RIGHT;
}

// 1 �ص�
// 0 ���ص�
int IsOverlapSnake(int x, int y, const Snake *pSnake)
{
	Node *pNode;
	for (pNode = pSnake->pHead; pNode; pNode = pNode->pNext) {
		if (pNode->data.x == x && pNode->data.y == y) {
			return 1;
		}
	}

	return 0;
}

// ����ʳ��
// 1. ���
// 2. ��Χ��
// 3. �������ص�
void FoodInit(Position *pFood, int width, int height, const Snake *pSnake)
{
	int x, y;

	do {
		x = rand() % width;
		y = rand() % height;
	} while (IsOverlapSnake(x, y, pSnake));

	pFood->x = x;
	pFood->y = y;
}

void GameInit(Game *pGame)
{
	pGame->width = 28;
	pGame->height = 32;
	system("mode con cols=60 lines=38");

	SnakeInit(&(pGame->snake));
	FoodInit(
		&(pGame->foodPosition), 
		pGame->width, 
		pGame->height,
		&(pGame->snake));

	pGame->score = 0;
	pGame->scorePerFood = 10;
	pGame->speed = 200; // ����
}

// �õ���һ��������
Position GetNextPosition(const Snake *pSnake)
{
	Position nextPos;
	nextPos.x = pSnake->pHead->data.x;
	nextPos.y = pSnake->pHead->data.y;

	switch (pSnake->direction) {
	case UP:
		nextPos.y -= 1;
		break;
	case DOWN:
		nextPos.y += 1;
		break;
	case LEFT:
		nextPos.x -= 1;
		break;
	case RIGHT:
		nextPos.x += 1;
		break;
	}

	return nextPos;
}

// ͷ��
void PushFront(Snake *pSnake, Position nextPos)
{
	Node *pNewNode = (Node *)malloc(sizeof(Node));
	assert(pNewNode);
	pNewNode->data = nextPos;

	pNewNode->pNext = pSnake->pHead;
	pSnake->pHead = pNewNode;

	DisplaySnakeNode(nextPos);
}

// βɾ
void PopBack(Snake *pSnake)
{
	Node *pNode = pSnake->pHead;
	while (pNode->pNext->pNext != NULL) {
		pNode = pNode->pNext;
	}

	CleanSnakeNode(pNode->pNext->data);
	free(pNode->pNext);
	pNode->pNext = NULL;
}

// 1 ��ʾײ��
// 0 ��ʾû��
int IsCrashByWall(int width, int height, Position nextPos)
{
	if (nextPos.x < 0 || nextPos.x >= width) {
		return 1;
	}

	if (nextPos.y < 0 || nextPos.y >= height) {
		return 1;
	}

	return 0;
}

// 1 ��ʾײ��
// 0 ��ʾûײ
int IsCrashHimself(const Snake *pSnake)
{
	Node *pHead = pSnake->pHead;	// ��һ�����
	Node *pNode = pSnake->pHead->pNext;	// �ڶ������

	while (pNode != NULL) {
		if (pHead->data.x == pNode->data.x && pHead->data.y == pNode->data.y) {
			return 1;
		}

		pNode = pNode->pNext;
	}

	return 0;
}

void GameStart()
{
	Game	game;

	GameInit(&game);
	DisplayWall(game.width, game.height);
	DisplaySnake(&(game.snake));
	DisplayFood(game.foodPosition);

	while (1) {
		if (GetAsyncKeyState(VK_UP) && game.snake.direction != DOWN) {
			game.snake.direction = UP;
		}
		else if (GetAsyncKeyState(VK_DOWN) && game.snake.direction != UP) {
			game.snake.direction = DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT) && game.snake.direction != RIGHT) {
			game.snake.direction = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && game.snake.direction != LEFT) {
			game.snake.direction = RIGHT;
		}

		Position nextPos = GetNextPosition(&(game.snake));
		if (nextPos.x == game.foodPosition.x && nextPos.y == game.foodPosition.y) {
			// �Ե�ʳ��

			// ������һ��
			PushFront(&(game.snake), nextPos);

			// �ӷ�
			game.score += game.scorePerFood;

			// ������ʳ��
			FoodInit(&(game.foodPosition),
				game.width, game.height,
				&(game.snake));

			DisplayFood(game.foodPosition);

		}
		else {
			// û�Ե�ʳ��

			// ����ǰһ��
			PushFront(&(game.snake), nextPos);
			PopBack(&(game.snake));
		}

		if (IsCrashByWall(game.width, game.height, nextPos)) {
			break;
		}

		if (IsCrashHimself(&(game.snake))) {
			break;
		}

		Sleep(game.speed);
	}
}