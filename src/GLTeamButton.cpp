#include "GLTeamButton.h"

using namespace std;

std::tstring GLTeamButton::team2str(int in_team)
{
	if (in_team == 0)
		return TEXT("-");
	else
		return tto_string(in_team);
}

GLTeamButton::GLTeamButton(int in_team, std::string font_name, int pixel_size):
	team(in_team),
	TGLButton(team2str(in_team),font_name,pixel_size)
{
}

int GLTeamButton::OnLButtonDown(WPARAM mk_code, int x, int y)
{
	if (!enable) return 0;
	if (this->TGLButton::OnLButtonDown(mk_code, x, y))
	{
		team++;
		if (team > 4)
			team = 0;
		float i = text->GetFontSizeScale();
		text = make_unique<TBoxFreeType>(team2str(team), font_name, pixel_size);
		text->SetFontSizeScale(i);
		return 1;
	}
	return 0;
}

int GLTeamButton::OnRButtonDown(WPARAM mk_code, int x, int y)
{
	if (!enable) return 0;
	if (this->TGLButton::OnRButtonDown(mk_code, x, y))
	{
		team--;
		if (team <0 )
			team = 4;
		float i = text->GetFontSizeScale();
		text = make_unique<TBoxFreeType>(team2str(team), font_name, pixel_size);
		text->SetFontSizeScale(i);
		return 1;
	}
	return 0;
}

int GLTeamButton::GetTeam()
{
	return team;
}

void GLTeamButton::DrawByClipCoord(int w, int h, float x1, float y1, float x2, float y2)
{
	float height = text->GetClipCoordHeight(h);
	float cx = (x1 + x2) / 2.0f;
	this->TGLButton::DrawByClipCoord(w, h, cx - height / 2.0f, y1, cx + height / 2.0f, y2);
}
