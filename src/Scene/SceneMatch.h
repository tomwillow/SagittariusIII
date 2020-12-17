#pragma once
#include "Scene.h"

#include "StarRender.h"

#include "TFixedSpirit.h"
#include "TFixedFreeType.h"

#include "TBoxFreeType.h"

#include "GLUsernameDialog.h"
#include "GLNetworkDialog.h"

#include "ExternResource.h"

class SceneMatch :
    public Scene
{
private:
    int W, H;
    std::unique_ptr<StarRender> starRender;

    std::unique_ptr<TBoxFreeType> text;

    std::unique_ptr<GLUsernameDialog> usernameDialog;
    std::unique_ptr<GLNetworkDialog> searchDialog;
public:
    //新建一个StarRender
    SceneMatch(SceneController* controller,int w,int h);

    //将传入的StarRender转移所有权
    SceneMatch(SceneController* controller,std::unique_ptr<StarRender> &lastStarRender);

    virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void Render(int w, int h)override;

    friend class SceneController;
};

