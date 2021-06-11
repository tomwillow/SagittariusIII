#include "SceneMatch.h"

#include "gl_assist.h"

#include "SceneController.h"

using namespace std;

SceneMatch::SceneMatch(SceneController* controller, int w, int h) :
	Scene(controller), text(nullptr), searchDialog(nullptr)
{
	starRender = make_unique<StarRender>(w, h);

	if (controller->ini.GetValue(TEXT("username")).empty())
		usernameDialog = make_unique<GLUsernameDialog>(controller);
	else
		searchDialog = make_unique<GLNetworkDialog>(controller);
}

SceneMatch::SceneMatch(SceneController* controller, std::unique_ptr<StarRender>& lastStarRender) :
	Scene(controller), text(nullptr), searchDialog(nullptr)
{
	starRender.reset(lastStarRender.release());

	if (controller->ini.GetValue(TEXT("username")).empty())
		usernameDialog = make_unique<GLUsernameDialog>(controller);
	else
		searchDialog = make_unique<GLNetworkDialog>(controller);
}

int SceneMatch::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_MOUSEMOVE)
	{
		int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
		starRender->SetViewPosByWindowCoord(W, H, x, y);
	}

	if (usernameDialog)
	{
		int key = usernameDialog->WndProc(uMsg, wParam, lParam);
		if (key == IDOK)
		{
			controller->ini.SetValue(TEXT("username"), usernameDialog->GetValue());
			controller->ini.SaveToFile();

			usernameDialog = nullptr;

			searchDialog = make_unique<GLNetworkDialog>(controller);
		}
		if (key == IDCANCEL)
		{
			controller->GoCover(W, H);
		}
		return 0;
	}

	if (searchDialog)
	{
		int key = searchDialog->WndProc(uMsg, wParam, lParam);
		if (key == IDBUILDROOM)//开设房间
		{
			controller->GoPrepRoom(W, H);
			return 0;
		}
		if (key == IDCANCEL)
		{
			controller->GoCover(W, H);
			return 0;
		}
	}

	return 0;
}

void SceneMatch::Render(int w, int h)
{
	W = w; H = h;
	float t = (float)GetTickCount() / 1000.0f;
	glClearColor(17.0f / 255.0f, 25.0f / 255.0f, 36.0f / 255.0f, 0);//设置清屏时的颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏。这里清掉颜色缓冲区与深度缓冲区

	starRender->Draw(w, h, t);

	if (usernameDialog)
		usernameDialog->Draw(w, h);

	if (searchDialog)
		searchDialog->Draw(w, h);
}
