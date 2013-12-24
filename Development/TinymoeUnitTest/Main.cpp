#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "UnitTest.h"

int wmain(int argc , wchar_t* args[])
{
	_CrtDumpMemoryLeaks();
	return 0;
}