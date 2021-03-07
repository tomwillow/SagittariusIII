#pragma once
#include "Scene.h"

#include "TFixedSpirit.h"
#include "TFixedFreeType.h"

#include "StarRender.h"

#include "ExternResource.h"

class SceneCover :
    public Scene
{
private:
    int W, H;
    std::unique_ptr<TFixedSpirit> tex1, tex2, tex3, round_rect, copyright;

    bool hoverStart, hoverLoad, hoverExit;
    std::unique_ptr<TFixedFreeType> textStart, textLoad, textExit;
    std::unique_ptr<TFreeType> text;

    std::unique_ptr<StarRender> starRender;
public:
    SceneCover(SceneController* controller);
    void Start(int w, int h)override;

    virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    //void OnKeyDown(WPARAM vk_code, LPARAM lParam)override;
    //void OnMouseMove(WPARAM mk_code, int x, int y) override;
    //void OnLButtonDown(WPARAM mk_code, int x, int y)override;

    void Render(int w, int h)override;

    friend class SceneController;
};

