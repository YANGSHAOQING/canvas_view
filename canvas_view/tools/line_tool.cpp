#include "StdAfx.h"
#include "windows/main_canvas.h"
#include "line_tool.h"

CLineToolUI::CLineToolUI() : m_iCursor(-1), m_uButtonState(0)
{
    m_ptClipBasePoint.x = m_ptClipBasePoint.y = 0;
    m_ptLastMouse.x = m_ptLastMouse.y = 0;
    pen_color_ = 0xffff0000;
    pen_size_ = 1;
    type_ = LINE;
    offset_y_ = 0;
}

LPCTSTR CLineToolUI::GetClass() const
{
    return DUI_CTR_LINETOOL;
}

LPVOID CLineToolUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_LINETOOL) == 0) return static_cast<CLineToolUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CLineToolUI::GetControlFlags() const
{
    return UIFLAG_SETCURSOR;
}

RECT CLineToolUI::GetSizerRect(int iIndex)
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

RECT CLineToolUI::GetSizerRect(POINT pt)
{
    LONG SIZER_WIDTH = m_rcInset.left;
    return CDuiRect(pt.x - SIZER_WIDTH, pt.y - SIZER_WIDTH, pt.x + SIZER_WIDTH, pt.y + SIZER_WIDTH);
}


LPTSTR CLineToolUI::GetSizerCursor(POINT& pt, int& iCursor)
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

void CLineToolUI::SetEndPoint(POINT end, bool flag)
{ 
    end_display_.x = end.x;
    end_display_.y = end.y; 
    if (flag)
    {
        Gdiplus::PointF to = { (float)end_display_.x, (float)end_display_.y };
        Gdiplus::PointF from = { (float)start_display_.x, (float)start_display_.y };

        RECT rect;
             
        rect.left = rect.right = start_display_.x;
        rect.top = rect.bottom = start_display_.y;
        //得到最小值
        if (rect.left > end_display_.x)rect.left = end_display_.x;
        if (rect.right < end_display_.x)  rect.right = end_display_.x;

        if (rect.top > end_display_.y)rect.top = end_display_.y;
        if (rect.bottom < end_display_.y)  rect.bottom = end_display_.y;

        rect.left -= 3;
        rect.right += 3;
        rect.top -= 3;
        rect.bottom += 3;

        SetPos(rect);
    }

}

void CLineToolUI::PaintBorder(HDC hDC) {
    /*for (int i = 0; i < 9; ++i) {
        CRenderEngine::DrawColor(hDC, GetSizerRect(i), m_dwBorderColor);
        }*/

    //矩形
    RECT rc = m_rcItem;

    //CRenderEngine::DrawRect(hDC, rc, 1, 0xff00ff00);
    
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;

  
    //CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, m_dwBorderColor);
    //CRenderEngine::DrawRect(hDC, rc, 1, m_dwBorderColor);

    //箭头
    Graphics cs(hDC);
    cs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    cs.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
    Gdiplus::SolidBrush brush(Gdiplus::Color((ARGB)pen_color_));
    

    Gdiplus::PointF to = { (float)end_display_.x, (float)end_display_.y };
    Gdiplus::PointF from = { (float)start_display_.x, (float)start_display_.y };

    int pensize = pen_size_ * 2;
    if (pensize < 1)
        pensize = 1;

    Gdiplus::Pen pen(&brush, pensize);
    pen.SetDashStyle(Gdiplus::DashStyleSolid);
    pen.SetEndCap(Gdiplus::LineCapRound);
    cs.DrawLine(&pen, from.X, from.Y, to.X, to.Y);

    if (tools_status_==TOOLS_HOVER)
    {
        DrawHover(hDC, m_dwBorderColor);
    }
    else if (tools_status_ == TOOLS_EDITABLE)
    {   
        //DrawHover(hDC,m_dwBorderColor);
        CRenderEngine::DrawColor(hDC, GetSizerRect(end_display_), m_dwBorderColor);
        CRenderEngine::DrawColor(hDC, GetSizerRect(start_display_), m_dwBorderColor);

   
    }

    //if (tools_status_ == TOOLS_HOVER)
    //{
    //    CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, 0xff00ffff);
    //}
    //else if (tools_status_ == TOOLS_EDITABLE)
    //{
    //    for (int i = 0; i < 9; ++i) {
    //        CRenderEngine::DrawColor(hDC, GetSizerRect(i), m_dwBorderColor);
    //    }
    //    CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, m_dwBorderColor);
    //}
    //else if (tools_status_ == TOOLS_SELECTED)
    //{
    //    CRenderEngine::DrawRect(hDC, rc, 3 /*m_rcBorderSize.left*/, 0xff00ffff);
    //}

}

