#include "StdAfx.h"
#include "image_tool.h"
#include <windows/main_canvas.h>

CImageToolUI::CImageToolUI() : m_iCursor(-1), m_uButtonState(0)
{
    m_ptClipBasePoint.x = m_ptClipBasePoint.y = 0;
    m_ptLastMouse.x = m_ptLastMouse.y = 0;
    type_ = IMAGE;

}

LPCTSTR CImageToolUI::GetClass() const
{
    return DUI_CTR_IMAGETOOL;
}

LPVOID CImageToolUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_IMAGETOOL) == 0) return static_cast<CImageToolUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CImageToolUI::GetControlFlags() const
{
    return UIFLAG_SETCURSOR;
}

RECT CImageToolUI::GetSizerRect(int iIndex)
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

LPTSTR CImageToolUI::GetSizerCursor(POINT& pt, int& iCursor)
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

void CImageToolUI::PaintBorder(HDC hDC) {
    /*for (int i = 0; i < 9; ++i) {
        CRenderEngine::DrawColor(hDC, GetSizerRect(i), m_dwBorderColor);
    }*/

    //矩形
    RECT rc = m_rcItem;
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;
    //CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, m_dwBorderColor);
    
    if (tools_status_ == TOOLS_HOVER)
    {
        CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, 0xff00ffff);
    }
    else if (tools_status_ == TOOLS_EDITABLE)
    {
        /*for (int i = 0; i < 9; ++i) {
            CRenderEngine::DrawColor(hDC, GetSizerRect(i), m_dwBorderColor);
            }*/

        CRenderEngine::DrawColor(hDC, GetSizerRect(0), m_dwBorderColor);
        CRenderEngine::DrawColor(hDC, GetSizerRect(2), m_dwBorderColor);
        CRenderEngine::DrawColor(hDC, GetSizerRect(6), m_dwBorderColor);
        CRenderEngine::DrawColor(hDC, GetSizerRect(8), m_dwBorderColor);

        CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, m_dwBorderColor);
    }


 


}

void CImageToolUI::DoEvent(TEventUI& event)
{
    if (event.Type == UIEVENT_SETCURSOR && IsEnabled())
    {
        if ((m_uButtonState & UISTATE_CAPTURED) != 0)
            return;
        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(GetSizerCursor(event.ptMouse, m_iCursor))));
        return;
    }
    else if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
    {
        m_uButtonState |= UISTATE_CAPTURED;
        m_ptLastMouse = event.ptMouse;
        m_org_rect_ = m_rcItem;
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

        if ((m_uButtonState & UISTATE_CAPTURED) == 0) return;
        //RECT rcWindow = main_canvas_->GetWindowRect();
        RECT rcNewItem = m_rcItem;
        switch (m_iCursor) {
        case 0://左上
        case 2://右上
        case 6://左下
        case 8://右下
            rcNewItem.left = min(event.ptMouse.x, m_ptClipBasePoint.x); if (rcNewItem.left < 0) rcNewItem.left = 0;
            rcNewItem.top = min(event.ptMouse.y, m_ptClipBasePoint.y); if (rcNewItem.top < 0) rcNewItem.top = 0;
            rcNewItem.right = max(event.ptMouse.x, m_ptClipBasePoint.x); if (rcNewItem.right < 0) rcNewItem.right = 0;
            rcNewItem.bottom =  max(event.ptMouse.y, m_ptClipBasePoint.y); if (rcNewItem.bottom < 0) rcNewItem.bottom = 0;
            break;
        default://中
        {
            LONG cx = event.ptMouse.x - m_ptLastMouse.x;
            LONG cy = event.ptMouse.y - m_ptLastMouse.y;

            rcNewItem.left = m_org_rect_.left + cx;
            rcNewItem.right = m_org_rect_.right + cx;
            rcNewItem.top = m_org_rect_.top + cy;
            rcNewItem.bottom = m_org_rect_.bottom + cy;

            /*if (rcNewItem.left < rcWindow.left)
                rcNewItem.left = rcWindow.left;
                if (rcNewItem.right > rcWindow.right)
                rcNewItem.right = rcWindow.right;*/
        }
        break;
        }
        SetPos(rcNewItem);
    }
    else if (event.Type == UIEVENT_MOUSEENTER)
    {
   //     m_bMouseOver = true;
  //      Invalidate();
    }
    else if (event.Type == UIEVENT_MOUSELEAVE)
    {
    //    m_bMouseOver = false;
    //    Invalidate();
    }
    CContainerUI::DoEvent(event);
}


void CImageToolUI::SetToolsOffset(int offsetx, int offsety)
{
    RECT rect = m_rcItem;
    rect.left += offsetx;
    rect.right += offsetx;
    rect.top += offsety;
    rect.bottom += offsety;
    SetPos(rect);
}

Json::Value CImageToolUI::Marshal()
{   
    Json::Value tool;
    tool["uuid"] = control_uuid_;
    tool["x1"] = to_string(base_pos_.x);
    tool["y1"] = to_string(base_pos_.y);
    tool["x2"] = to_string(image_size_.cx);
    tool["y2"] = to_string(image_size_.cy);
    return tool;
}

bool CImageToolUI::UnMarshal(Json::Value &tool)
{

    if (tool["x1"].isNull() || tool["y1"].isNull()
        || tool["x2"].isNull() || tool["y2"].isNull())
        return false;
    /*if (type == 0 && !tool["uuid"].isNull())
    {
    control_uuid_ = tool["uuid"].asString();
    }*/
    base_pos_.x = tool["x1"].asInt();
    base_pos_.y = tool["y1"].asInt();
    image_size_.cx = tool["x2"].asInt();
    image_size_.cy = tool["y2"].asInt();
    return true;
}


void CImageToolUI::Setfilepath(std::wstring filepath)
{ 
    filepath_ = filepath; 
  //  int width=0, height=0;
  //  if (!GetImageSize(filepath, width, height))
   //     return ;
  //  SetImageSize(SIZE{ width, height });
    SetBkImage(filepath.c_str());
}

void CImageToolUI::SetToolsState(ToolsStats status)
{
    tools_status_ = status;
    Invalidate();
}

void CImageToolUI::SetToolsScale(float scale)
{
    scale_ = scale;
    RECT rect;
    rect.left = left_ * scale_;
    rect.top = top_ * scale_;
    rect.right = right_ * scale_;
    rect.bottom = bottom_ * scale_;
    SetPos(rect);

}

void CImageToolUI::SetOffsetY(int offset_y)
{
    //ToolBase::SetOffsetY(offset_y);
    /*start_src_.x = start_.x*scale_;
    start_src_.y = start_.y*scale_ - basey;
    end_src_.x = end_.x*scale_;
    end_src_.y = end_.y*scale_ - basey;*/
}

CImageToolUI*  CImageToolUI::CreateTools()
{
    CImageToolUI *pImagetool = new CImageToolUI;
    pImagetool->SetFloat(true);
    pImagetool->SetToolsType(IMAGE);
    pImagetool->SetInset(RECT{ 2, 2, 2, 2 });
    pImagetool->SetBorderColor(0xFFFF00FF);
    pImagetool->SetBorderSize(2);
    pImagetool->SetMouseEnabled(false);
    pImagetool->SetBasePos(POINT{0,0});
    pImagetool->SetToolsScale(1.0); 
    return pImagetool;
}

int CImageToolUI::ToolHitTest(POINT pt)
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

    //return MapFun_Point2Rect(m_rcItem, pt, 10);
}

