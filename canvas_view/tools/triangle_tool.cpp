#include "StdAfx.h"
#include "windows/main_canvas.h"
#include "triangle_tool.h"

CTriangleToolUI::CTriangleToolUI() : m_iCursor(-1), m_uButtonState(0)
{
    m_ptClipBasePoint.x = m_ptClipBasePoint.y = 0;
    m_ptLastMouse.x = m_ptLastMouse.y = 0;
    pen_color_ = 0xffff0000;
    pen_size_ = 1;
    type_ = TRIANGLE;
    offset_y_ = 0;
}

LPCTSTR CTriangleToolUI::GetClass() const
{
    return DUI_CTR_TRIANGLETOOL;
}

LPVOID CTriangleToolUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_TRIANGLETOOL) == 0) return static_cast<CTriangleToolUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CTriangleToolUI::GetControlFlags() const
{
    return UIFLAG_SETCURSOR;
}

RECT CTriangleToolUI::GetSizerRect(int iIndex)
{
    LONG lMiddleX = (m_rcItem.left + m_rcItem.right) / 2;
    LONG lMiddleY = (m_rcItem.top + m_rcItem.bottom) / 2;
    LONG SIZER_WIDTH = m_rcInset.left * 2;
    switch (iIndex) {
    case 0:
        return CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left + SIZER_WIDTH, m_rcItem.top + SIZER_WIDTH);
    case 1:
        return CDuiRect(lMiddleX - SIZER_WIDTH / 2, m_rcItem.top, lMiddleX + SIZER_WIDTH / 2, m_rcItem.top + SIZER_WIDTH);
    case 2:
        return CDuiRect(m_rcItem.right - SIZER_WIDTH, m_rcItem.top, m_rcItem.right, m_rcItem.top + SIZER_WIDTH);
    case 3:
        return CDuiRect(m_rcItem.left, lMiddleY - SIZER_WIDTH / 2, m_rcItem.left + SIZER_WIDTH, lMiddleY + SIZER_WIDTH / 2);
    case 4:
        break;
    case 5:
        return CDuiRect(m_rcItem.right - SIZER_WIDTH, lMiddleY - SIZER_WIDTH / 2, m_rcItem.right, lMiddleY + SIZER_WIDTH / 2);
    case 6:
        return CDuiRect(m_rcItem.left, m_rcItem.bottom - SIZER_WIDTH, m_rcItem.left + SIZER_WIDTH, m_rcItem.bottom);
    case 7:
        return CDuiRect(lMiddleX - SIZER_WIDTH / 2, m_rcItem.bottom - SIZER_WIDTH, lMiddleX + SIZER_WIDTH / 2, m_rcItem.bottom);
    case 8:
        return CDuiRect(m_rcItem.right - SIZER_WIDTH, m_rcItem.bottom - SIZER_WIDTH, m_rcItem.right, m_rcItem.bottom);
    default:
        break;
    }
    return CDuiRect();
}

