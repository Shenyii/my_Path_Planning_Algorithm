#include "my_planner.h"

//int first_node = 181;                                        //test
//int second_node = 230;                                //test

my_planner::my_planner()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			map_[i][j].setPosition(j, i);
		}
	}
}


my_planner::~my_planner()
{
}

void my_planner::setObstacle(int x, int y, int length, int height)
{
	for (int i = y; i < y + height; i++)
	{
		for (int j = x; j < x + length; j++)
		{
			map_[i][j].setObstacle(true);
		}
	}

	setObstacleBrim();
}

void my_planner::setStartAndGoal(int x_start, int y_start, int x_goal, int y_goal)
{
	x_start_ = x_start;
	y_start_ = y_start;
	x_goal_ = x_goal;
	y_goal_ = y_goal;
}

int my_planner::startPathPlanning()
{
	path_.clear();
	double slope_of_line = (double)(y_goal_ - y_start_) / (double)(x_goal_ - x_start_);
	int det_distance = sqrt((y_goal_ - y_start_) * (y_goal_ - y_start_) + (x_goal_ - x_start_) * (x_goal_ - x_start_));
	path_.push_back(map_[y_start_][x_start_]);
	//for (int i = 0; i < abs(x_goal_ - x_start_); i++)
	//{
	//	int x = x_start_ + (x_goal_ - x_start_) / abs(x_goal_ - x_start_) * (i + 1);
	//	int y = slope_of_line * (x - x_start_) + y_start_;
	//	path_.push_back(map_[y][x]);
	//}
	for (int i = 0; i < det_distance;i++)
	{
		int det_x;
		int det_y;
		det_x = (double)(x_goal_ - x_start_) * (double)(i + 1) / (double)det_distance;
		det_y = (double)(y_goal_ - y_start_) * (double)(i + 1) / (double)det_distance;
		path_.push_back(map_[y_start_ + det_y][x_start_ + det_x]);
	}

	int begin_of_part_in_path_num = 0;
	int i = begin_of_part_in_path_num;
	int end_of_part_in_path_num = 0;
	while(1)
	{
		if (path_[i] == map_[y_goal_][x_goal_])
		{
			return 1;
		}

		if(path_[i].obstacleCheck() == 1)
		{
			return 0;
		}

		int brim_num = haveAddToBrimCheck(path_[i]) - 1;
		int brim_num2 = haveAddToBrimCheck2(path_[i]);
		if (brim_num != -1)
		{
			int extern_direction = rand() % 2 * 2 - 1;
			//extern_direction = -1;                                                                              //不产生随机方向，便于测试
			vector<node> part_of_path;
			part_of_path.push_back(obstacle_brim_[brim_num][brim_num2]);
			
			while (1)
			{
				brim_num2 = brim_num2 + extern_direction;
				int number1 = obstacle_brim_[brim_num].size();
				if (brim_num2 >= number1)
				{
					brim_num2 = 0;
				}
				if (brim_num2 < 0)
				{
					brim_num2 = obstacle_brim_[brim_num].size() - 1;
				}
				part_of_path.push_back(obstacle_brim_[brim_num][brim_num2]);

				int break_flag1 = 0;
				for (int j = i; j < path_.size(); j++)
				{
					if(obstacle_brim_[brim_num][brim_num2] == path_[j])
					{ 
						break_flag1 = 1;
						end_of_part_in_path_num = j;
					}
				}

				if (break_flag1 == 1)
				{
					path_.erase(path_.begin() + i, path_.begin() + end_of_part_in_path_num + 1);
					for (int k = 0; k < part_of_path.size(); k++)
					{
						path_.insert(path_.begin() + i + k,part_of_path[k]);
					}
					i = part_of_path.size() + i + 1;
					
					break;
				}
			}
		}
		i++;
	}
	return 1;
}

