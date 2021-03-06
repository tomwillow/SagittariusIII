#pragma once
#include "tstring.h"
#include <vector>

#include "TGLDialog.h"

#include "TGLButton.h"
#include "TBoxFreeType.h"

class TIniFile;
class TGLMessageBox :
    public TGLDialog
{
protected:
	const float x_margin = 0.08f;
	const float top_margin = 0.15f;
	const float bottom_margin = 0.2f;
	const int text_pixel = 48;
	float text_height;
	std::tstring s_content,s_caption;
	unsigned int mb_code;
	std::unique_ptr<TBoxFreeType> text;
	std::vector<TGLButton> buttons;
	TGLMessageBox(TIniFile *lang,std::tstring text, std::tstring caption, unsigned int mb_code,float width,float height);
public:
	TGLMessageBox(TIniFile* lang, std::tstring text,std::tstring caption,unsigned int mb_code);
	void Draw(int w, int h);

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual int OnKeyDown(WPARAM vk_code, LPARAM lParam);
	virtual void OnMouseMove(WPARAM mk_code, int x, int y);
	virtual int OnLButtonDown(WPARAM mk_code, int x, int y);
};

