#include "SceneMatch.h"

#include "gl_assist.h"

#include "SceneController.h"

using namespace std;

SceneMatch::SceneMatch(SceneController* controller, int w, int h) :
	Scene(controller), text(nullptr), msgBox(nullptr)
{
	starRender = make_unique<StarRender>(w, h);

	if (controller->ini.GetValue(TEXT("username")).empty())
		editDialog = make_unique<TGLEditDialog>(TEXT("请输入用户名："), TEXT(""), MB_OKCANCEL);
}

SceneMatch::SceneMatch(SceneController* controller, std::unique_ptr<StarRender>& lastStarRender) :
	Scene(controller), text(nullptr)
{
	starRender.reset(lastStarRender.release());

	if (controller->ini.GetValue(TEXT("username")).empty())
		editDialog = make_unique<TGLEditDialog>(TEXT("请输入用户名："), TEXT(""), MB_OKCANCEL);
}

void SceneMatch::OnChar(TCHAR tc, LPARAM lParam)
{
	if (editDialog)
		editDialog->OnChar(tc, lParam);
}

void SceneMatch::OnMouseMove(WPARAM mk_code, int x, int y)
{
	if (msgBox)
	{
		msgBox->OnMouseMove(mk_code, x, y);
		return;
	}
	if (editDialog)
		editDialog->OnMouseMove(mk_code, x, y);
}

void SceneMatch::OnLButtonDown(WPARAM mk_code, int x, int y)
{
	if (msgBox)
	{
		int key=msgBox->OnLButtonDown(mk_code, x, y);
		if (key == IDOK)
			msgBox = nullptr;
		return;
	}

	if (editDialog)
	{
		int key = editDialog->OnLButtonDown(mk_code, x, y);
		if (key == IDOK)
		{
			tstring username = editDialog->GetValue();
			if (username.empty())
			{
				msgBox= make_unique<TGLMessageBox>(TEXT("用户名不正确。"), TEXT(""), MB_OK);
				return;
			}
			if (username.find(TEXT(' ')) != tstring::npos)
			{
				msgBox = make_unique<TGLMessageBox>(TEXT("不能有空格。"), TEXT(""), MB_OK);
					return;
			}

			controller->ini.SetValue(TEXT("username"), username);
			controller->ini.SaveToFile();
			editDialog = nullptr;
			return;
		}
		if (key == IDCANCEL)
		{
			controller->GoCover(W, H);
			return;
		}
	}
}

void SceneMatch::Render(int w, int h)
{
	W = w; H = h;
	float t = (float)GetTickCount() / 1000.0f;
	glClearColor(17.0f / 255.0f, 25.0f / 255.0f, 36.0f / 255.0f, 0);//设置清屏时的颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏。这里清掉颜色缓冲区与深度缓冲区

	starRender->Draw(w, h, t);
	if (editDialog)
		editDialog->Draw(w, h);

	if (msgBox)
		msgBox->Draw(w, h);
}
