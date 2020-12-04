#pragma once
#include "tool.h"

class CArrowToolUI : public CContainerUI,
     public ToolBase
{
public:
    CArrowToolUI();
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    // start:0    end:1    over:2

    RECT GetSizerRect(int iIndex);
    LPTSTR GetSizerCursor(POINT& pt, int& iCursor);
    void PaintBorder(HDC hDC);
    void DoEvent(TEventUI& event);

    void SetStartPoint(POINT start){ start_display_ = start; };
    void SetEndPoint(POINT end,bool flag=false);
    void CalculateVertexs(const Gdiplus::PointF &ptStart, const Gdiplus::PointF &ptEnd, double dArrowLen, double dArrowAngle, Gdiplus::PointF &ptVertex1, Gdiplus::PointF &ptVertex2);
    RECT GetSizerRect(POINT pt);  

public:
    void DrawHover(HDC hDC, DWORD color);

public: 

    Json::Value Marshal() override;

    bool UnMarshal( Json::Value& tool) override;


    void SetOffsetY(int basey) override;

    void SetToolsOffset(int offsetx, int offsety) override;

    int ToolHitTest(POINT pt) override;

    void SetToolsState(ToolsStats status) override;

    void SetToolsScale(float scale) override;


    static CArrowToolUI* CreateTools(DWORD pen_color, int pen_size, float scale=1.0);
protected:
    int m_iCursor;
    UINT m_uButtonState;
    POINT m_ptClipBasePoint; // 调整基准点
    POINT m_ptLastMouse; //


    POINT start_display_;
    POINT end_display_;

    POINT start_;
    POINT end_;
};
