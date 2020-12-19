#include "SceneYukiDispr.h"

#include "gl_assist.h"
#include "ExternResource.h"

#include "SceneController.h"

using namespace std;

#define INI_KEY TEXT("yuki_dispearence")

int SceneYukiDispr::GetYear()
{
    time_t curtime;
    time(&curtime);
    tm* info = localtime(&curtime);
    return info->tm_year+1900;
}

bool SceneYukiDispr::IsThatDay()
{
    time_t curtime;
    time(&curtime);
    tm* info = localtime(&curtime);
    return info->tm_mon==11 && info->tm_mday==18;//12.18
}

bool SceneYukiDispr::ShouldShow()
{
    if (!IsThatDay())
        return false;
    int year = GetYear();
    int last_remind = 0;
    try
    {
        last_remind = stoi(controller->ini.GetValue(INI_KEY));
    }
    catch (...)
    {
        last_remind = 0;
    }
    return year>last_remind;
}

SceneYukiDispr::SceneYukiDispr(SceneController* controller):
	Scene(controller)
{
    if (!ShouldShow())
        return;

    t0 = (float)GetTickCount() / 1000.0f;
    prefix = make_unique<TBoxFreeType>(TEXT("YUKI.N>"), FONT_CJK, 24);
    q.push({ 1,TEXT("これをあなたが読んでいる時、") });
    q.push({ 0,TEXT("わたしはわたしではないだろう。")});
    q.push({ 0,TEXT("")});
    q.push({ 1,TEXT("このメッセージが表示されたということは、")});
    q.push({ 0,TEXT("そこにはあなた、わたし、涼宫ハルヒ、朝比奈みくる、") });
    q.push({ 0,TEXT("古泉一樹が存在しているはずである。") });
    q.push({ 0,TEXT("") });
    q.push({ 1,TEXT("そわが键。") });
    q.push({ 0,TEXT("あなたは解答を見つけ出した。") });
    q.push({ 0,TEXT("") });
    q.push({ 1,TEXT("これは緊急脱出プログラムである。") });
    q.push({ 0,TEXT("") });
    q.push({ 1,TEXT("起動させる場合は工ソ夕一キ一を、") });
    q.push({ 0,TEXT("そうでない場合はそれ以外のキ一を選択せよ。") });
    q.push({ 0,TEXT("") });
    q.push({ 0,TEXT("起動させた場合、") });
    q.push({ 0,TEXT("あなたは時空修正の機会を得る。") });
    q.push({ 0,TEXT("ただし成功は保証できない。") });
    q.push({ 0,TEXT("また帰還の保証もできない。") });
    q.push({ 0,TEXT("") });
    q.push({ 1,TEXT("このプログラムが起動するのは一度きりである。") });
    q.push({ 0,TEXT("実行ののち、消去される。") });
    q.push({ 0,TEXT("") });
    q.push({ 0,TEXT("非実行が選択された場合は起動せずに消去される。") });
    q.push({ 0,TEXT("") });
    q.push({ 0,TEXT("Ready？") });
    //old_msg.push_back({ true,TEXT("aa")});
    //old_msg.push_back({ false,TEXT("bb") });

}

void SceneYukiDispr::Start(int W, int H)
{
    if (!ShouldShow())
        controller->GoIntro(W, H);
}

void SceneYukiDispr::Render(int W, int H)
{
    static float last_t = 0.0f;
    static unique_ptr<YukiMsg> yuki_showing;
    t = (float)GetTickCount() / 1000.0f-t0;

    this->W = W; this->H = H;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//设置清屏时的颜色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏。这里清掉颜色缓冲区与深度缓冲区


    if (yuki_showing)
    {
        static int count = 0;
        if (t-last_t > 0.1f)
        {

            count++;

            text = make_unique<TBoxFreeType>(yuki_showing->s.substr(0, count), FONT_CJK, 24);

            if (count > yuki_showing->s.size())
            {
                //
                old_msg.push_back({ yuki_showing->has_prefix,yuki_showing->s });
                text = nullptr;

                //
                count = 0;
                yuki_showing = nullptr;

            }
            last_t = t;
        }
    }
    else
    {
        if (!q.empty())
        {
            yuki_showing = make_unique<YukiMsg>(q.front().has_prefix,q.front().s);
            q.pop();
        }
        else
            yuki_showing = nullptr;
    }

    //total line = 26
    float pixel = 24;
    float total_line = 26+2;
    float want_height = 2.0f / total_line;
    float char_width = pixel * 2.0f / W;
    float prefix_width = char_width * 7;

    EnableTexture();
    float coe = want_height / (pixel*2.0f / H);
    float h = want_height;

    float x1 = -1.0f;
    float x2 = 1.0f;
    float y2 = 1.0f;
    float y1 = y2 - h;
    for (auto& msg : old_msg)
    {
        if (msg.has_prefix)
        {
            prefix->SetFontSizeScale(coe);
            prefix->DrawByClipCoord(W, H, x1, y1, x2, y2);
        }
        msg.text->SetFontSizeScale(coe);
        msg.text->DrawByClipCoord(W, H, x1+ prefix_width, y1, x2, y2);
        y2 = y1;
        y1 -= h;
    }

    if (text)
    {
        if (yuki_showing->has_prefix)
        {
            prefix->SetFontSizeScale(coe);
            prefix->DrawByClipCoord(W, H, x1, y1, x2, y2);
        }
        text->SetFontSizeScale(coe);
            text->DrawByClipCoord(W, H, x1+ prefix_width, y1, x2, y2);
    }
}

int SceneYukiDispr::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (q.empty())
    {
        if (uMsg == WM_KEYDOWN)
        {
            if (wParam == VK_RETURN)
            {
                controller->ini.SetValue(INI_KEY, GetYear());
                controller->ini.SaveToFile();
                controller->GoIntro(W, H);
                return 0;
            }
            else
            {
                exit(0);
            }
        }
    }
    return 0;
}

SceneYukiDispr::RenderedYukiMsg::RenderedYukiMsg(bool in_has_prefix, std::tstring in_s):
    has_prefix(in_has_prefix),
    text(std::make_unique<TBoxFreeType>(in_s, FONT_CJK, 24))
{
}