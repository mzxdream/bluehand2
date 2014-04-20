#include "BhMaster.hpp"
#include "BhGateModule.hpp"
#include <stdio.h>

#define BHGATEMODULE 1

int main(int argc, char **argv)
{
	BhGateModule gate(2, 100);
	if (gate.Init() != 0)
	{
		return -1;
	}
	BhMaster master;
	if (master.Init() != 0)
	{
		return -2;
	}
	if (master.AddModule(BHGATEMODULE, &gate) != 0)
	{
		return -3;
	}
	if (master.Start() != 0)
	{
		return -4;
	}
	while (true)
	{
		if (getchar() == 'q')
		{
			master.Stop();
			master.Join();
		}
	}
	gate.Clear();
	return 0;
}
