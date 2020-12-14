#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdexcept>
#include "tstring.h"

#include "resource.h"
#include "MainWindow.h"

using namespace std;

#ifdef _CONSOLE
int main()
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
#endif
{
	{
		InitControlStyle();

		MainWindow win;
		win.LoadTitleIcon(IDI_ICON1);
		win.Create(TEXT("THE DAY OF SAGITTARIUS III by Tom Willow"), TEXT("SagittariusIII"));

		win.MessageLoop();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}