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

	//��ɫ����
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//��������ʱ����ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//���������������ɫ����������Ȼ�����

	const float fade_time = 0.8f;//���뵭��ʱ��
	const float show_time = 1.5f;//�ȶ���ʾʱ��
	float opacity;
	if (t<fade_time)//����
		opacity=t/fade_time;
	else
		if (t<fade_time+show_time)
			opacity = 1.0f;
		else
			if (t<fade_time*2.0f+show_time)//����
				opacity = 1.0f-(t-fade_time-show_time)/fade_time;
			else
			{
				//������������ת����
				controller->GoCover(w,h);
				return;
			}

	//����͸���Ƚ��л���
	EnableTexture();
	//printf("%f\n", opacity);
	glColor4f(1, 1, 1, opacity);
	logo->Draw(w, h);
}
