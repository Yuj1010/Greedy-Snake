#pragma once

// 坐标原点位于左上角，
// 向右为 x 轴的正方向，
// 向下为 y 轴的正方向，
typedef struct Position {
	int	x;
	int y;
}	Position;

// 链表结点（单向）
typedef struct Node {
	Position	data;
	struct Node *pNext;
}	Node;


// 表示蛇当前的前进方向
typedef enum Direction {
	UP, DOWN, LEFT, RIGHT
}	Direction;

// 蛇的结构
typedef struct Snake {
	Direction direction;
	Node *pHead;
}	Snake;

// 一盘游戏中用到的其他非独立字段
typedef struct Game {
	Snake		snake;
	Position	foodPosition;

	int	score;	// 当前得分
	int scorePerFood;	// 每吃一个食物得分
	int speed;	//	蛇的前进速度
	int width;	// 宽
	int height;	// 高
}	Game;