LPTSTR CTriangleToolUI::GetSizerCursor(POINT& pt, int& iCursor)
{  
    LONG SIZER_WIDTH = m_rcInset.left * 2;
    LONG SIZER_TO_ROOT = 20;
    RECT rcRoot = m_pManager->GetRoot()->GetPos();

    iCursor = -1;
    for (int i = 8; i >= 0; --i) {
        RECT rc = GetSizerRect(i);
        if (::PtInRect(&rc, pt)) {
            iCursor = i;
            break;
        }
    }
    if (iCursor < 0 && rcRoot.bottom - m_rcItem.bottom < SIZER_TO_ROOT) {
        RECT rc = CDuiRect(m_rcItem.left, m_rcItem.bottom - SIZER_WIDTH, m_rcItem.right, m_rcItem.bottom);
        if (::PtInRect(&rc, pt)) {
            iCursor = 7;
        }
    }
    if (iCursor < 0 && rcRoot.right - m_rcItem.right < SIZER_TO_ROOT) {
        RECT rc = CDuiRect(m_rcItem.right - SIZER_WIDTH, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
        if (::PtInRect(&rc, pt)) {
            iCursor = 5;
        }
    }
    if (iCursor < 0 && m_rcItem.left - rcRoot.left < SIZER_TO_ROOT) {
        RECT rc = CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left + SIZER_WIDTH, m_rcItem.bottom);
        if (::PtInRect(&rc, pt)) {
            iCursor = 3;
        }
    }
    if (iCursor < 0 && m_rcItem.top - rcRoot.top < SIZER_TO_ROOT) {
        RECT rc = CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.top + SIZER_WIDTH);
        if (::PtInRect(&rc, pt)) {
            iCursor = 1;
        }
    }
    if (iCursor < 0 ) iCursor = 4;
    switch (iCursor) {
    case 0:
        return IDC_SIZENWSE;
    case 1:
        return IDC_SIZENS;
    case 2:
        return IDC_SIZENESW;
    case 3:
        return IDC_SIZEWE;
    case 4:
        return IDC_SIZEALL;
    case 5:
        return IDC_SIZEWE;
    case 6:
        return IDC_SIZENESW;
    case 7:
        return IDC_SIZENS;
    case 8:
        return IDC_SIZENWSE;
    default:
        return IDC_SIZEALL;
    }
}

void CTriangleToolUI::DrawTriangle(HDC hDC)
{

    RECT rc = m_rcItem;
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;

    Gdiplus::Graphics cs(hDC);
    cs.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
    cs.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQuality);

    int offset = (pen_size_ + 1)*scale_ / 2;
    rc.left += offset;
    rc.top += offset;
    rc.right -= offset;
    rc.bottom -= offset;


    int pensize = pen_size_ * 2;
    if (pensize < 1)
        pensize = 1;
    Gdiplus::Pen pen(pen_color_, pensize);
    pen.SetDashStyle(Gdiplus::DashStyleSolid);

    
    pt_[0].X = rc.left;
    pt_[0].Y = rc.bottom;
    pt_[1].X = (rc.left + rc.right) / 2;
    pt_[1].Y = rc.top;
    pt_[2].X = rc.right;
    pt_[2].Y = rc.bottom;
    cs.DrawPolygon(&pen, pt_, 3);

}

void CTriangleToolUI::PaintBorder(HDC hDC)
{   

    RECT rc = m_rcItem;
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;
    //矩形
    //if (tools_status == TOOLS_NORMAL)
    {
        DrawTriangle(hDC);
      
    }
    if (tools_status_ == TOOLS_HOVER)
    {
        CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, m_dwBorderColor);
    }
    else if (tools_status_ == TOOLS_EDITABLE)
    {
        for (int i = 0; i < 9; ++i) {
            CRenderEngine::DrawColor(hDC, GetSizerRect(i), m_dwBorderColor);
        }
        CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, m_dwBorderColor);
    }

}

