#include "StdAfx.h"
#include "windows/main_canvas.h"
#include "rectangle_tool.h"

CRectangleToolUI::CRectangleToolUI() : m_iCursor(-1), m_uButtonState(0)
{
    m_ptClipBasePoint.x = m_ptClipBasePoint.y = 0;
    m_ptLastMouse.x = m_ptLastMouse.y = 0;
    pen_color_ = 0xffff0000;
    pen_size_ = 1;
    type_ = RECTANGLE;
    offset_y_ = 0;
}

LPCTSTR CRectangleToolUI::GetClass() const
{
    return DUI_CTR_RECTANGLETOOL;
}

LPVOID CRectangleToolUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_RECTANGLETOOL) == 0) return static_cast<CRectangleToolUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CRectangleToolUI::GetControlFlags() const
{
    return UIFLAG_SETCURSOR;
}

RECT CRectangleToolUI::GetSizerRect(int iIndex)
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

LPTSTR CRectangleToolUI::GetSizerCursor(POINT& pt, int& iCursor)
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

void CRectangleToolUI::DoPaint(HDC hDC, const RECT& rcPaint)
{   

    RECT rcTemp = { 0 };
    if (!::IntersectRect(&rcTemp, &rcPaint, &m_rcItem)) return;

    RECT rc = m_rcItem;

    //CRenderEngine::DrawRect(hDC, rc, 1, 0xff00ff00);

    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;
    //矩形
    //if (tools_status == TOOLS_NORMAL)
    {

        Gdiplus::Graphics cs(hDC);
        cs.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
        cs.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQuality);

        int offset = (pen_size_ + 1)*scale_ / 2;
        //Pen pen(Gdiplus::Color(pen_color_), pen_size_*scale_);

        int pensize = pen_size_ * 2;
        if (pensize < 1)
            pensize = 1;
        Pen pen(Gdiplus::Color(pen_color_), pensize);
        cs.DrawRectangle(&pen, rc.left + offset, rc.top + offset, rc.right - rc.left - 2 * offset, rc.bottom - rc.top - 2 * offset);
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
    else if (tools_status_ == TOOLS_SELECTED)
    {
        CRenderEngine::DrawRect(hDC, rc, 3 /*m_rcBorderSize.left*/, 0xff00ffff);
    }

    //CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rc, L"Fuck me111111111111", pen_color_, 1, DT_CENTER | DT_SINGLELINE );


}

void CRectangleToolUI::DoEvent(TEventUI& event)
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
            //SetToolsOffset(cx, cy);
            break;
        }

    }
    else if (event.Type == UIEVENT_MOUSEENTER)
    {
        OutputDebugStringA("UIEVENT_MOUSEENTER\r\n");
    }
    else if (event.Type == UIEVENT_MOUSELEAVE)
    {
        OutputDebugStringA("UIEVENT_MOUSELEAVE\r\n");
    }

    CContainerUI::DoEvent(event);
}


Json::Value CRectangleToolUI::Marshal()
{
    left_ = m_rcItem.left / scale_;
    top_ = m_rcItem.top / scale_ + offset_y_;
    right_ = m_rcItem.right / scale_;
    bottom_ = m_rcItem.bottom / scale_ + offset_y_;

    Json::Value tool = ToolBase::Marshal();
    return tool;
}

bool CRectangleToolUI::UnMarshal(Json::Value &tool)
{
    if (!ToolBase::UnMarshal(tool))
        return false;

    RECT rect;
    rect.left = left_ * scale_;
    rect.top = (top_- offset_y_) * scale_ ;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - offset_y_) * scale_ ;
    SetPos(rect);
    return true;
}


void CRectangleToolUI::SetToolsOffset(int offsetx, int offsety)
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

int CRectangleToolUI::ToolHitTest(POINT pt)
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

    //确定边框
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;

    return MapFun_Point2Rect(rc, pt, 3);
}

void CRectangleToolUI::SetToolsState(ToolsStats status)
{
    tools_status_ = status;
    Invalidate();
}


void CRectangleToolUI::SetToolsScale(float scale)
{
    scale_ = scale;
    RECT rect;
    rect.left = left_ * scale_;
    rect.top = (top_ - offset_y_) * scale_;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - offset_y_) * scale_;
    SetPos(rect);
    
}

void CRectangleToolUI::SetOffsetY(int offset_y)
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



CRectangleToolUI *CRectangleToolUI::CreateTools(DWORD color, int pen_size, float scale)
{
    CRectangleToolUI *rectangle = new CRectangleToolUI;
    rectangle->SetFloat(true);
    rectangle->SetPenColor(color);
    rectangle->SetPenSize(pen_size);
    rectangle->SetInset(RECT{3, 3, 3,3 });
    rectangle->SetBorderColor(0xFF2A89FF);
    rectangle->SetBorderSize(2);
    rectangle->SetMouseEnabled(false);
    rectangle->SetToolsScale(scale);

    return rectangle;
}




#if 0
void CRectangleToolUI::ResetToolsMap(int basey, float scale, bool flag)
{
    /*if (flag)
    {
    RECT rect;
    rect.left = start_.x*scale;
    rect.top = start_.y*scale - basey;
    rect.right = end_.x * scale;
    rect.bottom = end_.y*scale - basey;
    scale_ = scale;
    SetPos(rect);
    }
    else
    {
    base_y_ = basey;
    start_.x = m_rcItem.left/scale;
    start_.y = (m_rcItem.top + basey )/ scale;
    end_.x = m_rcItem.right / scale;
    end_.y = (m_rcItem.bottom + basey) / scale;
    }*/




    /*RECT rect;
    rect.left = x_ * scale_;
    rect.top = y_ * scale_;
    rect.right = (x_ + cx_) * scale_;
    rect.bottom = (y_ + cy_) * scale_;
    SetPos(rect);*/



}

#endif
