#pragma once
#include "Scene.h"

#include "GLMap.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SceneGame :
    public Scene
{
private:
    int W, H;
    float t0;
    const float ival_p = 20;

    //
    const float std_status_height_p = 180;
    const float std_condition_height_p = 250;
    const float std_command_heigth_p = 515;

    const float std_map_height_p = std_status_height_p+ std_condition_height_p+ std_command_heigth_p+2*ival_p;
    const float std_H_p = std_map_height_p + ival_p * 2;
    
    //
    const float std_left_width_p = 940;
    const float std_right_width_p = 440;

    const float std_W_p = std_left_width_p + std_right_width_p + ival_p * 3;

    //
    const float std_command_width_p = 185;

    float text_height_p;
    float left_width_p,map_height_p; 
    float right_x1, status_height_p,right_width_p;
    float condition_y1; float condition_height_p;
    float command_width_p, command_height_p;
    //glm::fvec4 map_p,status_p,condition_p,command_p,location_p;

    GLMap wnd_map;
    GLWindow wnd_status;
    GLWindow wnd_condition;
    GLWindow wnd_command;
    GLWindow wnd_location;
    void Calc();
    void SetWndPos();
    void SetWndSize();
public:
    SceneGame(SceneController* controller);
    void Start(int W, int H)override;
    void Render(int W, int H)override;

    virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

