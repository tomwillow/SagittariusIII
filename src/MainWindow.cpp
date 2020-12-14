#include "MainWindow.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>

#include <WindowsX.h>

#include <stdexcept>

#include "TButton.h"

using namespace std;

void MainWindow::OnCreate(CREATESTRUCT* pCS)
{
	try
	{
		::GetClientRect(m_hWnd, &ClientRect);
		controller = make_unique<SceneController>(ClientRect.right,ClientRect.bottom);
	}
	catch (runtime_error e)
	{
		tstring s = tto_string(e.what());
		MessageBox(m_hWnd, s.c_str(), TEXT("Exception"), MB_ICONERROR);
		exit(-1);
	}

	this->RegisterMessage(WM_KEYDOWN, [&](TWindow* pCtrl, WPARAM wParam, LPARAM lParam)->LRESULT {
		controller->OnKeyDown(wParam, lParam);
		return 1;
		});

	this->RegisterMessage(WM_MOUSEMOVE, [&](TWindow* pCtrl, WPARAM wParam, LPARAM lParam)->LRESULT {
		controller->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 1;
		});

	this->RegisterMessage(WM_LBUTTONDOWN, [&](TWindow* pCtrl, WPARAM wParam, LPARAM lParam)->LRESULT {
		controller->OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 1;
		});

	this->RegisterMessage(WM_LBUTTONUP, [&](TWindow* pCtrl, WPARAM wParam, LPARAM lParam)->LRESULT {
		controller->OnLButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 1;
		});

	this->RegisterMessage(WM_CHAR, [&](TWindow* pCtrl, WPARAM wParam, LPARAM lParam)->LRESULT {
		controller->OnChar((TCHAR)wParam, lParam);
		return 1;
		});
}

bool MainWindow::OnDraw(HDC hdc)
{
	LineTo(hdc, 1000, 1000);
	return 0;
}



#ifdef _OPENGL
void MainWindow::Render()
{
	controller->Render(ClientRect.right,ClientRect.bottom);
	//SetText(std::tto_string(fps));
}
#endif