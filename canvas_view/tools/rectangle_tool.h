#pragma once
#include "tool.h"

class CRectangleToolUI : public CContainerUI,
    public ToolBase
{
public:
    CRectangleToolUI();
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    // 0    1    2
    // 3    4    5
    // 6    7    8
    RECT GetSizerRect(int iIndex);
    LPTSTR GetSizerCursor(POINT& pt, int& iCursor);
    //void PaintBorder(HDC hDC);
    void DoPaint(HDC hDC, const RECT& rcPaint);
    void DoEvent(TEventUI& event);


public: //继承 ToolBase类的函数和属性
    Json::Value Marshal() override;
    bool UnMarshal( Json::Value &tool) override;

    void SetToolsOffset(int offsetx, int offsety) override;
    int ToolHitTest(POINT pt) override;
    void SetToolsState(ToolsStats status) override;
    void SetToolsScale(float scale) override;
    void SetOffsetY(int offset_y) override;

    static CRectangleToolUI *CreateTools(DWORD color = 0xffff0000, int pen_size = 1, float scale = 1.0);

protected:
    int m_iCursor;
    UINT m_uButtonState;
    POINT m_ptClipBasePoint; // 调整基准点
    POINT m_ptLastMouse; //
};
