#pragma once
#include "tool.h"
//#include <main_canvas.h>

class CPencileUI : 
    public CContainerUI,
    public ToolBase
{
public:
    CPencileUI();
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
    void AddPath(POINT from, POINT to);
    void AddPt(POINT pt);

    //fresh  是否刷新窗口 false 只是添加数据()  true 添加完毕刷新(针对上课实时添加)
    void AddWorldPt(POINT pt,bool fresh=false, int offsety=0, float scale=1.0);

    int  getNum(){ return vt_src_pt_.size(); }

    static CPencileUI *CreateTools(DWORD color = 0xffff0000, int pen_size = 1);
    
public:

    void SetOffsetY(int basey) override;
    void SetToolsScale(float scale) override;
    void SetToolsState(ToolsStats status) override;
    int ToolHitTest(POINT pt) override;
protected:
    int m_iCursor;
    UINT m_uButtonState;
    POINT m_ptClipBasePoint; // 调整基准点
    POINT m_ptLastMouse; // 
    Gdiplus::GraphicsPath graphics_path_;
    //屏幕坐标
    std::vector<POINT> vt_src_pt_;

    //世界坐标
    std::vector<POINT> vt_pt_;

    MainCanvas* main_canvas_;
};
