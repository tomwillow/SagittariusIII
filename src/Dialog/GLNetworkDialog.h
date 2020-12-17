#pragma once
#include "TGLDialog.h"

#include "TBoxFreeType.h"

#include "TGLEditDialog.h"
#include "TGLButton.h"

#include "ExternResource.h"

#define IDIPCONNECT 101
#define IDBUILDROOM 102

class GLNetworkDialog :
    public TGLDialog
{
private:
	SceneController* controller;
	const float x_margin = 0.08f;
	const float top_margin = 0.15f;
	const float bottom_margin = 0.2f;
	const int text_pixel = 48;
	float text_height;
	std::unique_ptr<TBoxFreeType> text;
	std::vector<TGLButton> buttons;
	std::unique_ptr<TGLEditDialog> ipDialog;
	std::unique_ptr<TGLMessageBox> ipMsgBox;
public:
	GLNetworkDialog(SceneController* in_controller);

	void Draw(int w, int h);

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

