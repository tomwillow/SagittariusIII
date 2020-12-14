#include "TControl.h"


LRESULT CALLBACK TControl::SubControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TWindow* pControl = nullptr;
#ifdef _WIN64
	pControl = (TControl*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
#else
	pControl = (TWindow*)GetWindowLong(hWnd, GWL_USERDATA);
#endif

	WNDPROC oldProc = (WNDPROC)GetProp(hWnd, TEXT("OldProc"));
	if (pControl)
	{
		//if (uMsg == WM_DROPFILES)
		//{
		//	vector<tstring> dropFiles = pControl->PreDrop(wParam);
		//	return pControl->DropProc(dropFiles);
		//}
		//else
			return pControl->WndProc(oldProc, hWnd, uMsg, wParam, lParam);
	}
	else
		return CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
}

void TControl::RegisterProc()//创建窗口后注册
{
#ifdef _WIN64
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	WNDPROC oldProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, SubControlProc);
#else
	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
	WNDPROC oldProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWL_WNDPROC, (LONG_PTR)SubControlProc);
#endif

	SetProp(m_hWnd, TEXT("OldProc"), oldProc);
}