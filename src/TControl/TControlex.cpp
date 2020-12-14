#pragma once

#include "TControl.h"

#include <stdio.h>//_vsnprintf_s
#include <assert.h>
#include <vector>
#include <tchar.h>
#include <shellapi.h>//HDROP

using namespace std;


TControl::~TControl()
{
}

TControl::TControl(const TControl& control)
{
	*this = control;
}

TControl& TControl::operator=(const TControl& control)
{
	TCHAR className[MAX_PATH];
	GetClassName(control.m_hWnd, className, MAX_PATH);

	LONG style = GetWindowLong(control.m_hWnd, GWL_STYLE);
	LONG exstyle = GetWindowLong(control.m_hWnd, GWL_EXSTYLE);
	LONG id = GetWindowLong(control.m_hWnd, GWL_ID);

	RECT rc = control.GetClientRect();
	RECT rcPos = control.GetPosition();

	m_hParent = control.m_hParent;
	m_hInst = control.m_hInst;
	m_hWnd = CreateWindowEx(exstyle,className, control.GetText().c_str(),
		style,rcPos.left,rcPos.top, rc.right, rc.bottom, 
		control.m_hParent, (HMENU)id, control.m_hInst, 0);
	
	HFONT hFont = (HFONT)SendMessage(control.m_hWnd, WM_GETFONT, 0, 0);
	SetFont(hFont);

	SetDragAccept(control.bCanAcceptDrag);
	return *this;
}



vector<tstring> TControl::PreDrop(WPARAM wParam) const
{
	vector<tstring> ret;
	HDROP hDrop = (HDROP)wParam;
	UINT nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); // 拖拽文件个数
	TCHAR strFileName[MAX_PATH];
	for (UINT i = 0; i < nFileNum; i++)
	{
		DragQueryFile(hDrop, i, strFileName, MAX_PATH);//获得拖曳的文件名
		ret.push_back(strFileName);
	}
	DragFinish(hDrop);      //释放hDrop
	return ret;
}

LRESULT TControl::DropProc(const std::vector<tstring>& dropFiles)
{
	return TRUE;
}


void TControl::LinkControl(HWND hDlg, int id)
{
	LinkControl(GetDlgItem(hDlg, id));
}

void TControl::SetFixedMargin(bool b, std::vector<bool> fixedMargin)
{
	assert(m_hWnd);
	assert(m_hParent);
	bAutoMargin = b;
	RECT rcParent;
	::GetClientRect(m_hParent, &rcParent);
	RECT rc;
	rc = GetWindowRect();
	POINT pt1{ rc.left,rc.top }, pt2{ rc.right,rc.bottom };
	ScreenToClient(m_hParent, &pt1);
	ScreenToClient(m_hParent, &pt2);
	margin.left = pt1.x - rcParent.left;
	margin.top = pt1.y - rcParent.top;
	margin.right = rcParent.right - pt2.x;
	margin.bottom = rcParent.bottom - pt2.y;

	//
	fixedWidth = pt2.x - pt1.x;
	fixedHeight = pt2.y - pt1.y;
	
	//
	assert(fixedMargin.size() == 4);
	this->fixedMarginOrientation = fixedMargin;
}

void TControl::OnSize()
{
	RECT rcParent;
	::GetClientRect(m_hParent, &rcParent);
	RECT rc;
	if (fixedMarginOrientation[0])//left
		rc.left = margin.left;
	else
		rc.left = rcParent.right - margin.right - fixedWidth;

	if (fixedMarginOrientation[1])//top
		rc.top = margin.top;
	else
		rc.top = rcParent.bottom - margin.bottom - fixedHeight;

	if (fixedMarginOrientation[2])//right
		rc.right = rcParent.right - margin.right;
	else
		rc.right = margin.left + fixedWidth;

	if (fixedMarginOrientation[3])//bottom
		rc.bottom = rcParent.bottom - margin.bottom;
	else
		rc.bottom = margin.top + fixedHeight;
	SetPosition(rc);

	AfterOnSize();
}

void TControl::LinkControl(HWND hWndCtrl)//链接到已有控件（用于对话框中）
{
	assert(hWndCtrl);
	m_hInst = GetModuleHandle(NULL);
	m_hParent = GetParent(hWndCtrl);
	m_hWnd = hWndCtrl;

	//测试发现subControlProc中会反复处理 uMsg=0x87, WM_GETDLGCODE 消息，
	//查询 https://blog.csdn.net/amwfnyq/article/details/5612289 描述是控件
	//没有 WS_EX_CONTROLPARENT 样式导致死循环
	//LONG style = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	//SetWindowLong(m_hWnd, GWL_EXSTYLE, style | WS_EX_CONTROLPARENT);

	//再次测试发现是两个HWND注册到同一个class导致死循环

	RegisterProc();

	DoAfterLinkControl();
}


LRESULT TControl::WndProc(WNDPROC wndproc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)//虚拟消息处理函数，可重载
{
	auto it = msgDealer.find(uMsg);
	if (it != msgDealer.end())
	{
		it->second(this,hWnd,wParam,lParam);
	}
	return CallWindowProc(wndproc, hWnd, uMsg, wParam, lParam);
}

int TControl::GetID()
{
	return GetDlgCtrlID(m_hWnd);
}


void TControl::SetDragAccept(bool bCanAcceptDrop)
{
	this->bCanAcceptDrag = bCanAcceptDrop;
	DragAcceptFiles(m_hWnd, bCanAcceptDrop);
}

void TControl::SetID(int id)
{
	SetWindowLong(m_hWnd, GWL_ID, id);
}