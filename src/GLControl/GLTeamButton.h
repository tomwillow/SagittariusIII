#pragma once
#include "TGLButton.h"
class GLTeamButton :
    public TGLButton
{
private:
	int team;
	std::tstring team2str(int in_team);
public:
	GLTeamButton(int in_team,std::string font_name, int pixel_size = 48);
	virtual int OnLButtonDown(WPARAM mk_code, int x, int y) override;
	virtual int OnRButtonDown(WPARAM mk_code, int x, int y) override;

	int GetTeam();
	virtual void DrawByClipCoord(int w, int h, float x1, float y1, float x2, float y2) override;
};

