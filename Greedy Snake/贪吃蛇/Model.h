#pragma once

// ����ԭ��λ�����Ͻǣ�
// ����Ϊ x ���������
// ����Ϊ y ���������
typedef struct Position {
	int	x;
	int y;
}	Position;

// �����㣨����
typedef struct Node {
	Position	data;
	struct Node *pNext;
}	Node;


// ��ʾ�ߵ�ǰ��ǰ������
typedef enum Direction {
	UP, DOWN, LEFT, RIGHT
}	Direction;

// �ߵĽṹ
typedef struct Snake {
	Direction direction;
	Node *pHead;
}	Snake;

// һ����Ϸ���õ��������Ƕ����ֶ�
typedef struct Game {
	Snake		snake;
	Position	foodPosition;

	int	score;	// ��ǰ�÷�
	int scorePerFood;	// ÿ��һ��ʳ��÷�
	int speed;	//	�ߵ�ǰ���ٶ�
	int width;	// ��
	int height;	// ��
}	Game;