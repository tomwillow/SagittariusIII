#pragma once
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <cmath>
#include <stdexcept>
#include "TWindow.h"

using namespace std;

bool TWindow::m_bMainWindow = false;

#ifdef _OPENGL
float TWindow::fps;
#endif

TWindow::~TWindow()
{
	if (m_hFont != NULL)
		::DeleteObject(m_hFont);

	//�����ٻ��ŵĶ���
	for (auto& p : childs)
	{
		if (p.second == true)
			delete p.first;
	}

	//�ڸ��ڵ㴦ע���Լ�
	if (m_pParent)
		m_pParent->childs[this] = false;
}


void TWindow::RegisterMessage(UINT uMsg, PMsgDealer pFunc)
{
	msgDealer[uMsg] = pFunc;
}

LRESULT TWindow::WndProc(WNDPROC defProc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto it = msgDealer.find(uMsg);
	if (it != msgDealer.end())
	{
		if (it->second(this, uMsg, wParam, lParam) == 0)
			return 0;
	}
	switch (uMsg)
	{
	case WM_CREATE:
		GetWindowRect();
		GetClientRect();
		m_hWnd = hWnd;

#ifdef _OPENGL
		glhdc = win32_get_gl_dc(hWnd);
		hglrc = gl_create(glhdc);
		wglMakeCurrent(glhdc, hglrc);
#endif

		OnCreate((CREATESTRUCT*)lParam);
		break;
	case WM_MOVE:
		GetWindowRect();
		return 0;
	case WM_SIZE:
		GetWindowRect();
		GetClientRect();
#ifdef _OPENGL
		if (m_bMainWindow)
			glViewport(0, 0, ClientRect.right, ClientRect.bottom);//�����ӿ�
#endif
		OnSize(wParam, lParam);
		return 0;
	case WM_SETFOCUS:
#ifdef _OPENGL
		//isFocus = true;
#endif
		OnSetFocus();
		return 0;
	case WM_KILLFOCUS:
#ifdef _OPENGL
		//isFocus = false;
#endif
		OnKillFocus();
		return 0;
#ifdef _OPENGL
	case WM_ACTIVATE:
	{
		WORD wa = LOWORD(wParam);
		if (wa == WA_INACTIVE)
			isFocus = false;
		else
			isFocus = true;
		return 0;
	}
#endif
	case WM_DRAWITEM:
		return 0;
		//case WM_ERASEBKGND:
		//	return 0;
		//	if (m_bDoubleBuffer == false)
		//		DefWindowProc(hWnd, uMsg, wParam, lParam);
		//	return 0;
	case WM_PAINT:
		if (m_bDrawSelf == false)
			break;

		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect();

		bool useDefProc;
		if (m_bDoubleBuffer)
		{
			HDC hDCMem;
			HBITMAP hBitmap;
			hDCMem = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top);
			SelectObject(hDCMem, hBitmap);

			//toDo:
			useDefProc = OnDraw(hDCMem);

			BitBlt(hdc, 0, 0, ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top, hDCMem, 0, 0, SRCCOPY);
			DeleteObject(hBitmap);
			DeleteDC(hDCMem);
		}
		else
			useDefProc = OnDraw(ps.hdc);

		EndPaint(hWnd, &ps);
		if (useDefProc)
			break;
		else
			return 0;

	case WM_DESTROY:
		if (m_bMainWindow)
			PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		if (OnClose())
			break;//Quit
		else
			return 0;
	}

	return CallWindowProc(defProc, hWnd, uMsg, wParam, lParam);
}


LRESULT CALLBACK TWindow::StaticProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TWindow* pWindow;

	if (uMsg == WM_NCCREATE)
	{
		pWindow = (TWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;

#ifdef _WIN64
		SetWindowLong(hWnd, GWLP_USERDATA, pWindow);
#else
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)pWindow);
#endif
	}
	else
#ifdef _WIN64
		pWindow = (TWindow*)GetWindowLong(hWnd, GWLP_USERDATA);
#else
		pWindow = (TWindow*)GetWindowLong(hWnd, GWL_USERDATA);
#endif

#ifdef _DEBUG
	TCHAR temp[MAX_PATH];//
	wsprintf(temp, TEXT("Message(0x%x, 0x%x, 0x%x, 0x%x)\n"), hWnd, uMsg, wParam, lParam);
	OutputDebugString(temp);
