#include "SceneCover.h"

#include "gl_assist.h"

#include "TBox.h"
#include "SceneController.h"

#include <random>

using namespace std;

SceneCover::SceneCover(SceneController* controller) :
	Scene(controller),
	tex1(make_unique<TFixedSpirit>(IMG_DAY, 0.3, 0.85, 0.4, true)),
	tex2(make_unique<TFixedSpirit>(IMG_SAGITTARIUS, 0.5, 0.75, 0.6, true)),
	tex3(make_unique<TFixedSpirit>(IMG_III, 0.5, 0.6, 0.15, true)),
	round_rect(make_unique<TFixedSpirit>(IMG_ROUNDRECT, 0.5, 0.3, 0.14, true)),
	copyright(make_unique<TFixedSpirit>(IMG_COPYRIGHT, 0.5, 0.1, 0.1, true)),
	textStart(make_unique<TFixedFreeType>(TEXT("START"), FONT_ENG, 0.5, 0.4, 0.10, true)),
	textLoad(make_unique<TFixedFreeType>(TEXT("LOAD"), FONT_ENG, 0.5, 0.3, 0.08, true)),
	textExit(make_unique<TFixedFreeType>(TEXT("EXIT"), FONT_ENG, 0.5, 0.2, 0.08, true)),
	text(make_unique<TFreeType>(TEXT("朝比奈みくる"), FONT_CJK))
{
}

void SceneCover::Start(int w, int h)
{
	W = w, H = h;

	starRender = make_unique<StarRender>(w, h);
}

int SceneCover::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	{
		int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
		if (textStart->PosInTexture(x, y, W, H))
		{
			controller->PlaySoundEffect();
			controller->GoMatch(W, H);
			return 0;
		}
		if (textExit->PosInTexture(x, y, W, H))
		{
			controller->PlaySoundEffect();
			exit(0);
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
		hoverStart = textStart->PosInTexture(x, y, W, H);
		hoverLoad = textLoad->PosInTexture(x, y, W, H);
		hoverExit = textExit->PosInTexture(x, y, W, H);

		starRender->SetViewPosByWindowCoord(W, H, x, y);
		break;
	}
	}
	return 0;
}


void SceneCover::Render(int w, int h)
{
	static float t0 = (float)GetTickCount() / 1000.0f;
	float t = (float)GetTickCount() / 1000.0f;

	W = w, H = h;
	glClearColor(17.0f / 255.0f, 25.0f / 255.0f, 36.0f / 255.0f, 0);//设置清屏时的颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏。这里清掉颜色缓冲区与深度缓冲区

	starRender->Draw(w, h, t);

	//渲染各种标题

	EnableTexture();

	//
	glColor4f(1, 1, 1, 1);
	tex1->Draw(w, h);
	tex2->Draw(w, h);
	tex3->Draw(w, h);
	round_rect->Draw(w, h);
	copyright->Draw(w, h);

	//
	if (hoverStart)
		glColor4f(0.9, 0.9, 0, 1);
	else
		glColor4f(0.9, 0.9, 1, 0.8);
	textStart->Draw(w, h);

	if (hoverLoad)
		glColor4f(0.9, 0.9, 0, 1);
	else
		glColor4f(0.9, 0.9, 1, 0.8);
	textLoad->Draw(w, h);

	if (hoverExit)
		glColor4f(0.9, 0.9, 0, 1);
	else
		glColor4f(0.9, 0.9, 1, 0.8);
	textExit->Draw(w, h);

	//标题渲染结束
}