void CLineToolUI::DoEvent(TEventUI& event)
{   
    if (event.Type == UIEVENT_SETCURSOR && IsEnabled())
    {
        if ((m_uButtonState & UISTATE_CAPTURED) != 0)
            return;

        RECT rc_start = GetSizerRect(start_display_);
        RECT rc_end = GetSizerRect(end_display_);
        if (::PtInRect(&rc_start, event.ptMouse)) {
            m_iCursor = 0;
        }
        else if (::PtInRect(&rc_end, event.ptMouse)) {
            m_iCursor = 1;
        }
        else if (ToolHitTest(event.ptMouse) != -1)
        {
           m_iCursor = 2;
        }
        else
        {
            m_iCursor = -1;
        }
        if (m_iCursor>-1)
           ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL)));
        else
            ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
        return;
    }

    if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
    {
        m_uButtonState |= UISTATE_CAPTURED;
        m_ptLastMouse = event.ptMouse;

#if 0
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

#endif
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
        switch (m_iCursor) {
        case 0://起始点
            start_display_ = event.ptMouse;
            SetEndPoint(end_display_, true);
            break;
        case 1://终点
            end_display_ = event.ptMouse;
            SetEndPoint(end_display_,true);
            break;
        
        default://中
             //SetToolsOffset(cx, cy);
            break;
        }
    }
    else if (event.Type == UIEVENT_MOUSEENTER)
    {
  //      m_bMouseOver = true;
  //      Invalidate();
    }
    else if (event.Type == UIEVENT_MOUSELEAVE)
    {
  //      m_bMouseOver = false;
    //    Invalidate();
    }
    CContainerUI::DoEvent(event);
}

void CLineToolUI::SetOffsetY(int basey)
{
    offset_y_ = basey;
    RECT rect;
    rect.left = left_ * scale_;
    rect.top = (top_ - basey) * scale_;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - basey) * scale_;
    SetPos(rect);

    start_display_.y = (start_.y - basey) * scale_;
    end_display_.y = (end_.y - basey) * scale_;

    //start_.y = start_.y - (basey / scale_);
    //end_.y = end_.y - (basey / scale_);
    //ToolBase::SetOffsetY(basey);
    /*start_src_.x = start_.x*scale_;
    start_src_.y = start_.y*scale_ - basey;
    end_src_.x = end_.x*scale_;
    end_src_.y = end_.y*scale_ - basey;*/
}
#if 0
void CArrowToolUI::ResetToolsMap(int basey, float scale, bool flag)
{
    /*if (flag)
    {    
        start_src_.x = start_.x*scale;
        start_src_.y = start_.y*scale - basey;
        end_src_.x =  end_.x*scale ;
        end_src_.y =  end_.y*scale - basey;
        scale_ = scale;
        SetEndPoint(end_src_, true);
    }
    else
    {  

        start_.x = start_src_.x/scale ;
        start_.y =  (start_src_.y + basey)/scale;
        end_.x = end_src_.x/scale ;
        end_.y = (end_src_.y + basey)/scale;
    }*/
}
#endif
void CLineToolUI::SetToolsOffset(int offsetx, int offsety)
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


    start_display_.x += offsetx;
    start_display_.y += offsety;
    end_display_.x += offsetx;
    end_display_.y += offsety;

    start_.x += (offsetx / scale_);
    start_.y += (offsety / scale_);
    end_.x += (offsetx / scale_);
    end_.y += (offsety / scale_);

}

