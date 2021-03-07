#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include "tstring.h"

class TControl
{
private:
	//如果设置为true，之后在父窗口OnSize中调用自身OnSize，可以实现控件margin始终一致
	bool bAutoMargin;
	int fixedWidth, fixedHeight;
	std::vector<bool> fixedMarginOrientation;
	RECT margin;
	virtual void AfterOnSize() {}

	bool bCanAcceptDrag;

	std::vector<std::tstring> PreDrop(WPARAM wParam) const;

	friend class TFixedMarginLayout;
public:
	TControl(const TControl& control);
	TControl& operator=(const TControl& control);


	//调用时将取得当前margin存储
	//fixedMargin: left, top, right, bottom
	void SetFixedMargin(bool b = true, std::vector<bool> fixedMargin = { true,true,true,true });
	void OnSize();

	void SetID(int id);

	void SetDragAccept(bool bCanAcceptDrop);

protected:
	HWND m_hParent;
	HWND m_hWnd;
	HINSTANCE m_hInst;
	TControl() :m_hWnd(0), m_hParent(0), m_hInst(0), bCanAcceptDrag(false), bAutoMargin(false), margin({ 0 }),
		fixedWidth(100),fixedHeight(100)
	{}
	virtual ~TControl();

	virtual LRESULT WndProc(WNDPROC wndproc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//虚拟消息处理函数，可覆盖

	virtual LRESULT DropProc(const std::vector<std::tstring>& dropFiles);

	virtual void DoAfterLinkControl() {}
};