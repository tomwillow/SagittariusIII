#pragma once
#include "TWindow.h"
class TControl :
    public TWindow
{
private:
	static LRESULT CALLBACK SubControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	void RegisterProc();//创建窗口后注册
};

