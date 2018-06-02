
#include "DebugApplication.h"

int main()
{
	DebugApplication App;

	if (App.Run(1280, 720, "Debug Application"))
		return -1;

	return 0;
}