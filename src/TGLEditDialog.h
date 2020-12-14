#pragma once

#include "TGLMessageBox.h"

#include "TGLEdit.h"

class TGLEditDialog :
    public TGLMessageBox
{
private:
	std::unique_ptr<TGLEdit> edit;
public:
	TGLEditDialog(std::tstring text, std::tstring caption, unsigned int mb_code, float width, float height);
	void Draw(int w, int h);
	virtual void OnMouseMove(WPARAM mk_code, int x, int y);
	virtual int OnLButtonDown(WPARAM mk_code, int x, int y);
	virtual void OnChar(TCHAR tc, LPARAM lParam);
};

