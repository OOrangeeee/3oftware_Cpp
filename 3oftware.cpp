#include"classes.h"
using namespace std;

int main()
{
	Solver* solve = new Solver();
	solve->init();
	for (int zhen = 1; zhen <= 15000; zhen++)
	{
		solve->everyInput();
		solve->action();
		puts("OK");
		fflush(stdout);
	}

	return 0;
}
