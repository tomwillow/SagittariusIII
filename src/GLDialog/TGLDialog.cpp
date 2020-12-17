#include "TGLDialog.h"

#include <Windows.h>
#include <gl\GL.h>

#include "gl_assist.h"
using namespace std;

TGLDialog::TGLDialog(float width, float height):width(width),height(height),
bk(make_unique<TTexture>(IMG_DIALOG_BG)),fade_time(0.1f)
{
	t0 = (float)GetTickCount() / 1000.0f;
}

void TGLDialog::Draw(int w, int h)
{
	t = (float)GetTickCount() / 1000.0f-t0;

	EnableTexture();

	glColor4f(1, 1, 1, 1);

	float show_h;
	if (t < fade_time)
		show_h = height / 2.0f * t/fade_time;
	else
		show_h = height / 2.0f;

	x1 = -width / 2.0f;
	x2 = width / 2.0f;
	y1 = -show_h;
	y2 = show_h;

	bk->DrawByClipCoord(x1,y1,x2,y2);
}
