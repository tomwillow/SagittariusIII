#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include "tstring.h"

class TControl
{
private:
	//�������Ϊtrue��֮���ڸ�����OnSize�е�������OnSize������ʵ�ֿؼ�marginʼ��һ��
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


	//����ʱ��ȡ�õ�ǰmargin�洢
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

	virtual LRESULT WndProc(WNDPROC wndproc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//������Ϣ���������ɸ���

	virtual LRESULT DropProc(const std::vector<std::tstring>& dropFiles);

	virtual void DoAfterLinkControl() {}
};