#endif

	if (pWindow)
	{
#ifdef _DEBUG
		//TCHAR temp[MAX_PATH], name[MAX_PATH];

		////GetWindowText(hWnd, name, MAX_PATH);
		//wsprintf(temp, TEXT("Message(0x%x, 0x%x, 0x%x, 0x%x)\n"),hWnd, uMsg,wParam,lParam);
		//OutputDebugString(temp);
#endif
		return pWindow->WndProc(DefWindowProc, hWnd, uMsg, wParam, lParam);
	}
	else
		return CallWindowProc(DefWindowProc, hWnd, uMsg, wParam, lParam);//DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void TWindow::MyRegisterClass(LPCTSTR lpszClass)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);

	//���������Ƿ���ע�ᣬ��ע���򷵻�true����������Ϣ
	if (!GetClassInfoEx(m_hInst, lpszClass, &wc))
	{
		//δע�ᣬ�������
		memset(&wc, 0, sizeof(wc));

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = 0; // CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = StaticProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hIcon = m_hTitleIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_MENU + 1);//
		wc.lpszMenuName = NULL;
		wc.hIconSm = NULL;

		//���Ͼ�Ϊ����
		wc.hInstance = m_hInst;
		wc.lpszClassName = lpszClass;

		if (!RegisterClassEx(&wc))
			throw runtime_error("aaa");
	}
}


void TWindow::Create(LPCTSTR lpszName, LPCTSTR lpszClass, DWORD dwStyle, DWORD dwExStyle,
	int x, int y, int nWidth, int nHeight, TWindow* pParent,
	HMENU hMenu)
{
	MyRegisterClass(lpszClass);

	m_hInst = m_hInst;
	m_pParent = pParent;

	m_hWnd = CreateWindowEx(dwExStyle, lpszClass,
		lpszName, dwStyle, x, y, nWidth, nHeight,
		pParent ? pParent->GetHWND() : 0, hMenu, m_hInst, this);

	if (!m_hWnd)
		throw runtime_error("CreateWindowEx failed.");

	if (m_pParent)
	{
		m_pParent->childs[this] = true;
	}
}

void TWindow::LoadTitleIcon(UINT id)
{
	m_hTitleIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(id));
}

WPARAM TWindow::MessageLoop()
{
	MSG msg;
	if (m_bMainWindow == false)
		m_bMainWindow = true;
	else
		throw runtime_error("Rather than one MessageLoop is called.");


#ifdef _OPENGL

	wglMakeCurrent(glhdc, hglrc);//����gl��DC�����ڵ�hdc������ǵ�DC��ֻ�����һ�μ���

bool bQuit = false;
while (!bQuit)
{
	static auto lastTick = GetTickCount();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			bQuit = true;// return;
		if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Render();
	SwapBuffers(glhdc);//��ת����������Ϊʹ����˫���壬����Ҫ��ת���ܿ������ƽ����

	auto nowTick = GetTickCount();
	fps = 1000.0f / (nowTick - lastTick);
	lastTick = nowTick;
	Sleep(1000.0 / 60 - (nowTick - lastTick));//����֡��60
}

#else

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
#endif

	return msg.wParam;
}

void TWindow::SetAccel(UINT id)
{
	m_hAccelTable = LoadAccelerators(m_hInst, (LPCTSTR)id);
}

void TWindow::SetDoubleBuffer(bool bDoubleBuffer)
{
	m_bDoubleBuffer = bDoubleBuffer;
}

void TWindow::SetDrawSelf(bool bDrawSelf)
{
	m_bDrawSelf = bDrawSelf;
}

bool TWindow::ShowWindow(int nCmdShow) const
{
	return ::ShowWindow(m_hWnd, nCmdShow);
}

bool TWindow::UpdateWindow() const
{
	return ::UpdateWindow(m_hWnd);
}

void TWindow::Invalidate()
{
	InvalidateRect(m_hWnd, &GetClientRect(), FALSE);
}


HWND TWindow::GetHWND()
{
	return m_hWnd;
}

void TWindow::SetFont(HFONT hFont)
{
	assert(m_hWnd);
	SendMessage(m_hWnd,             // Handle of edit control
		WM_SETFONT,         // Message to change the font
		(WPARAM)hFont,     // handle of the font
		MAKELPARAM(TRUE, 0) // Redraw text
	);
}

HFONT TWindow::GetFont()
{
	return (HFONT)SendMessage(m_hWnd, WM_GETFONT, 0, 0);;
}

void TWindow::SetDefaultGuiFont()
{
	if (m_hFont != NULL)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	m_hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);//
	SetFont(m_hFont);
}

void TWindow::SetFont(TCHAR FontName[], int FontSize)
{
	if (m_hFont != NULL)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	_tcscpy_s(lf.lfFaceName, _tcslen(FontName) + 1, FontName);
	lf.lfHeight = -MulDiv(FontSize, GetDeviceCaps(GetDC(m_hWnd), LOGPIXELSY), 72);
	//lf.lfWeight = 900;

	m_hFont = CreateFontIndirect(&lf);
	SetFont(m_hFont);
}

void TWindow::SetText(const tstring& s)
{
	assert(m_hWnd);
	::SetWindowText(m_hWnd, s.c_str());
}

void TWindow::GetText(TCHAR text[])
{
	assert(m_hWnd);
	::GetWindowText(m_hWnd, text, GetLength() + 1);
}

tstring TWindow::GetText() const
{
	assert(m_hWnd);
	int len = GetLength() + 1;
	TCHAR* temp = new TCHAR[len];
	::GetWindowText(m_hWnd, temp, len);
	tstring s(temp);
	delete[] temp;
	return s;
}

