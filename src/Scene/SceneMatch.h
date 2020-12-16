#pragma once
#include "Scene.h"

#include "StarRender.h"

#include "TFixedSpirit.h"
#include "TFixedFreeType.h"

#include "TBoxFreeType.h"

#include "TGLEditDialog.h"

#include "ExternResource.h"

class SceneMatch :
    public Scene
{
private:
    int W, H;
    std::unique_ptr<StarRender> starRender;

    std::unique_ptr<TBoxFreeType> text;

    std::unique_ptr<TGLEditDialog> editDialog;
    std::unique_ptr<TGLMessageBox> msgBox;
public:
    //新建一个StarRender
    SceneMatch(SceneController* controller,int w,int h);

    //将传入的StarRender转移所有权
    SceneMatch(SceneController* controller,std::unique_ptr<StarRender> &lastStarRender);
    void OnChar(TCHAR tc, LPARAM lParam) override;
    virtual void OnMouseMove(WPARAM mk_code, int x, int y) override;
    virtual void OnLButtonDown(WPARAM mk_code, int x, int y) override;
	void Render(int w, int h);

    friend class SceneController;
};

