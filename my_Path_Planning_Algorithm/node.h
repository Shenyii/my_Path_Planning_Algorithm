#pragma once
#include <iostream>

using namespace std;

class node
{
public:
	node();
	~node();
	bool obstacleCheck();
	void setObstacle(bool state);
	void setPosition(int x, int y);
	int getX();
	int getY();

private:
	int x_;
	int y_;
	bool obstacle_flag_;
	node* previous_node_;

	
};

bool operator==(node node1,node node2);