int TWindow::GetLength() const
{
	assert(m_hWnd);
	return ::GetWindowTextLength(m_hWnd);
}

void TWindow::SetVisible(bool bShow)
{
	assert(m_hWnd);
	::ShowWindow(m_hWnd, bShow ? SW_SHOWNORMAL : SW_HIDE);
}

bool TWindow::GetVisible()
{
	assert(m_hWnd);
	return (bool)IsWindowVisible(m_hWnd);
}

void TWindow::SetEnable(bool bEnable)
{
	assert(m_hWnd);
	EnableWindow(m_hWnd, bEnable);
}

bool TWindow::GetEnable()
{
	assert(m_hWnd);
	return (bool)IsWindowEnabled(m_hWnd);
}

//�Զ�ȥ��С��ĩβ0�������ʾ6λ
void TWindow::SetDouble(double d)
{
	tstring s(tto_string(d));
	SetText(s);
}


//��ʹ��x,y���꣬width,heightʹ��ԭ��С
void TWindow::SetPositionOnlyOrigin(const RECT& rect)
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	SetPosition(rect.left, rect.top, rc.right, rc.bottom);
}

//right��bottom������ǿ�͸�
void TWindow::SetRect(RECT rect)
{
	::MoveWindow(m_hWnd, rect.left, rect.top, rect.right, rect.bottom, true);
}

//�Խǵ�����
void TWindow::SetRect(int x1, int y1, int x2, int y2)
{
	::MoveWindow(m_hWnd, x1, y1, x2 - x1, y2 - y1, true);
}

//���أ�����ڸ����ڵ� x1, y1, x2, y2
RECT TWindow::GetPosition()
{
	RECT rc = GetWindowRect();
	POINT pt1 = { rc.left,rc.top };
	POINT pt2 = { rc.right,rc.bottom };
	ScreenToClient(m_pParent->GetHWND(), &pt1);
	ScreenToClient(m_pParent->GetHWND(), &pt2);
	return { pt1.x,pt1.y,pt2.x,pt2.y };
}

void TWindow::SetPosition(int x, int y, int width, int height)
{
	::MoveWindow(m_hWnd, x, y, width, height, true);
	//::SetWindowPos(m_hWnd, HWND_TOP, x, y, width, height, 0);//SWP_SHOWWINDOW
}

//rect��Ϊx1,y1,x2,y2
void TWindow::SetPosition(RECT rect)
{
	::MoveWindow(m_hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
	//::SetWindowPos(m_hWnd, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0);//SWP_SHOWWINDOW
}

//��ù�������С
RECT TWindow::GetClientRect()
{
	::GetClientRect(m_hWnd, &ClientRect);
	return ClientRect;
}

RECT TWindow::GetWindowRect()
{
	::GetWindowRect(m_hWnd, &WindowRect);
	return WindowRect;
}

int GetFontHeight(HFONT hFont)
{
	LOGFONT lf;
	GetObject(hFont, sizeof(lf), &lf);
	HDC hdc = GetDC(NULL);
	double DOTSY = GetDeviceCaps(hdc, LOGPIXELSY);
	ReleaseDC(NULL, hdc);
	int pixels = (int)abs(((double)lf.lfHeight / DOTSY) * 72.0);
	return pixels;
}

#ifdef _OPENGL
//����OpenGL API
HGLRC gl_create(HDC hdc)
{
	auto hglrc = wglCreateContext(hdc);
	if (!hglrc)
		throw runtime_error("wglCreateContext failed.");
	if (!wglMakeCurrent(hdc, hglrc))
		throw runtime_error("wglMakeCurrent failed.");
	//******��ȡ�߰汾API��glew������Ҫ���ܾ�������һ����
	/*
	 ��������ȡ���õ���opengl api����ЩAPI���ƿ���ȥopengl������ѯ
	 ��Ϊ��Ҫ�Ĺ��ܲ����ӣ�����Ҫʹ����ɫ����GL1.0�汾��API�㹻���ˡ�
	 �������ﲢû�л�ȡ���и߰汾��API��ֻ��ȡ��һ��glCreateShader��Ϊ���ӡ�
	*/
	auto glCreateShader = (GLuint(__stdcall*)(GLenum type))wglGetProcAddress("glCreateShader");
	//******��ȡ�߰汾API����
	return hglrc;
}

//��ȡ������OpenGL���Ƶ�DC
//https://blog.csdn.net/hhhhhhhhhhkkkkkkkkkk/article/details/103600876#comments_13890857
HDC win32_get_gl_dc(HWND hwnd)
{
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;// 
	pfd.cStencilBits = 1;
	auto hdc = GetDC(hwnd);
	auto pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (!pixelFormat)
	{
		ReleaseDC(hwnd, hdc);
		throw runtime_error("Getting openGL DC failed.");
	}
	if (!SetPixelFormat(hdc, pixelFormat, &pfd))
	{
		ReleaseDC(hwnd, hdc);
		throw runtime_error("Getting openGL DC failed.");
	}
	return hdc;
}
#endif