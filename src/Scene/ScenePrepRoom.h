#pragma once
#include "Scene.h"

#include "StarRender.h"

#include "TBoxFreeType.h"
#include "TGLButton.h"
#include "GLTeamButton.h"
#include "TGLColorCombo.h"

#include "Player.h"


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

    std::unique_ptr<TBoxFreeType> title,name,color,team,king,preinstall_title;
    std::vector<TGLButton> buttons,preinstall_buttons;

    void DrawPreinstall(float w, float h, float x1, float y1, float x2, float y2);
    void DrawList(float w, float h, float x1, float y1, float x2, float y2);
public:
    //新建一个StarRender
    ScenePrepRoom(SceneController* controller, int w, int h);

    //将传入的StarRender转移所有权
    ScenePrepRoom(SceneController* controller, std::unique_ptr<StarRender>& lastStarRender);

    virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    void Render(int w, int h)override;
};

