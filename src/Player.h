#pragma once

#include "tstring.h"
#include <memory>

#include "TBoxFreeType.h"
#include "TGLButton.h"
#include "GLTeamButton.h"
#include "TGLColorCombo.h"

#include "ExternResource.h"

struct Player
{
    std::tstring name;
    int ai;
    bool king;
    int color;
    int team;
    std::unique_ptr<TBoxFreeType> freetype;
    GLTeamButton team_btn;
    TGLColorCombo color_combo;

    float x, y;
    float speed;
    int angle;//µ¥Î»Îªdegree
    Player(std::tstring in_name, int in_color, int in_team, bool in_king, int in_ai) :
        name(in_name), color(in_color), team(in_team), king(in_king), ai(in_ai),
        freetype(std::make_unique<TBoxFreeType>(in_name, FONT_CJK, 48)),
        team_btn(in_team, FONT_CJK, 48),
        angle(0),speed(0),x(0),y(0)
    {
        freetype->SetFontSizeScale(0.5);
        team_btn.SetFontSizeScale(0.5);
        team_btn.SetEnable(false);
        color_combo.SetCur(color);
    }
};
