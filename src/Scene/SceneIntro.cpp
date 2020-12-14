#include "SceneIntro.h"
#include "gl_assist.h"

#include "SceneController.h"

using namespace std;

SceneIntro::SceneIntro(SceneController* controller):Scene(controller),logo(make_unique<TFixedSpirit>(SOS_BRIGADE,0.5f, 0.5f, 0.8f, false))
{
}

void SceneIntro::Render(int w, int h)
{
	static auto start_tick = GetTickCount();
	float t = (float)(GetTickCount() - start_tick) / 1000.0f;

	//白色背景
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//设置清屏时的颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏。这里清掉颜色缓冲区与深度缓冲区

	const float fade_time = 0.8f;//淡入淡出时间
	const float show_time = 1.5f;//稳定显示时间
	float opacity;
	if (t<fade_time)//淡入
		opacity=t/fade_time;
	else
		if (t<fade_time+show_time)
			opacity = 1.0f;
		else
			if (t<fade_time*2.0f+show_time)//淡出
				opacity = 1.0f-(t-fade_time-show_time)/fade_time;
			else
			{
				//淡出结束，跳转场景
				controller->GoCover(w,h);
				return;
			}

	//设置透明度进行绘制
	EnableTexture();
	//printf("%f\n", opacity);
	glColor4f(1, 1, 1, opacity);
	logo->Draw(w, h);
}
