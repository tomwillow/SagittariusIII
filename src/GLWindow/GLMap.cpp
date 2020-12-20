#include "GLMap.h"

#include "gl_assist.h"

GLMap::GLMap():
	GLWindow()
{
	bk_color = { 18,23,29,255 };
}

void GLMap::DrawByPiexl(int W, int H)
{
	this->GLWindow::DrawByPiexl(W, H);

}