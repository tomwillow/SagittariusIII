#include "ScenePrepRoom.h"

#include "gl_assist.h"

#include "SceneController.h"

#include "ColorConfig.h"

using namespace std;

ScenePrepRoom::ScenePrepRoom(SceneController* controller, int w, int h):
	Scene(controller),W(w),H(h),
	title(make_unique<TBoxFreeType>(TEXT("多人联机游戏准备室"), FONT_CJK, text_pixel)),
	name(make_unique<TBoxFreeType>(TEXT("名称"), FONT_CJK, text_pixel)),
	color(make_unique<TBoxFreeType>(TEXT("颜色"), FONT_CJK, text_pixel)),
	team(make_unique<TBoxFreeType>(TEXT("组队"), FONT_CJK, text_pixel)),
	preinstall_title(make_unique<TBoxFreeType>(TEXT("预设战役"), FONT_CJK, text_pixel))
{
	starRender = make_unique<StarRender>(w, h);

	//
	buttons.emplace_back(TEXT("开始游戏"), FONT_CJK, text_pixel);
	buttons.emplace_back(TEXT("取消"), FONT_CJK, text_pixel);

	//
	players.emplace_back(controller->ini.GetValue(TEXT("username")), glm::vec4(1, 1, 1, 1), 0,true);
	players[0].team_btn.SetEnable();

	//
	name->SetFontSizeScale(0.5);
	color->SetFontSizeScale(0.5);
	team->SetFontSizeScale(0.5);

	//
	preinstall_title->SetFontSizeScale(0.8);

	preinstall_buttons.emplace_back(TEXT("SOS团VS计研社"), FONT_CJK, text_pixel);
	preinstall_buttons[0].SetFontSizeScale(0.5);
}

ScenePrepRoom::ScenePrepRoom(SceneController* controller, std::unique_ptr<StarRender>& lastStarRender):
	Scene(controller)
{
	starRender.reset(lastStarRender.release());
}

int ScenePrepRoom::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
	{
		for (auto& btn : preinstall_buttons)
			btn.OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		for (auto& btn : buttons)
			btn.OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		for (auto& player : players)
			player.team_btn.OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		color_combo.OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int key = 0;
		if (color_combo.OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
		{
			controller->PlaySoundEffect();
			players[0].color = color_combo.GetColor();
		}

		for (auto& player : players)
		{
			if (player.team_btn.OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
			{
				controller->PlaySoundEffect();
				player.team = player.team_btn.GetTeam();
			}
		}

		key = preinstall_buttons[0].OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (key)//战役1
		{
			controller->PlaySoundEffect();

			players.clear();
			players.emplace_back(TEXT("キョン艦隊"), glm::vec4(0.176f, 0.341f, 0.984f, 1), 1, false);
			players.emplace_back(TEXT("ハルヒ☆閣下☆艦隊"), glm::vec4(1.0f, 0.027f, 0.039f, 1), 1, true);
			players.emplace_back(TEXT("古泉くん艦隊"), glm::vec4(0.137f, 0.608f, 0.353f, 1), 1, false);
			players.emplace_back(TEXT("みくるちゃん艦隊"), glm::vec4(0.984f, 0.282f, 0.6f, 1), 1, false);
			players.emplace_back(TEXT("ユキ艦隊"), glm::vec4(0.765f, 0.631f, 0.808f, 1), 1, false);
			players.emplace_back(TEXT("DIE"), glm::vec4(0.016f, 0.067f, 0.0f, 1), 2, true);
			players.emplace_back(TEXT("LUP"), glm::vec4(0.596f, 0.333f, 0.545f, 1), 2, false); //red
			players.emplace_back(TEXT("IRA"), glm::vec4(0.816f, 0.824f, 0.475f, 1), 2, false);//yellow
			players.emplace_back(TEXT("MUS"), glm::vec4(0.376f, 0.533f, 0.624f, 1), 2, false);//blue
			players.emplace_back(TEXT("EQU"), glm::vec4(0.627f, 0.408f, 0.573f, 1), 2, false); //purple

			players[0].team_btn.SetEnable(true);
		}
		//key = buttons[0].OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));//开始游戏
		//if (key)
		//{
		//	ipDialog = make_unique<TGLEditDialog>(TEXT("请输入服务器IP："), TEXT(""), MB_OKCANCEL);
		//}

		key = buttons[1].OnLButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));//取消
		if (key)
		{
			controller->PlaySoundEffect();
			controller->GoCover(W, H);
			return 0;
		}

		break;
	}
	case WM_RBUTTONDOWN:
	{
		int key = 0;

		for (auto& player : players)
		{
			if (player.team_btn.OnRButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
			{
				controller->PlaySoundEffect();
				player.team = player.team_btn.GetTeam();
			}
		}
		break;
	}
	}
	return 0;
}

void ScenePrepRoom::DrawPreinstall(float w,float h,float x1,float y1,float x2,float y2)
{

	EnableGeometry();
	ApplyEditBackground();
	DrawRect(GL_QUADS, x1, y1, x2, y2);
	ApplyEditBorder();
	DrawRect(GL_LINE_LOOP, x1, y1, x2, y2);

	float title_left = x1 + x_margin;
	float title_right = x2 - x_margin;
	float title_top = y2 - top_margin;
	float title_bottom = title_top - preinstall_title->GetClipCoordHeight(h);
		EnableTexture();
	glColor4f(1, 1, 1, 1);
	preinstall_title->DrawCenterByClipCoord(w, h, title_left, title_bottom, title_right, title_top);

	float btn_y2 = title_bottom - top_margin;
	for (auto& btn : preinstall_buttons)
	{
		float btn_y1 = btn_y2 - btn.GetClipCoordHeight(h);
		btn.DrawByClipCoord(w, h,title_left,btn_y1,title_right,btn_y2);
		btn_y2 = btn_y1 - top_margin;
	}
}

