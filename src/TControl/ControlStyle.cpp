#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib,"comctl32.lib")//样式使用

bool InitControlStyle()
{
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = ICC_WIN95_CLASSES; // Or ICC_PROGRESS_CLASS
	return InitCommonControlsEx(&iccx);
}