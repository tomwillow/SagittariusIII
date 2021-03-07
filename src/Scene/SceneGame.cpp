#include "SceneGame.h"

#include "gl_assist.h"

#include "SceneController.h"

#include <algorithm>

using namespace std;

void SceneGame::Calc()
{
    //status = 5 line
    float right_pure_height = std_status_height_p + std_condition_height_p + std_command_heigth_p;
    float now_pure = H - 4.0f * ival_p;

    float hcoe = now_pure / right_pure_height;//std*coe=real
    status_height_p = std_status_height_p * hcoe;//��status�߶�Ϊ׼
    right_width_p = std_right_width_p / std_status_height_p * status_height_p;

    text_height_p = status_height_p / 5.0f;

    left_width_p = W - right_width_p - 3 * ival_p;

    map_height_p = H - 2 * ival_p;

    //map_p = { ival_p,ival_p,ival_p + left_width_p,ival_p + map_height_p };

    //
    condition_height_p = std_condition_height_p * hcoe;
    command_height_p = std_command_heigth_p * hcoe;
    command_width_p = std_command_width_p / std_command_heigth_p * command_height_p;

    right_x1 = 2 * ival_p + left_width_p;
    //location_p = { right_x1 + command_width_p + ival_p,ival_p,W - ival_p,ival_p + command_height_p };

    //command_p = { right_x1,ival_p,right_x1 + command_width_p,ival_p + command_height_p };

    //status_p = { right_x1,H - ival_p - status_height_p,W - ival_p,H - ival_p };

    condition_y1 = 2 * ival_p + command_height_p;
    //condition_p = { right_x1,condition_y1,W - ival_p,condition_y1 + condition_height_p };
}

void SceneGame::SetWndPos()
{
    wnd_map.SetPos(ival_p, ival_p);
    wnd_status.SetPos(right_x1, H - ival_p - status_height_p);
    wnd_condition.SetPos(right_x1, condition_y1);
    wnd_command.SetPos(right_x1, ival_p);
    wnd_location.SetPos(right_x1 + command_width_p + ival_p, ival_p);
}

void SceneGame::SetWndSize()
{
    wnd_map.SetSize(left_width_p, map_height_p);
    wnd_status.SetSize(right_width_p, status_height_p);
    wnd_condition.SetSize(right_width_p, condition_height_p);
    wnd_command.SetSize(command_width_p, command_height_p);
    wnd_location.SetSize(W - ival_p - right_x1 - command_width_p - ival_p, command_height_p);
}

SceneGame::SceneGame(SceneController* controller):
    Scene(controller),
    wnd_map(controller),
    wnd_location(controller),state(0)
{
    t0= (float)GetTickCount() / 1000.0f;
    memset(&vk_codes, 0, sizeof(vk_codes));
}

SceneGame::~SceneGame()
{
    GLWindow::ReleaseResource();
}

void SceneGame::Start(int W, int H)
{
    this->W = W; this->H = H;
    Calc();
    SetWndPos();
    SetWndSize();
}

void SceneGame::Render(int W, int H)
{
    this->W = W; this->H = H;
    float t = (float)GetTickCount() / 1000.0f - t0;
    static float prev_t = t;


    glClearColor(17.0f / 255.0f, 25.0f / 255.0f, 36.0f / 255.0f, 0);//��������ʱ����ɫ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//���������������ɫ����������Ȼ�����

    //��������
    auto& player = controller->players[0];

    //����ת��
    if (vk_codes[VK_LEFT] || vk_codes['A'])
    {
        player.angle += 5;
    }
    if (vk_codes[VK_RIGHT] || vk_codes['D'])
    {
        player.angle -= 5;
    }

    //ǰ��
    if (vk_codes[VK_UP] || vk_codes['W'])
    {
        if (player.speed < 10.0f)
        {
            if (state == 0)//ֹͣ״̬ת��ǰ��
            {
                state = 1;
                //printf("state = %d\n", state);
            }

            if (state == -1 && player.speed >= 0)//����״̬���ٶ��Ѿ�����0
                player.speed = 0;
            else
                player.speed += 0.1f;  
        }
    }

    //����
    if (vk_codes[VK_DOWN] || vk_codes['S'])
    {
        if (player.speed > -10.0f)
        {
            if (state == 0)//��ֹͣ״̬ת�뵹��
            {
                state = -1;
                //printf("state = %d\n", state);
            }

            if (state == 1 && player.speed <= 0)//ǰ��״̬���Ѿ�ɲס
                player.speed = 0;
            else

            player.speed -= 0.1f;
        }
    }
    //���㲢����λ��
    float dt = (t - prev_t);
    float sin_ = sin(player.angle / 180.0f * M_PI), cos_ = cos(player.angle / 180.0f * M_PI);

    player.x += -player.speed * sin_ * dt;
    player.y += player.speed * cos_ * dt;

    //������λ
    if (player.x < 0)
    {
        player.x = 0;
        player.speed = 0;
    }
    if (player.y < 0)
    {
        player.y = 0;
        player.speed = 0;
    }
    if (player.x >controller->bat_width)
    {
        player.x = controller->bat_width;
        player.speed = 0;
    }
    if (player.y >controller->bat_height)
    {
        player.y = controller->bat_height;
        player.speed = 0;
    }

    if (vk_codes[VK_SPACE])
        controller->PlaySoundEffect();

    //ͶӰ����ʹ����������
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, W, 0, H, -1, 1);

    //���Ƹ�����
    wnd_map.DrawByPiexl(W, H);
    wnd_status.DrawByPiexl(W, H);
    wnd_condition.DrawByPiexl(W, H);
    wnd_command.DrawByPiexl(W, H);
    wnd_location.DrawByPiexl(W, H);

    prev_t = t;
}

int SceneGame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    wnd_map.WndProc(uMsg, wParam, lParam);
    wnd_status.WndProc(uMsg, wParam, lParam);
    wnd_condition.WndProc(uMsg, wParam, lParam);
    wnd_command.WndProc(uMsg, wParam, lParam);
    wnd_location.WndProc(uMsg, wParam, lParam);
    switch (uMsg)
    {
    case WM_KEYDOWN:
    {
        WPARAM vk_code = wParam;
        vk_codes[vk_code] = true;
        break;
    }
    case WM_KEYUP:
    {
        WPARAM vk_code = wParam;
        vk_codes[vk_code] = false;

        if (vk_code==VK_UP || vk_code=='W')
        {
            if (state == -1 && abs(controller->players[0].speed) <= 0.1f)//����;��ǰ����ͣס
            {
                state = 0;
                //printf("state = %d\n", state);
            }
        }

        if (vk_code==VK_DOWN || vk_code=='S')
        {
            if (state == 1 && abs(controller->players[0].speed)<=0.1f)//ǰ��;��ɲ����ͣס
            {
                state = 0;
                //printf("state = %d\n", state);
            }
        }

        break;
    }
    case WM_KILLFOCUS:
    {
        memset(&vk_codes, 0, sizeof(vk_codes));//ʧȥ������հ������
        break;
    }
    case WM_SIZE:
    {
        W = GET_X_LPARAM(lParam), H = GET_Y_LPARAM(lParam);
        Calc();
        SetWndPos();
        SetWndSize();
        //printf("WM_SIZE\n");
        return 1;//���ݸ�Ĭ����Ϣ����
    }
    }
    return 0;
}
