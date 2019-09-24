#include <iostream>
#include "my_planner.h"

using namespace std;

int main()
{
	srand((unsigned)time(NULL));
	cout << "hellow world!!!" << endl;
	my_planner my_planner_test;
	my_planner_test.setObstacle(25, 25, 6, 4);
	//my_planner_test.setObstacle(50, 40, 30,5);
	my_planner_test.setObstacle(60, 30, 6, 50);
	my_planner_test.setObstacle(30, 70, 20, 10);
	my_planner_test.setObstacle(80, 70, 2, 15);
	my_planner_test.setStartAndGoal(20, 20, 95, 50);
	my_planner_test.startPathPlanning();
	my_planner_test.displayMap();
	getchar();
	while (1)
	{
		for (int i = 0; i < 100; i++)
		{
			my_planner_test.optimizePath();
		}
		my_planner_test.displayMap();
		getchar();
		cout << endl;
	}
	
	return 1;
}