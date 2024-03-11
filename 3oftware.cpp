#include <bits/stdc++.h>
#include"classes.h"
using namespace std;

int main()
{
	Solver* solve = new Solver();
	solve->init();
	for (int zhen = 1; zhen <= 15000; zhen++)
	{
		solve->everyInput();
		for (int i = 0; i < robot_num; i++)
			printf("move %d %d\n", i, rand() % 4);
		puts("OK");
		fflush(stdout);
	}

	return 0;
}
