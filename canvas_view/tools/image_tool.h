#pragma once
#include "tool.h"

class CImageToolUI : public CContainerUI,
    public ToolBase
{
public:
    CImageToolUI();
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
    void Setfilepath(std::wstring filepath);
    void SetPicUrl(std::string url){ url_ = url; }

    void SetBasePos(POINT pt){ base_pos_ = pt;}
    void SetImageSize(SIZE size){ image_size_ = size; }
    int ToolHitTest(POINT pt)override;

    void SetToolsState(ToolsStats status) override;
    void SetToolsScale(float scale) override;
    void SetOffsetY(int offset_y) override;

    static  CImageToolUI* CreateTools();

 
public:
    //针对移动
    Json::Value Marshal() override;
    bool UnMarshal(Json::Value &tool) override;
    
    void SetToolsOffset(int offsetx, int offsety) override;
protected:
    int m_iCursor;
    UINT m_uButtonState;
    POINT m_ptClipBasePoint; // 调整基准点
    POINT m_ptLastMouse; // 

    RECT m_org_rect_;
public:
    POINT base_pos_;
    SIZE  image_size_;
    std::wstring filepath_;
    std::string  url_;

};
