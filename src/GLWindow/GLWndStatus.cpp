#include "GLWndStatus.h"

GLWndStatus::GLWndStatus():
	GLWindow(TEXT("STATUS"))
{
	bk_color = { 23, 37, 64, 255 };
}

void GLWndStatus::DrawByPiexl(int W, int H)
{
	this->GLWindow::DrawByPiexl(W, H);


}
