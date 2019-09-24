#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <Windows.h>
#include "node.h"

using namespace std;

class my_planner
{
public:
	my_planner();
	~my_planner();
	void setObstacle(int x,int y,int length,int height);
	void setStartAndGoal(int x_start, int y_start, int x_goal, int y_goal);
	int startPathPlanning();
	void optimizePath();
	void displayMap();

private:
	node map_[100][100];
	vector<node> path_;
	vector<vector<node>> obstacle_brim_;
	int x_start_;
	int y_start_;
	int x_goal_;
	int y_goal_;
	int path_num_test_;

	void setObstacleBrim();
	int obstacleBrimNodeCheck(node node_check);
	int haveAddToBrimCheck(node node_check);
	int haveAddToBrimCheck2(node node_check);
	void addToBrim(node brim_node);
};

