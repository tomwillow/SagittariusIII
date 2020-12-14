#include "TStatic.h"
#include <stdexcept>

using namespace std;

const TCHAR className[] = TEXT("static");

void TStatic::Create(TWindow* pParent,RECT rect)
{
	TWindow::Create(NULL, className, WS_CHILD | WS_VISIBLE, 0, rect.left, rect.top, rect.right, rect.bottom,
		pParent, 0);// | SS_OWNERDRAW

	RegisterProc();
}

void TStatic::SetTransparent()
{
	assert(m_pParent);
	m_pParent->RegisterMessage(WM_CTLCOLORSTATIC, [](TWindow *pWin,WPARAM wParam, LPARAM lParam)
		{
			HDC hdcStatic = (HDC)wParam;
			SetBkMode(hdcStatic, TRANSPARENT);
			//SetTextColor(hdcStatic, RGB(255, 0,0));
			//SetBkColor(hdcStatic, RGB(0, 0, 0));
			return (INT_PTR)GetStockObject(NULL_BRUSH);//设置整个控件Client区透明
		});
}

#ifdef _OPENGL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void TStatic::Render()
{
	static float angle = 0.1f;
	glm::vec4 p1(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 p2(-1.0f, -1.0f, 0.0f, 1.0f);
	glm::vec4 p3(1.0f, -1.0f, 0.0f, 1.0f);

	glm::mat4 R(1.0f);
	R = glm::rotate(R, angle, glm::vec3(0, 0, 1));

	//p1 = M * R * p1;
	//p2 = M * R * p2;
	//p3 = M * R * p3;

	p1 = R * p1;
	p2 = R * p2;
	p3 = R * p3;

	//设置点
	glBegin(GL_TRIANGLE_STRIP); //以画线方式绘制，
	glColor3f(1, 0, 0); glVertex2f(p1.x, p1.y);//中心端点，为红色
	glColor3f(1, 0, 0); glVertex2f(p2.x, p2.y);//到左上角中点端点，为绿色
	glColor3f(1, 0, 0); glVertex2f(p3.x, p3.y);//到上方中点端点，为蓝色
	glEnd();

	if (angle >= 2.0f * 3.1415f)
		angle = 0.0f;
	else
		angle += 0.1f;
}
#endif