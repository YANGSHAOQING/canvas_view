#include "stdafx.h"
#include "console_helper.h"

#include <windows.h>
#include <cstdio>

ConsoleHelper::ConsoleHelper(TCHAR* titile)
{
	AllocConsole();
	SetConsoleTitle(titile);
	freopen("conin$", "r+t", stdin);
	freopen("conout$", "w+t", stdout);
	freopen("conout$", "w+t", stderr);
}


ConsoleHelper::~ConsoleHelper()
{
	fclose(stderr);
	fclose(stdout);
	fclose(stdin);
	FreeConsole();
}
