锘?include "stdlib.h"
#include "math.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <queue>

#ifndef PUZZLE_H_
#define PUZZLE_H_

using namespace std;

const int MY_INFINITE = 10000;
const int MAX_PATH_LENGTH = 100;

typedef struct PuzzleNode
{
	int f;
	int h;
	int g;
	int* state;		//褰撳墠鐘舵€?	char *path;		//褰撳墠宸茶蛋杩囩殑璺緞
	PuzzleNode* next;
}PuzzleNode;

class Puzzle
{
public:
	PuzzleNode *start_node_;	//绗竴涓妭鐐?	PuzzleNode *current_node_;
//	int* state;
	int length_;
	
	int num_;
	int num_rows_;
	int *start_state_;
	int *goal_state_;
	char *path_;

	Puzzle();
	Puzzle(const int num, const int initial_distance);
	~Puzzle();

	void setState(const int *state);
	int *readState();
	queue<char> readPath();

private:
	void initPuzzle(const int d);
	void idas();
	void rbfs();
	char *loopRBFS(PuzzleNode* current, const int limit, int &new_f);

	bool belongToBorder(const int index, const char border);
	char lastAction(const char *path);
	bool hasArrived(const int *a);
	int distance(const int *a);
	void addChildren(PuzzleNode *node);
	void deleteNode(PuzzleNode *&node, PuzzleNode *pre_node);
	int getIndex(const int *a, const int value);
	void swap(int *a, const int index_1, const int index_2);
	void swap(const int *src, int *dst, const int index_1, const int index_2);
	PuzzleNode *cloneNode(const PuzzleNode *node);
	PuzzleNode *findTailNode();
	int *cloneState(const int *state);
};

#endif /* PUZZLE_H_ */
