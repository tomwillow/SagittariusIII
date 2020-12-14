#include "TButton.h"

using namespace std;

const TCHAR className[] = TEXT("button");

void TButton::Create(TWindow* pParent, UINT uId, std::tstring text)
{
	TWindow::Create(text.c_str(), className, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0,0,0,0,
		pParent, (HMENU)uId);// | SS_OWNERDRAW

	//m_hInst = hInst;
	//m_hWnd = CreateWindow(
	//	TEXT("button"),//类名，更改后按钮不显示
	//	text,
	//	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	//	0,
	//	0,
	//	0,
	//	0,
	//	hParent, (HMENU)uId,//id
	//	hInst,
	//	NULL);

	RegisterProc();
}