void CTriangleToolUI::DoEvent(TEventUI& event)
{
    if (event.Type == UIEVENT_SETCURSOR && IsEnabled())
    {   
        //非选中状态不改变光标
        if ((m_uButtonState & UISTATE_CAPTURED) != 0 )
            return;
        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(GetSizerCursor(event.ptMouse, m_iCursor))));
        return;
    }
    else if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
    {
        m_uButtonState |= UISTATE_CAPTURED;
        m_ptLastMouse = event.ptMouse;
        switch (m_iCursor) {
        case 0:
        case 3:
            m_ptClipBasePoint.x = m_rcItem.right;
            m_ptClipBasePoint.y = m_rcItem.bottom;
            break;
        case 1:
        case 2:
            m_ptClipBasePoint.x = m_rcItem.left;
            m_ptClipBasePoint.y = m_rcItem.bottom;
            break;

        case 5:
        case 8:
            m_ptClipBasePoint.x = m_rcItem.left;
            m_ptClipBasePoint.y = m_rcItem.top;
            break;
        case 6:
        case 7:
            m_ptClipBasePoint.x = m_rcItem.right;
            m_ptClipBasePoint.y = m_rcItem.top;
            break;
        default:
            m_ptClipBasePoint.x = m_rcItem.left;
            m_ptClipBasePoint.y = m_rcItem.top;
            break;
        }
        return;
    }
    if (event.Type == UIEVENT_BUTTONUP)
    {
        if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
            m_uButtonState &= ~UISTATE_CAPTURED;
        }
        return;
    }
    else if (event.Type == UIEVENT_MOUSEMOVE)
    {   
        //解决鼠标箭头的问题
        TEventUI event1 = event;
        event1.Type = UIEVENT_SETCURSOR;
        event1.dwTimestamp = ::GetTickCount();
        Event(event1);
        if ((m_uButtonState & UISTATE_CAPTURED) == 0 || m_iCursor<0) return;

        LONG cx = event.ptMouse.x - m_ptLastMouse.x;
        LONG cy = event.ptMouse.y - m_ptLastMouse.y;
        if (fabsl(cx) < 3 && fabsl(cy) < 3)
            return;
        m_ptLastMouse = event.ptMouse;
        RECT rcNewItem = m_rcItem;
        switch (m_iCursor) {
        case 0://左上
        case 2://右上
        case 6://左下
        case 8://右下
            rcNewItem.left = min(event.ptMouse.x, m_ptClipBasePoint.x); if (rcNewItem.left < 0) rcNewItem.left = 0;
            rcNewItem.top = min(event.ptMouse.y, m_ptClipBasePoint.y); if (rcNewItem.top < 0) rcNewItem.top = 0;
            rcNewItem.right = max(event.ptMouse.x, m_ptClipBasePoint.x); if (rcNewItem.right < 0) rcNewItem.right = 0;
            rcNewItem.bottom = max(event.ptMouse.y, m_ptClipBasePoint.y); if (rcNewItem.bottom < 0) rcNewItem.bottom = 0;
            SetPos(rcNewItem);
            break;
        case 1://上中
        case 7://下中
            rcNewItem.top = min(event.ptMouse.y, m_ptClipBasePoint.y); if (rcNewItem.top < 0) rcNewItem.top = 0;
            rcNewItem.bottom = max(event.ptMouse.y, m_ptClipBasePoint.y); if (rcNewItem.bottom < 0) rcNewItem.bottom = 0;
            SetPos(rcNewItem);
            break;
        case 3://左中
        case 5://右中
            rcNewItem.left = min(event.ptMouse.x, m_ptClipBasePoint.x); if (rcNewItem.left < 0) rcNewItem.left = 0;
            rcNewItem.right = max(event.ptMouse.x, m_ptClipBasePoint.x); if (rcNewItem.right < 0) rcNewItem.right = 0;
            SetPos(rcNewItem);
            break;
        default://中
            SetToolsOffset(cx, cy);
            break;
        }

    }
    else if (event.Type == UIEVENT_MOUSEENTER)
    {
    //    bHover_ = true;
    //    Invalidate();
    }
    else if (event.Type == UIEVENT_MOUSELEAVE)
    {
  //      bHover_ = false;
   //     Invalidate();
    }
    CContainerUI::DoEvent(event);
}


Json::Value CTriangleToolUI::Marshal()
{
    left_ = m_rcItem.left / scale_;
    top_ = m_rcItem.top / scale_ + offset_y_;
    right_ = m_rcItem.right / scale_;
    bottom_ = m_rcItem.bottom / scale_ + offset_y_;

    Json::Value tool = ToolBase::Marshal();

    Json::Value ext;
    Json::Value pt0;
    pt0["x"] = (int)(pt_[0].X / scale_);
    pt0["y"] = (int)(pt_[0].Y / scale_);
    ext["points"].append(pt0);

    Json::Value pt1;
    pt1["x"] = (int)(pt_[1].X / scale_);
    pt1["y"] = (int)(pt_[1].Y / scale_);
    ext["points"].append(pt1);

    Json::Value pt2;
    pt2["x"] = (int)(pt_[2].X / scale_);
    pt2["y"] = (int)(pt_[2].Y / scale_);
    ext["points"].append(pt2);

    tool["ext"] = ext;


    return tool;
}

