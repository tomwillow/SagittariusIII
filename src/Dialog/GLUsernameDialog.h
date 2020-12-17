#pragma once
#include "TGLEditDialog.h"

class SceneController;
class GLUsernameDialog :
    public TGLEditDialog
{
private:
    SceneController* controller;
    std::unique_ptr<TGLMessageBox> msgBox;
public:
    GLUsernameDialog(SceneController *in_controller);
    virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    virtual void Draw(int w, int h)override;

    virtual std::tstring GetValue()override;
};

