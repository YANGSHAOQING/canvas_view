#pragma once
#include "tool.h"

class CSelectRectToolUI : public CContainerUI
{
public:
    CSelectRectToolUI();
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    // 0    1    2
    // 3    4    5
    // 6    7    8
    RECT GetSizerRect(int iIndex);
    LPTSTR GetSizerCursor(POINT& pt, int& iCursor);
    void PaintBorder(HDC hDC);
    void DoEvent(TEventUI& event);
    void SetPenColor(DWORD color){ color_ = color; }
    void SetPenSize(int size);

protected:
    int m_iCursor;
    UINT m_uButtonState;
    POINT m_ptClipBasePoint; // 调整基准点
    POINT m_ptLastMouse; //
    DWORD color_;
    int pen_size_;
    bool m_bMouseOver;
    RECT m_org_rect_;

    MainCanvas* main_canvas_;
};