bool CTriangleToolUI::UnMarshal(Json::Value &tool)
{
    if (!ToolBase::UnMarshal(tool))
        return false;

    RECT rect;
    rect.left = left_ * scale_;
    rect.top = (top_ - offset_y_) * scale_;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - offset_y_) * scale_;
    SetPos(rect);
    return true;
}

void CTriangleToolUI::SetToolsOffset(int offsetx, int offsety)
{
    RECT rect;
    rect = GetPos();
    rect.left += offsetx;
    rect.right += offsetx;
    rect.top += offsety;
    rect.bottom += offsety;
    SetPos(rect);

    left_ = m_rcItem.left / scale_;
    top_ = m_rcItem.top / scale_;
    right_ = m_rcItem.right / scale_;
    bottom_ = m_rcItem.bottom / scale_;
}

int CTriangleToolUI::ToolHitTest(POINT pt)
{   
    RECT rc = m_rcItem;
    if (!::PtInRect(&rc, pt))
    {
        return -1;
    }
    else
    {
        for (int i = 0; i < 9; ++i) {
            RECT rt = GetSizerRect(i);
            if (::PtInRect(&rt, pt))
                return i;
        }
        return 4;
    }

    ////精确度为4像素
    //static int ignore = 2;
    //RECT rc = m_rcItem;
    //rc.left -= ignore;
    //rc.top -= ignore;
    //rc.right += ignore;
    //rc.bottom += ignore;
    //if (!::PtInRect(&rc, pt))
    //{
    //    return false;
    //}
    //else
    //{
    //    if (tools_status_ == TOOLS_EDITABLE || tools_status_ == TOOLS_HOVER)
    //    {
    //        return true;
    //    }
    //}

    //POINT spt;
    //POINT ptS{ m_rcItem.left, m_rcItem.bottom }, ptM{ (m_rcItem.left + m_rcItem.right) / 2, m_rcItem.top }, ptE{ m_rcItem.right, m_rcItem.bottom };
    //double dis = 1000;
    //if (MapFun_Point2Line(ptS, ptM, pt, spt, &dis) == 0 && dis < ignore)
    //    return true;
    //if (MapFun_Point2Line(ptE, ptM, pt, spt, &dis) == 0 && dis < ignore)
    //    return true;
    //if (MapFun_Point2Line(ptS, ptE, pt, spt, &dis) == 0 && dis < ignore)
    //    return true;
    //return false;
}

void CTriangleToolUI::SetToolsState(ToolsStats status)
{
    tools_status_ = status;
    Invalidate();
}

void CTriangleToolUI::SetToolsScale(float scale)
{
    scale_ = scale;
    RECT rect;
    rect.left = left_ * scale_;
    rect.top = (top_ - offset_y_) * scale_;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - offset_y_) * scale_;
    SetPos(rect);

}

void CTriangleToolUI::SetOffsetY(int offset_y)
{
    offset_y_ = offset_y;
    RECT rect;

    rect.left = left_ * scale_;
    rect.top = (top_ - offset_y) * scale_;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - offset_y) * scale_;

    SetPos(rect);

    //ToolBase::SetOffsetY(offset_y);
    /*start_src_.x = start_.x*scale_;
    start_src_.y = start_.y*scale_ - basey;
    end_src_.x = end_.x*scale_;
    end_src_.y = end_.y*scale_ - basey;*/
}

CTriangleToolUI *CTriangleToolUI::CreateTools(DWORD color, int pen_size, float scale)
{
    CTriangleToolUI *pContainer = new CTriangleToolUI;
    pContainer->SetFloat(true);
    pContainer->SetPenColor(color);
    pContainer->SetPenSize(pen_size);
    pContainer->SetInset(RECT{3, 3, 3,3 });
    pContainer->SetBorderColor(0xFF2A89FF);
    pContainer->SetBorderSize(2);
    pContainer->SetMouseEnabled(false);
    pContainer->SetToolsScale( scale);

    return pContainer;
}




