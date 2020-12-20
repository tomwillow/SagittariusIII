#include "GLWindow.h"

#include "gl_assist.h"

#include "ExternResource.h"

using namespace std;

TTexture* GLWindow::tex_border=nullptr;

GLWindow::GLWindow():
    caption(make_unique<TFreeType>(TEXT("Window"),FONT_CJK)),
    border(4),header(16),caption_align_center(false),bk_color({255,0,0,255})
{
    if (tex_border == nullptr)
    {
    tex_border = new TTexture[8]{
        TTexture(IMG_WINDOW_01),
        TTexture(IMG_WINDOW_02),
        TTexture(IMG_WINDOW_03),
        TTexture(IMG_WINDOW_04),
        TTexture(IMG_WINDOW_05),
        TTexture(IMG_WINDOW_06),
        TTexture(IMG_WINDOW_07),
        TTexture(IMG_WINDOW_08) };
    }
}

void GLWindow::DrawByPiexl(int W, int H)
{
    EnableGeometry();
    glColor4ub(bk_color.r, bk_color.g, bk_color.b, bk_color.a);
    DrawRect(GL_QUADS, x1, y1, x1 + w, y1 + h);
    float x2 = x1 + w;
    float y2 = y1 + h;

    EnableTexture();
    glColor4f(1, 1, 1, 1);

    //left
    tex_border[0].DrawByClipCoord(x1, y2 - header, x1 + border, y2);
    tex_border[1].DrawByClipCoord(x1,y1,x1+border,y1+border);
    tex_border[4].DrawByClipCoord(x1, y1 + border, x1 + border, y2 - header);

    //bottom
    tex_border[2].DrawByClipCoord(x2 - border, y1, x2, y1 + border);
    tex_border[5].DrawByClipCoord(x1 + border, y1, x2 - border, y1 + border);

    //right
    tex_border[3].DrawByClipCoord(x2 - border, y2 - header, x2, y2);
    tex_border[6].DrawByClipCoord(x2 - border, y1 + border, x2, y2 - header);

    //header
    tex_border[7].DrawByClipCoord(x1 + border, y2 - header, x2 - border, y2);

    //caption
    glColor4f(0, 0, 0, 1);
    caption->SetFontSizeScale(header / 48.0f);
    float center_diff = header;
    if (caption_align_center)
    {
        float caption_width = caption->GetWidthPixel();
        center_diff = (x2 - x1 - 2 * border) / 2.0f - caption_width / 2.0f;
    }
    caption->DrawByPixel(x1+border+center_diff, y2 - header+border/2.0f);
}

void GLWindow::SetWindow(float x1, float y1, float w, float h)
{
    this->x1 = x1; this->y1 = y1; this->w = w; this->h = h;
}

void GLWindow::SetSize(float w, float h)
{
    this->w = w; this->h = h;
}

void GLWindow::SetPos(float x1, float y1)
{
    this->x1 = x1; this->y1 = y1;
}