void my_planner::displayMap()
{
	//////////////////test
	//for (int i = 0; i < path_.size(); i++)
	//{
	//	cout << "(" << path_[i].getY() << "," << path_[i].getX() << ")" << " ";
	//	if ((i + 1) % 20 == 0)
	//	{
	//		cout << endl;
	//	}
	//}
	//cout << endl;
	///////////////////test
	cout << "   ";
	for (int i = 0; i < 100; i++)
	{
		cout << i % 10 << " ";
	}
	cout << endl;
	for (int i = 0; i < 100; i++)
	{
		cout << i % 10 << "  ";
		for (int j = 0; j < 100; j++)
		{
			/////////////////////test
			if ((i == path_[path_num_test_].getY()) && (j == path_[path_num_test_].getX()))
			{
				cout << "@ ";
				continue;
			}
			//if ((i == path_[second_node].getY()) && (j == path_[second_node].getX()))
			//{
			//	cout << "@ ";
			//	continue;
			//}
			/////////////////////test

			if (map_[i][j].obstacleCheck() == 1)
			{
				int map_state = 0;
				for (int k = 0; k < path_.size(); k++)
				{
					if (map_[i][j] == path_[k])
					{
						map_state = 1;
					}
				}
				if (map_state == 1)
				{
					cout << ">>";
					//Sleep(100);// getchar();                   //test
				}
				else
				{
					cout << "x ";
				}
			}
			else if (map_[i][j].obstacleCheck() == 0)
			{
				int map_state = 0;
				for (int k = 0; k < obstacle_brim_.size(); k++)
				{
					for (int l = 0; l < obstacle_brim_[k].size(); l++)
					{
						if (map_[i][j] == obstacle_brim_[k][l])
						{
							map_state = 2;
							break;
						}
					}
				}
				for (int k = 0; k < path_.size(); k++)
				{
					if (map_[i][j] == path_[k])
					{
						map_state = 1;
					}
				}
				if (map_state == 1)
				{
					cout << ">>";
					//Sleep(100);// getchar();                         //test
				}
				else if(map_state == 2)
				{
					cout << "0 ";
				}
				else
				{
					cout << ". ";
				}
			}
		}
		cout << endl;
	}
}

void my_planner::setObstacleBrim()
{
	obstacle_brim_.clear();
	for (int j = 0; j < 100; j++)
	{
		for(int i = 0;i < 100;i++)
		{
			if ((map_[j][i].obstacleCheck() == 0) && (obstacleBrimNodeCheck(map_[j][i])))
			{
				if(haveAddToBrimCheck(map_[j][i]) == 0)
				{ 
					addToBrim(map_[j][i]);
				}
			}
			else
			{
			}
		}
	}
}

