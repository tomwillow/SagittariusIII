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

    //飛貧匯肝短嗤序佩狛鳩範��賜貧匯肝鳩範厮将頁肇定式參念
    return year>last_remind;
}

SceneYukiDispr::SceneYukiDispr(SceneController* controller):
	Scene(controller)
{
    if (!ShouldShow())
        return;

    t0 = (float)GetTickCount() / 1000.0f;
    prefix = make_unique<TBoxFreeType>(TEXT("YUKI.N>"), FONT_CJK, 24);
    q.push({ 1,TEXT("これをあなたが�iんでいる�r、") });
    q.push({ 0,TEXT("わたしはわたしではないだろう。")});
    q.push({ 0,TEXT("")});
    q.push({ 1,TEXT("このメッセ�`ジが燕幣されたということは、")});
    q.push({ 0,TEXT("そこにはあなた、わたし、��幸ハルヒ、劾曳栂みくる、") });
    q.push({ 0,TEXT("硬畑匯�笋�贋壓しているはずである。") });
    q.push({ 0,TEXT("") });
    q.push({ 1,TEXT("そわが囚。") });
    q.push({ 0,TEXT("あなたは盾基を��つけ竃した。") });
    q.push({ 0,TEXT("") });
    q.push({ 1,TEXT("これは�o識用竃プログラムである。") });
    q.push({ 0,TEXT("") });
    q.push({ 1,TEXT("軟�咾気擦���栽は垢ソ櫓匯キ匯を、") });
    q.push({ 0,TEXT("そうでない��栽はそれ參翌のキ匯を�x�kせよ。") });
    q.push({ 0,TEXT("") });
    q.push({ 0,TEXT("軟�咾気擦���栽、") });
    q.push({ 0,TEXT("あなたは�r腎俐屎の�C氏を誼る。") });
    q.push({ 0,TEXT("ただし撹孔は隠�^できない。") });
    q.push({ 0,TEXT("また｢��の隠�^もできない。") });
    q.push({ 0,TEXT("") });
    q.push({ 1,TEXT("このプログラムが軟�咾垢襪里碗散箸�りである。") });
    q.push({ 0,TEXT("�g佩ののち、��肇される。") });
    q.push({ 0,TEXT("") });
    q.push({ 0,TEXT("掲�g佩が�x�kされた��栽は軟�咾擦困墨�肇される。") });
    q.push({ 0,TEXT("") });
    q.push({ 0,TEXT("Ready��") });
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//譜崔賠徳扮議冲弼
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//賠徳。宸戦賠渠冲弼産喝曝嚥侮業産喝曝


    if (yuki_showing)//嗤棋�塋承陳敞�
    {
        static int count = 0;
        if (t-last_t > 0.1f)//器欺忖憲柳埔扮字
        {

            count++;

            text = make_unique<TBoxFreeType>(yuki_showing->s.substr(0, count), FONT_CJK, 24);//奐紗忖憲

            if (count > yuki_showing->s.size())//厮�塋祥蟇笑�
            {
                //僕秘喟消�塋焦�
                old_msg.push_back({ yuki_showing->has_prefix,yuki_showing->s });
                text = nullptr;

                //賠腎棋�塋衝峽�
                count = 0;
                yuki_showing = nullptr;

            }
            last_t = t;
        }
    }
    else
    {
        if (!q.empty())//珊嗤棋�塋渉敞�
        {
            //函竃1佩旺pop
            yuki_showing = make_unique<YukiMsg>(q.front().has_prefix,q.front().s);
            q.pop();
        }
        else
            yuki_showing = nullptr;
    }

    //total line = 26
    float pixel = 24;
    float total_line = 26+2;//徳鳥悳佩方+2
    float want_height = 2.0f / total_line;
    float prefix_width = (float)prefix->GetWidthPixel()*2.0f/W;//念弸錐業

    EnableTexture();
    float coe = want_height / (pixel*2.0f / H);//忖憲寄弌距屁狼方��宥狛核參狼方聞徳鳥�塋捷牟�佩方
    float h = want_height;

    float x1 = -1.0f;
    float x2 = 1.0f;
    float y2 = 1.0f;
    float y1 = y2 - h;

    //喟消�塋床新�
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

    //幟忖�塋床新�
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