int CLineToolUI::ToolHitTest(POINT pt)
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
    /*else if (tools_status_ == TOOLS_EDITABLE || tools_status_ == TOOLS_HOVER)
    {
        return true;
    }
    POINT spt;
    double dis = 1000;
    if (MapFun_Point2Line(start_src_, end_src_, pt, spt, &dis) == 0 && dis < 2)
        return true;*/
}

void CLineToolUI::DrawHover(HDC hDC,DWORD color)
{
      
    Gdiplus::Pen ePen(Gdiplus::Color((ARGB)color), 2);
    ePen.SetDashStyle(Gdiplus::DashStyleCustom);
    Gdiplus::REAL dashVals[4] = {
        2, // 短划长为2  
        3, // 间隔为2  
        2, // 短划长为15  
        3   // 间隔为2
    };
    ePen.SetDashPattern(dashVals, 3);
    
    Graphics cs(hDC);
    cs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    cs.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);

    Gdiplus::PointF to = { (float)end_display_.x, (float)end_display_.y };
    Gdiplus::PointF from = { (float)start_display_.x, (float)start_display_.y };

    cs.DrawLine(&ePen, start_display_.x, start_display_.y, end_display_.x, end_display_.y);
   
}

Json::Value CLineToolUI::Marshal()
{
    left_ = m_rcItem.left / scale_;
    top_ = m_rcItem.top / scale_ + offset_y_;
    right_ = m_rcItem.right / scale_;
    bottom_ = m_rcItem.bottom / scale_ + offset_y_;

    start_.x = start_display_.x / scale_;
    start_.y = start_display_.y / scale_;
    end_.x = end_display_.x / scale_;
    end_.y = end_display_.y / scale_;

    Json::Value tool = ToolBase::Marshal();

    Json::Value ext;

    ext["start_x"] = start_.x;
    ext["start_y"] = start_.y;
    ext["end_x"] = end_.x;
    ext["end_y"] = end_.y;

    tool["ext"] = ext;
    
    return tool;
   
}

bool CLineToolUI::UnMarshal(Json::Value &tool)
{
    if (!ToolBase::UnMarshal(tool))
        return false;

    RECT rect;
    rect.left = left_ * scale_;
    rect.top = (top_ - offset_y_) * scale_;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - offset_y_) * scale_;
    SetPos(rect);

    Json::Value ext = tool["ext"];

    start_.x = ext["start_x"].asInt();
    start_.y = ext["start_y"].asInt();
    end_.x = ext["end_x"].asInt();
    end_.y = ext["end_y"].asInt();


    start_display_.x = start_.x * scale_;
    start_display_.y = start_.y * scale_;
    end_display_.x = end_.x * scale_;
    end_display_.y = end_.y * scale_;

    return true;
}

void CLineToolUI::SetToolsState(ToolsStats status)
{
    tools_status_ = status;
    Invalidate();
}

void CLineToolUI::SetToolsScale(float scale)
{
    scale_ = scale;
    RECT rect;
    rect.left = left_ * scale_;
    rect.top = (top_ - offset_y_) * scale_;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - offset_y_) * scale_;
    SetPos(rect);

    start_display_.x = start_.x * scale_;
    start_display_.y = (start_.y - offset_y_)* scale_;
    end_display_.x = end_.x * scale_;
    end_display_.y = (end_.y - offset_y_)* scale_;
}



CLineToolUI* CLineToolUI::CreateTools(DWORD pen_color, int pen_size, float scale)
{
    CLineToolUI* arrow_tool = new CLineToolUI;
    arrow_tool->SetPenColor(pen_color);
    arrow_tool->SetPenSize(pen_size);
    arrow_tool->SetInset(RECT{ 3, 3, 3, 3 });
    arrow_tool->SetFloat(true);
    arrow_tool->SetBorderColor(0xFF2A89FF);
    arrow_tool->SetBorderSize(2);
    arrow_tool->SetMouseEnabled(false);
    arrow_tool->SetToolsScale(scale);

    return arrow_tool;
;
}
