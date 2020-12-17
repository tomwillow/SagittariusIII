#pragma once
#include "Scene.h"

#include "StarRender.h"

#include "TBoxFreeType.h"
#include "TGLButton.h"
#include "GLTeamButton.h"
#include "TGLColorCombo.h"

struct Player
{
    std::tstring name;
    bool king;
    glm::vec4 color;
    int team;
    std::unique_ptr<TBoxFreeType> freetype;
    GLTeamButton team_btn;
    Player(std::tstring in_name, glm::vec4 in_color, int in_team,bool in_king) :
        name(in_name), color(in_color), team(in_team),king(in_king),
        freetype(std::make_unique<TBoxFreeType>(in_name, FONT_CJK, 48)),
        team_btn(in_team,FONT_CJK,48)
    {
        freetype->SetFontSizeScale(0.5);
        team_btn.SetFontSizeScale(0.5);
       team_btn.SetEnable(false);
    }
};

class ScenePrepRoom :
    public Scene
{
private:
    int W, H;
    std::unique_ptr<StarRender> starRender;

    const float x_margin = 0.08f;
    const float top_margin = 0.15f;
    const float bottom_margin = 0.2f;
    const int text_pixel = 48;
    float text_height;

    std::unique_ptr<TBoxFreeType> title,name,color,team,preinstall_title;
    std::vector<TGLButton> buttons,preinstall_buttons;

    std::vector<Player> players;
    TGLColorCombo color_combo;

    void DrawPreinstall(float w, float h, float x1, float y1, float x2, float y2);
public:
    //�½�һ��StarRender
    ScenePrepRoom(SceneController* controller, int w, int h);

    //�������StarRenderת������Ȩ
    ScenePrepRoom(SceneController* controller, std::unique_ptr<StarRender>& lastStarRender);

    virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void Render(int w, int h)override;
};
