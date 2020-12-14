#include "SceneMatch.h"

#include "gl_assist.h"

#include "SceneController.h"

using namespace std;

SceneMatch::SceneMatch(SceneController* controller, int w, int h):
	Scene(controller),text(nullptr),dialog(make_unique<TGLEditDialog>(TEXT("请输入名称："),TEXT(""),MB_OKCANCEL,1.0f,1.0f))
{
	starRender = make_unique<StarRender>(w, h);
}

SceneMatch::SceneMatch(SceneController* controller, std::unique_ptr<StarRender>& lastStarRender):
	Scene(controller), text(nullptr), dialog(make_unique<TGLEditDialog>(TEXT("请输入名称："), TEXT(""), MB_OKCANCEL, 1.0f, 1.0f))
{
	starRender.reset(lastStarRender.release());
}

void SceneMatch::OnChar(TCHAR tc, LPARAM lParam)
{
	dialog->OnChar(tc, lParam);
}

void SceneMatch::OnMouseMove(WPARAM mk_code, int x, int y)
{
	dialog->OnMouseMove(mk_code, x, y);
}

void SceneMatch::OnLButtonDown(WPARAM mk_code, int x, int y)
{
	int key=dialog->OnLButtonDown(mk_code, x, y);
	if (key == IDCANCEL)
	{
		controller->GoCover(W,H);
	}
}

void SceneMatch::Render(int w, int h)
{
	W = w; H = h;
	float t = (float)GetTickCount() / 1000.0f;
	glClearColor(17.0f / 255.0f, 25.0f / 255.0f, 36.0f / 255.0f, 0);//设置清屏时的颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏。这里清掉颜色缓冲区与深度缓冲区

	starRender->Draw(w, h, t);

	dialog->Draw(w, h);

}