int my_planner::obstacleBrimNodeCheck(node node_check)
{
	if ((node_check.getX() < 1) || (node_check.getX() > 98) || (node_check.getY() < 1) || (node_check.getY() > 98))
	{
		return 0;
	}

	if (node_check.obstacleCheck() == 1)
	{
		return 0;
	}

	if (map_[node_check.getY() - 1][node_check.getX() - 1].obstacleCheck())
	{
		return 1;
	}
	else if (map_[node_check.getY() - 1][node_check.getX()].obstacleCheck())
	{
		return 1;
	}
	else if (map_[node_check.getY() - 1][node_check.getX() + 1].obstacleCheck())
	{
		return 1;
	}
	else if (map_[node_check.getY()][node_check.getX() - 1].obstacleCheck())
	{
		return 1;
	}
	else if (map_[node_check.getY()][node_check.getX() + 1].obstacleCheck())
	{
		return 1;
	}
	else if (map_[node_check.getY() + 1][node_check.getX() - 1].obstacleCheck())
	{
		return 1;
	}
	else if (map_[node_check.getY() + 1][node_check.getX()].obstacleCheck())
	{
		return 1;
	}
	else if (map_[node_check.getY() + 1][node_check.getX() + 1].obstacleCheck())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int my_planner::haveAddToBrimCheck(node node_check)
{
	for (int i = 0; i < obstacle_brim_.size(); i++)
	{
		for (int j = 0; j < obstacle_brim_[i].size(); j++)
		{
			if(node_check == obstacle_brim_[i][j])
			{
				return 1 + i;
			}
		}
	}

	return 0;
}

int my_planner::haveAddToBrimCheck2(node node_check)
{
	for (int i = 0; i < obstacle_brim_.size(); i++)
	{
		for (int j = 0; j < obstacle_brim_[i].size(); j++)
		{
			if (node_check == obstacle_brim_[i][j])
			{
				return j;
			}
		}
	}

	return -1;
}

void my_planner::addToBrim(node brim_node)
{
	vector<node> one_obstacle_brim;
	one_obstacle_brim.push_back(brim_node);
	while (1)
	{
		int x = one_obstacle_brim[one_obstacle_brim.size() - 1].getX();
		int y = one_obstacle_brim[one_obstacle_brim.size() - 1].getY();
		if((one_obstacle_brim.size() > 5) && (one_obstacle_brim[0] == map_[y][x + 1]))
		{
			obstacle_brim_.push_back(one_obstacle_brim);
			break;
		}
		if ((one_obstacle_brim.size() > 5) && (one_obstacle_brim[0] == map_[y + 1][x]))
		{
			obstacle_brim_.push_back(one_obstacle_brim);
			break;
		}
		if ((one_obstacle_brim.size() > 5) && (one_obstacle_brim[0] == map_[y][x - 1]))
		{
			obstacle_brim_.push_back(one_obstacle_brim);
			break;
		}
		if ((one_obstacle_brim.size() > 5) && (one_obstacle_brim[0] == map_[y - 1][x]))
		{
			obstacle_brim_.push_back(one_obstacle_brim);
			break;
		}

		if (obstacleBrimNodeCheck(map_[y][x + 1]) == 1)
		{
			if(one_obstacle_brim.size() < 2 || !(map_[y][x + 1] == one_obstacle_brim[one_obstacle_brim.size() - 2]))
			{
				one_obstacle_brim.push_back(map_[y][x + 1]);
				continue;
			}
		}
		if (obstacleBrimNodeCheck(map_[y - 1][x]) == 1)
		{
			if (one_obstacle_brim.size() < 2 || !(map_[y - 1][x] == one_obstacle_brim[one_obstacle_brim.size() - 2]))
			{
				one_obstacle_brim.push_back(map_[y - 1][x]);
				continue;
			}
		}
		if (obstacleBrimNodeCheck(map_[y][x - 1]) == 1)
		{
			if (one_obstacle_brim.size() < 2 || !(map_[y][x - 1] == one_obstacle_brim[one_obstacle_brim.size() - 2]))
			{
				one_obstacle_brim.push_back(map_[y][x - 1]);
				continue;
			}
		}
		if (obstacleBrimNodeCheck(map_[y + 1][x]) == 1)
		{
			if (one_obstacle_brim.size() < 2 || !(map_[y + 1][x] == one_obstacle_brim[one_obstacle_brim.size() - 2]))
			{
				one_obstacle_brim.push_back(map_[y + 1][x]);
				continue;
			}
		}
	}
}

void my_planner::optimizePath()
{
	//cout << "the number of node in path is:" << path_.size() << endl;
	int first_node = 0;
	int second_node = 0;
	int swop_node;
	while (first_node == second_node)
	{
		first_node = rand() % path_.size();
		second_node = rand() % path_.size();
	}
	if (first_node > second_node)
	{
		swop_node = first_node;
		first_node = second_node;
		second_node = swop_node;
	}

	//first_node = 28;                                                                      //test
	//second_node = 110;                                                                 //test
	path_num_test_ = first_node;                                                   //test
	//cout << first_node << ", " << second_node << endl;

	vector<node> optimize_path;
	optimize_path.push_back(path_[first_node]);
	double slope_of_line = (double)(path_[second_node].getY() - path_[first_node].getY()) / (double)(path_[second_node].getX() - path_[first_node].getX());
	if (path_[second_node].getX() - path_[first_node].getX() == 0)
	{
		slope_of_line = (path_[second_node].getY() - path_[first_node].getY()) / fabs(path_[second_node].getY() - path_[first_node].getY()) * 10000.0;
	}
	//cout << slope_of_line << endl;
	int det_x0 = path_[second_node].getX() - path_[first_node].getX();
	int det_y0 = path_[second_node].getY() - path_[first_node].getY();
	for (int i = 0; i < sqrt(det_x0 * det_x0 + det_y0 * det_y0); i++)
	{
		int det_x;// = (path_[second_node].getX() - path_[first_node].getX()) / abs(path_[second_node].getX() - path_[first_node].getX()) * (i + 1);
		int det_y;// = slope_of_line * (det_x * 1.0);
		det_x = (double)(path_[second_node].getX() - path_[first_node].getX()) * double(i) / sqrt(det_x0 * det_x0 + det_y0 * det_y0);
		det_y = (double)(path_[second_node].getY() - path_[first_node].getY()) * double(i) / sqrt(det_x0 * det_x0 + det_y0 * det_y0);

		if (map_[path_[first_node].getY() + det_y][path_[first_node].getX() + det_x].obstacleCheck() == 1)
		{
			return;
		}
		else
		{
			optimize_path.push_back(map_[path_[first_node].getY() + det_y][path_[first_node].getX() + det_x]);
			//path_.push_back(map_[path_[first_node].getY() + det_y][path_[first_node].getX() + det_x]);
		}
	}

	path_.erase(path_.begin() + first_node, path_.begin() + second_node);
	for (int i = 0; i < optimize_path.size(); i++)
	{
		path_.insert(path_.begin() + first_node + i, optimize_path[i]);
	}
}
