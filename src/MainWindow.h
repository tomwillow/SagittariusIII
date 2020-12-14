#pragma once
#include "TWindow.h"
#include "TStatic.h"
#include "TSmallWindow.h"

#include "SceneController.h"

#include <memory>
#include "TTexture.h"

class MainWindow :
    public TWindow
{
private:
    std::unique_ptr<SceneController> controller;
    TSmallWindow win1, win2;
    void OnCreate(CREATESTRUCT* pCS)override;
    bool OnDraw(HDC hdc)override;
#ifdef _OPENGL
    void Render()override;
#endif

};

