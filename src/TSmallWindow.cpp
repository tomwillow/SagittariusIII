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

		//glClearColor(0.4f, 0.5f, 0.4f, 0);//��������ʱ����ɫ
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//���������������ɫ����������Ȼ�����
		//******��������Ϊ1.0�汾���Ʒ������߰汾�Ĳ��������������ˣ�����ʹ��glDraw*ϵ�к������������
		//���õ�
		glBegin(GL_TRIANGLE_STRIP); //�Ի��߷�ʽ���ƣ�
		glColor3f(1, 0, 0); glVertex2f(p1.x, p1.y);//���Ķ˵㣬Ϊ��ɫ
		glColor3f(0, 1, 0); glVertex2f(p2.x, p2.y);//�����Ͻ��е�˵㣬Ϊ��ɫ
		glColor3f(0, 0, 1); glVertex2f(p3.x, p3.y);//���Ϸ��е�˵㣬Ϊ��ɫ
		glEnd();

		if (angle >= 2.0f * 3.1415f)
			angle = 0.0f;
		else
			angle += 0.1f;
	}

}
#endif