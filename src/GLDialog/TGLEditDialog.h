#pragma once

#include "TGLMessageBox.h"

#include "TGLEdit.h"

class TGLEditDialog :
    public TGLMessageBox
{
private:
	std::unique_ptr<TGLEdit> edit;
public:
	TGLEditDialog(std::tstring text, std::tstring caption, unsigned int mb_code);

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	virtual void OnChar(TCHAR tc, LPARAM lParam);

	virtual std::tstring GetValue();
	virtual void Draw(int w, int h);
};

