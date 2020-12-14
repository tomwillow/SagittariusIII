#include "TSmallWindow.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef _OPENGL
void TSmallWindow::Render()
{

	{
		glm::vec4 p1(-1.0f, -1.0f, 0.0f, 1.0f);
		glm::vec4 p2(1.0f, -1.0f, 0.0f, 1.0f);
		glm::vec4 p3(1.0f, 1.0f, 0.0f, 1.0f);
		glm::vec4 p4(-1.0f, 1.0f, 0.0f, 1.0f);
		p1 = p1;
		p2 = p2;
		p3 = p3;
		p4 = p4;

		glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1, 0, 1); glVertex2f(p1.x, p1.y);
		glColor3f(1, 1, 0); glVertex2f(p2.x, p2.y);
		glColor3f(1, 0, 1); glVertex2f(p3.x, p3.y);
		glColor3f(1, 0, 1); glVertex2f(p4.x, p4.y);
		glEnd();
	}
	{
		static float angle = 0.1f;
		glm::vec4 p1(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 p2(0.5f, 0.5f, 0.0f, 1.0f);
		glm::vec4 p3(0.0f, 0.5f, 0.0f, 1.0f);

		glm::mat4 R(1.0f);
		R = glm::rotate(R, angle, glm::vec3(0, 0, 1));

		p1 = R * p1;
		p2 = R * p2;
		p3 = R * p3;

		//glClearColor(0.4f, 0.5f, 0.4f, 0);//设置清屏时的颜色
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏。这里清掉颜色缓冲区与深度缓冲区
		//******以下内容为1.0版本绘制方法，高版本的不建议这样绘制了，尽量使用glDraw*系列函数以提高性能
		//设置点
		glBegin(GL_TRIANGLE_STRIP); //以画线方式绘制，
		glColor3f(1, 0, 0); glVertex2f(p1.x, p1.y);//中心端点，为红色
		glColor3f(0, 1, 0); glVertex2f(p2.x, p2.y);//到左上角中点端点，为绿色
		glColor3f(0, 0, 1); glVertex2f(p3.x, p3.y);//到上方中点端点，为蓝色
		glEnd();

		if (angle >= 2.0f * 3.1415f)
			angle = 0.0f;
		else
			angle += 0.1f;
	}

}
#endif