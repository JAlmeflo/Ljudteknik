#include "System.h"

int main(void)
{
	System system = System();

	if (system.Initialize())
	{
		system.Run();
	}

	system.Shutdown();

	return 0;
}