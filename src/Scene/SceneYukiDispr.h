#pragma once
#include "Scene.h"
#include <queue>
#include "tstring.h"

#include "TBoxFreeType.h"

class SceneYukiDispr :
    public Scene
{
private:
    struct YukiMsg
    {
        bool has_prefix;
        std::tstring s;
        YukiMsg(bool in_has_prefix, std::tstring in_s) :
            has_prefix(in_has_prefix), s(in_s)
        {}
    };
    struct RenderedYukiMsg
    {
        bool has_prefix;
        std::unique_ptr<TBoxFreeType> text;
        RenderedYukiMsg(bool in_has_prefix, std::tstring in_s);
    };
    int W, H;
    float t0, t;
    std::unique_ptr<TBoxFreeType> prefix,text;
    std::vector<RenderedYukiMsg> old_msg;
    std::queue<YukiMsg> q;
    int GetYear();
    bool IsThatDay();

    //若当前为12月18日，且没有在该场景进行过确认，则进行显示
    bool ShouldShow();
public:
    //若符合显示条件，进行显示。不符合则不进行初始化
    SceneYukiDispr(SceneController* controller);
    void Start(int W,int H)override;//若不符合条件，此时将跳转至Intro
    void Render(int w, int h)override;

    virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

