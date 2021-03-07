#include "GLMap.h"

#include "gl_assist.h"

#include "ColorConfig.h"
#include "ExternResource.h"

#include "SceneController.h"

GLMap::GLMap(SceneController* controller):
	controller(controller),
	GLWindow(TEXT("Map ( x1 )")),
	ship(IMG_SHIP),
	mask("textures/mask1.png")
{
	bk_color = { 18,23,29,255 };
}

void GLMap::DrawByPiexl(int W, int H)
{
	this->GLWindow::DrawByPiexl(W, H);

	float bat_wnd_width = iw / grid_pixel;
	float bat_wnd_height = ih / grid_pixel;

	//
	auto& player = controller->players[0];
	float x = player.x, y = player.y;
	float x_p, y_p=0.0f;
	//if (x > bat_wnd_width / 2.0f)
	//{
	//	x_p = iw / 2.0f;
	//}
	//else
	{
		x_p = x * grid_pixel;
	}
	//if (y > bat_wnd_height / 2.0f)
	//{
	//	y_p = h / 2.0f;
	//}
	//else
	{
		y_p = y * grid_pixel;
	}

	//设置视口及投影坐标
	glViewport(ix1, iy1, iw, ih);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	glOrtho(0, iw, 0, ih, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//此处开始窗口范围0,0,iw,ih

	//glColorMask(1, 1, 1, 1);

	StartDrawMaskLayer();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	{
		glLoadIdentity();


		glEnable(GL_TEXTURE_2D);
		glColor4f(1, 1, 1, 1);
		mask.DrawCenterByPixel(x_p, y_p);
	}
	glDisable(GL_ALPHA_TEST);

	StartDrawMaskedLayer();
	{

		//测试画线
		glDisable(GL_TEXTURE_2D);
		glColor4f(1, 0, 1, 1);
		glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(iw, ih);
		glEnd();

		glRectf(0, 0, iw, ih);

		{
			//设置模型矩阵
			glLoadIdentity();

			glTranslatef(x_p, y_p, 0);//3.位移至指定位置
			glRotatef(player.angle, 0, 0, 1);//2.旋转
			glTranslatef(0, 25, 0);//1.重心向下移25

			//ship
			glEnable(GL_TEXTURE_2D);
			SetColor(player.color);
			ship.DrawCenterByPixel(0, 0);
		}
	}
	EndMask();

	//恢复模型矩阵
	glLoadIdentity();

	//恢复视口及投影坐标
	glViewport(0, 0, W, H);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();


}