void ScenePrepRoom::Render(int w, int h)
{
	W = w; H = h;
	float t = (float)GetTickCount() / 1000.0f;
	glClearColor(17.0f / 255.0f, 25.0f / 255.0f, 36.0f / 255.0f, 0);//设置清屏时的颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏。这里清掉颜色缓冲区与深度缓冲区

	//background
	starRender->Draw(w, h, t);

	//title
	float x1 = -1.0f, x2 = 1.0f, y1 = -1.0f, y2 = 1.0f;
	text_height = title->GetClipCoordHeight(h);

	EnableTexture();
	glColor4f(1, 1, 1, 1);
	float title_bottom = y2 - top_margin - text_height;
	title->DrawCenterByClipCoord(w, h, x1 + x_margin, title_bottom, x2 - x_margin, y2 - top_margin);

	float y_interval = 0.08f;

	//buttons
	float button_interval = 0.08f;
	float button_left = x1 + x_margin;
	float button_right = x2 - x_margin;
	float button_height = text_height * 1.1f;
	float button_bottom = y1 + bottom_margin;
	float button_top = button_bottom + button_height;

	float button_width = (button_right - button_left - (button_interval * ((float)buttons.size() - 1.0f))) / buttons.size();
	float button_x = button_left;
	for (auto& btn : buttons)
	{
		btn.DrawByClipCoord(w, h, button_x, button_bottom, button_x + button_width, button_top);
		button_x += button_width + button_interval;
	}

	//preinstall
	float preinstall_width = 0.5f;
	float preinstall_x_interval = 0.1f;

	float preinstall_right = x2 - x_margin;
	float preinstall_left = preinstall_right - preinstall_width;
	float preinstall_top = title_bottom - y_interval;
	float preinstall_bottom = button_top + y_interval;

	DrawPreinstall(w, h, preinstall_left, preinstall_bottom, preinstall_right, preinstall_top);

	//list
	float list_left = x1 + x_margin;
	float list_right = preinstall_left-preinstall_x_interval;
	float list_top = title_bottom - y_interval;
	float list_bottom = button_top + y_interval;

	//list header
	float small_text_height = name->GetClipCoordHeight(H);
	float header_bottom = list_top - small_text_height;
	float list_width = list_right - list_left;
	float name_percent = 0.5f;
	float color_percent = 0.2f;
	float team_percent = 0.3f;

	//
	EnableTexture();
	glColor4f(1, 1, 1, 1);
	float x1_pos = list_left;
	name->DrawCenterByClipCoord(w, h, x1_pos, header_bottom, x1_pos + list_width * name_percent, list_top);
	x1_pos += list_width * name_percent;
	color->DrawCenterByClipCoord(w, h, x1_pos, header_bottom, x1_pos + list_width * color_percent, list_top);
	x1_pos += list_width * color_percent;
	team->DrawCenterByClipCoord(w, h, x1_pos, header_bottom, x1_pos + list_width * team_percent, list_top);

	//list item
	float color_combo_x1;
	float color_combo_y1;
	float color_combo_x2;
	float color_combo_y2;

	float item_y_interval = 0.01f;
	float y1_pos = header_bottom - y_interval - small_text_height;
	for (int i=0;i<10;++i)
	{
		if (i < players.size())
		{
			auto& player = players[i];

			//name
			glColor4f(player.color.r, player.color.g, player.color.b, player.color.a);
			float x1_pos = list_left;
			float x2_pos = x1_pos + list_width * name_percent;

			EnableTexture();
			player.freetype->DrawCenterByClipCoord(w, h, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);
			x1_pos = x2_pos;
			x2_pos += list_width * color_percent;

			//color
			if (i == 0)
			{
				color_combo_x1 = x1_pos;
				color_combo_y1 = y1_pos;
				color_combo_x2 = x2_pos;
				color_combo_y2 = y1_pos + small_text_height;
			}
			else
			{
				EnableGeometry();
				glColor4f(player.color.r, player.color.g, player.color.b, player.color.a);
				DrawRect(GL_QUADS, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);
				ApplyEditBorder();
				DrawRect(GL_LINE_LOOP, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);
			}

			x1_pos = x2_pos;
			x2_pos += list_width * team_percent;

			//
			player.team_btn.DrawByClipCoord(w, h, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);

			//EnableGeometry();
			//ApplyEditBackground();
			//DrawRect(GL_QUADS, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);
			//ApplyEditBorder();
			//DrawRect(GL_LINE_LOOP, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);
			//EnableTexture();
			//player.freetype->DrawCenterByClipCoord(w, h, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);
		}
		else
		{
			float x1_pos = list_left;
			float x2_pos = x1_pos+list_width * name_percent;
			EnableGeometry();

			//name
			ApplyEditBackground();
			DrawRect(GL_QUADS, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);
			ApplyEditBorder();
			DrawRect(GL_LINE_LOOP, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);

			x1_pos = x2_pos;
			x2_pos+= list_width *color_percent;
			//color
			ApplyEditBackground();
			DrawRect(GL_QUADS, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);
			ApplyEditBorder();
			DrawRect(GL_LINE_LOOP, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);

			x1_pos = x2_pos;
			x2_pos += list_width * team_percent;
			//team
			ApplyEditBackground();
			DrawRect(GL_QUADS, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);
			ApplyEditBorder();
			DrawRect(GL_LINE_LOOP, x1_pos, y1_pos, x2_pos, y1_pos + small_text_height);
		}
		y1_pos -= small_text_height+item_y_interval;
	}


	color_combo.DrawByClipCoord(w, h, color_combo_x1, color_combo_y1, color_combo_x2, color_combo_y2);
}
