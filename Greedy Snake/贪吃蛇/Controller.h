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

	int	score;	// 当前得分
	int scorePerFood;	// 每吃一个食物得分
i	nt speed;	//	蛇的前进速度
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

// 1 重叠
// 0 不重叠
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

// 生成食物
// 1. 随机
// 2. 范围内
// 3. 不和蛇重叠
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
	pGame->speed = 200; // 毫秒
}

// 得到下一步的坐标
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

// 头插
void PushFront(Snake *pSnake, Position nextPos)
{
	Node *pNewNode = (Node *)malloc(sizeof(Node));
	assert(pNewNode);
	pNewNode->data = nextPos;

	pNewNode->pNext = pSnake->pHead;
	pSnake->pHead = pNewNode;

	DisplaySnakeNode(nextPos);
}

// 尾删
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

// 1 表示撞了
// 0 表示没有
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

// 1 表示撞了
// 0 表示没撞
int IsCrashHimself(const Snake *pSnake)
{
	Node *pHead = pSnake->pHead;	// 第一个结点
	Node *pNode = pSnake->pHead->pNext;	// 第二个结点

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
			// 吃到食物

			// 蛇增长一格
			PushFront(&(game.snake), nextPos);

			// 加分
			game.score += game.scorePerFood;

			// 生成新食物
			FoodInit(&(game.foodPosition),
				game.width, game.height,
				&(game.snake));

			DisplayFood(game.foodPosition);

		}
		else {
			// 没吃到食物

			// 蛇向前一步
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