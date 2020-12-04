#pragma once

class CCanvasContainerUI : public CContainerUI
{
public:
    CCanvasContainerUI();
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

    RECT GetSizerOutRect(RECT rect);
    //void SetPos(RECT rc);

protected:
    int m_iCursor;
    UINT m_uButtonState;
    POINT m_ptClipBasePoint; // 调整基准点
    POINT m_ptLastMouse; // 
    RECT rcClipPaddingTmp;
    int width_;
    int height_;
    //响应区
    int border_size_;
};
