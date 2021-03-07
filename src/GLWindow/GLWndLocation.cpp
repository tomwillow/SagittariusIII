#include "GLWndLocation.h"

#include "gl_assist.h"
#include "SceneController.h"

#include "ColorConfig.h"
#include "ExternResource.h"

GLWndLocation::GLWndLocation(SceneController* controller) :
	GLWindow(TEXT("LOCATION")), controller(controller),
	ship(IMG_SHIP)
{
	bk_color = { 23, 37, 64, 255 };
}

void GLWndLocation::DrawByPiexl(int W, int H)
{
	this->GLWindow::DrawByPiexl(W, H);

	//设置视口及投影坐标
	glViewport(ix1, iy1, iw, ih);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	glOrtho(0, iw, 0, ih, -1.0, 1.0);

	EnableGeometry();
	//glColor4ub(90, 191, 203, 255);
	glColor4ub(56, 63, 89, 255);
	glBegin(GL_LINES);
	const float grid_p = 4;
	const int x_grid_count = iw / grid_p;
	const int y_grid_count = ih / grid_p;
	for (int i = 0; i < x_grid_count + 1; ++i)
	{
		glVertex2f(grid_p * i, 0);
		glVertex2f(grid_p * i, ih);
	}
	for (float i = 0; i < y_grid_count + 1; i += 1.0f)
	{
		glVertex2f(0, grid_p * i);
		glVertex2f(iw, grid_p * i);
	}
	glVertex2f(0, 0);
	glVertex2f(iw, ih);
	glEnd();

	//
	EnableTexture();
	float ship_width_p = iw / 10.0f;
	float ship_scale = ship_width_p / (float)ship.width;

	auto team = controller->players[0].team;
	glMatrixMode(GL_MODELVIEW);
	for (auto& player : controller->players)
	{
		glLoadIdentity();
		glTranslatef(player.x / controller->bat_width * iw, player.y / controller->bat_height * ih, 0);
		if (player.team != team)
		{
			glRotatef(180.0f, 0, 0, 1);
		}
		SetColor(player.color);
		ship.DrawCenterByPixel(0,0, ship_scale);
	}

	glLoadIdentity();

	//恢复视口及投影坐标
	glViewport(0, 0, W, H);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
