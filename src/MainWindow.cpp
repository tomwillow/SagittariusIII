#include "MainWindow.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>

#include <stdexcept>

#include "TButton.h"

using namespace std;

void MainWindow::OnCreate(CREATESTRUCT* pCS)
{
	try
	{
		//初始化controller
		::GetClientRect(m_hWnd, &ClientRect);
		controller = make_unique<SceneController>(ClientRect.right,ClientRect.bottom);
	}
	catch (runtime_error e)
	{
		tstring s = tto_string(e.what());
		MessageBox(m_hWnd, s.c_str(), TEXT("Exception"), MB_ICONERROR);
		exit(-1);
	}

	//将消息转发至controller
	for (auto msg : controller->receivedMsg)
	{
		this->RegisterMessage(msg, [&](TWindow* pCtrl,UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT {
			return controller->WndProc(uMsg,wParam,lParam);
			});
	}
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
	SetText(std::tto_string(fps));
}
#endif