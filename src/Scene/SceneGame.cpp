#include "SceneGame.h"

#include "gl_assist.h"

void SceneGame::Calc()
{
    //status = 5 line
    float right_pure_height = std_status_height_p + std_condition_height_p + std_command_heigth_p;
    float now_pure = H - 4.0f * ival_p;

    float hcoe = now_pure / right_pure_height;//std*coe=real
    status_height_p = std_status_height_p * hcoe;//以status高度为准
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
    Scene(controller)
{
    t0= (float)GetTickCount() / 1000.0f;
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
    float t = (float)GetTickCount() / 1000.0f - t0;

    this->W = W; this->H = H;

    glClearColor(17.0f / 255.0f, 25.0f / 255.0f, 36.0f / 255.0f, 0);//设置清屏时的颜色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏。这里清掉颜色缓冲区与深度缓冲区

    Calc();
    SetWndPos();
    SetWndSize();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0, W, 0, H, -1, 1);

    wnd_map.DrawByPiexl(W, H);

    glColor4ub(23, 37, 64, 255);

    wnd_status.DrawByPiexl(W, H);
    wnd_condition.DrawByPiexl(W, H);
    wnd_command.DrawByPiexl(W, H);
    wnd_location.DrawByPiexl(W, H);
}

int SceneGame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    return 0;
}
