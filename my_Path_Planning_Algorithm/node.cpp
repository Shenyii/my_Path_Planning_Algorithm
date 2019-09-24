#include "node.h"

node::node()
	:obstacle_flag_(false),previous_node_(nullptr)
{
}


node::~node()
{
}

bool node::obstacleCheck()
{
	return obstacle_flag_;
}

void node::setObstacle(bool state)
{
	obstacle_flag_ = state;
}

void node::setPosition(int x, int y)
{
	x_ = x;
	y_ = y;
}

int node::getX()
{
	return x_;
}

int node::getY()
{
	return y_;
}

bool operator==(node node1,node node2)
{
	if ((node2.getX() == node1.getX()) && (node2.getY() == node1.getY()))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
