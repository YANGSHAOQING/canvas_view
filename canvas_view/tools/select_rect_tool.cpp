#include "StdAfx.h"
#include "windows/main_canvas.h"
#include "select_rect_tool.h"

CSelectRectToolUI::CSelectRectToolUI() : m_iCursor(0), m_uButtonState(0)
{
    m_ptClipBasePoint.x = m_ptClipBasePoint.y = 0;
    m_ptLastMouse.x = m_ptLastMouse.y = 0;
    color_ = 0XFF2A89FF;
    pen_size_ = 1;
    m_bMouseOver = false;
}

LPCTSTR CSelectRectToolUI::GetClass() const
{
    return DUI_CTR_SELECTTOOL;
}

LPVOID CSelectRectToolUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_SELECTTOOL) == 0) return static_cast<CSelectRectToolUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CSelectRectToolUI::GetControlFlags() const
{
    return UIFLAG_SETCURSOR;
}

RECT CSelectRectToolUI::GetSizerRect(int iIndex)
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

LPTSTR CSelectRectToolUI::GetSizerCursor(POINT& pt, int& iCursor)
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
    if (iCursor < 0) iCursor = 4;
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

void CSelectRectToolUI::PaintBorder(HDC hDC) {
    /*for (int i = 0; i < 9; ++i) {
        CRenderEngine::DrawColor(hDC, GetSizerRect(i), m_dwBorderColor);
        }*/

    //矩形
    RECT rc = m_rcItem;
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;

    Gdiplus::Graphics cs(hDC);
    cs.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
    cs.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQuality);

    int offset = (pen_size_ + 1) / 2;
    Pen pen(Gdiplus::Color(color_), pen_size_);
    //pen.SetDashStyle(DashStyle::DashStyleDashDot);
    //cs.DrawRectangle(&pen, rc.left + offset, rc.top + offset, rc.right - rc.left - 2 * offset, rc.bottom - rc.top - 2 * offset);
    Gdiplus::SolidBrush bk_brush(Gdiplus::Color(25, 42, 137, 255));
    cs.DrawRectangle(&pen, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    cs.FillRectangle(&bk_brush, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);

}

void CSelectRectToolUI::DoEvent(TEventUI& event)
{
    if (event.Type == UIEVENT_SETCURSOR && IsEnabled())
    {
        if ((m_uButtonState & UISTATE_CAPTURED) != 0)
            return;
        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(GetSizerCursor(event.ptMouse, m_iCursor))));
        return;
    }
    if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
    {
        m_uButtonState |= UISTATE_CAPTURED;
        m_ptLastMouse = event.ptMouse;
        RECT rcWindow = main_canvas_->GetWindowRect();
        m_org_rect_ = m_rcItem;
        switch (m_iCursor) {
        case 0:
        case 1:
        case 3:
            m_ptClipBasePoint.x = m_rcItem.left;
            m_ptClipBasePoint.y = rcWindow.bottom - m_rcItem.bottom;
            break;
        case 2:
        case 5:
            m_ptClipBasePoint.x = m_rcItem.left;
            m_ptClipBasePoint.y = rcWindow.bottom - m_rcItem.bottom;
            break;
        case 6:
        case 7:
            m_ptClipBasePoint.x = rcWindow.right - rcWindow.left - m_rcItem.right;
            m_ptClipBasePoint.y = m_rcItem.top;
            break;
        case 8:
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

        if ((m_uButtonState & UISTATE_CAPTURED) == 0) return;
        RECT rcWindow = main_canvas_->GetWindowRect();
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
            break;
        case 1://上中
        case 7://下中
            rcNewItem.top = min(event.ptMouse.y, m_ptClipBasePoint.y); if (rcNewItem.top < 0) rcNewItem.top = 0;
            rcNewItem.bottom = max(event.ptMouse.y, m_ptClipBasePoint.y); if (rcNewItem.bottom < 0) rcNewItem.bottom = 0;
            break;
        case 3://左中
        case 5://右中
            rcNewItem.left = min(event.ptMouse.x, m_ptClipBasePoint.x); if (rcNewItem.left < 0) rcNewItem.left = 0;
            rcNewItem.right = max(event.ptMouse.x, m_ptClipBasePoint.x); if (rcNewItem.right < 0) rcNewItem.right = 0;
            break;
        default://中
        {
            LONG cx = event.ptMouse.x - m_ptLastMouse.x;
            LONG cy = event.ptMouse.y - m_ptLastMouse.y;


            rcNewItem.left = m_org_rect_.left + cx;
            rcNewItem.right = m_org_rect_.right + cx;
            rcNewItem.top = m_org_rect_.top + cy;
            rcNewItem.bottom = m_org_rect_.bottom + cy;

            if (rcNewItem.left < rcWindow.left)
                rcNewItem.left = rcWindow.left;
            if (rcNewItem.right > rcWindow.right)
                rcNewItem.right = rcWindow.right;


        }
        break;
        }
        SetPos(rcNewItem);
    }
    else if (event.Type == UIEVENT_MOUSEENTER)
    {
        m_bMouseOver = true;
        Invalidate();
    }
    else if (event.Type == UIEVENT_MOUSELEAVE)
    {
        m_bMouseOver = false;
        Invalidate();
    }
    CContainerUI::DoEvent(event);
}

void CSelectRectToolUI::SetPenSize(int size)
{
    if (size == 0)
    {
        pen_size_ = 1;
    }
    else if (size == 1)
    {
        pen_size_ = 3;
    }
    else
    {
        pen_size_ = 5;
    }
}


