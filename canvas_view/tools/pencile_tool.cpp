#include "stdafx.h"
#include "pencile_tool.h"
#include "windows/main_canvas.h"

CPencileUI::CPencileUI() : m_iCursor(-1), m_uButtonState(0)
{
    m_ptClipBasePoint.x = m_ptClipBasePoint.y = 0;
    m_ptLastMouse.x = m_ptLastMouse.y = 0;
    pen_color_ = 0xffff0000;
    pen_size_ = 2;
    type_ = PENCILE;
}

LPCTSTR CPencileUI::GetClass() const
{
    return DUI_CTR_PENCILETOOL;
}

LPVOID CPencileUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_PENCILETOOL) == 0) return static_cast<CPencileUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CPencileUI::GetControlFlags() const
{
    return UIFLAG_SETCURSOR;
}

RECT CPencileUI::GetSizerRect(int iIndex)
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

LPTSTR CPencileUI::GetSizerCursor(POINT& pt, int& iCursor)
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



void CPencileUI::PaintBorder(HDC hDC) {
    
    if (vt_src_pt_.size() < 2)
        return;
    //¾ØÐÎ
    RECT rc = m_rcItem;
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;

    Graphics cs(hDC);
    cs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    cs.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
   
    int pensize = pen_size_*scale_;
    if (pensize < 1)
        pensize = 1;
    Pen pen(Gdiplus::Color(pen_color_), pensize);
    pen.SetStartCap(Gdiplus::LineCapRound);
    pen.SetEndCap(Gdiplus::LineCapRound);
    pen.SetLineJoin(Gdiplus::LineJoinRound);


    Gdiplus::GraphicsPath graphics_path_;
    for (int i = 0; i < vt_src_pt_.size() - 1; ++i)
    {
        POINT from = vt_src_pt_[i];
        POINT to = vt_src_pt_[i + 1];
        graphics_path_.AddLine(from.x, from.y, to.x, to.y);
    }
    cs.DrawPath(&pen, &graphics_path_);

    if (tools_status_ == TOOLS_HOVER)
    {
        CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, 0xff00ffff);
    }

}

void CPencileUI::DoEvent(TEventUI& event)
{
    if (event.Type == UIEVENT_SETCURSOR && IsEnabled())
    {
        if ((m_uButtonState & UISTATE_CAPTURED) != 0 )
            return;
        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(GetSizerCursor(event.ptMouse, m_iCursor))));
        return;
    }
    if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
    {
        m_uButtonState |= UISTATE_CAPTURED;
        m_ptLastMouse = event.ptMouse;
        RECT rcWindow = main_canvas_->GetWindowRect();
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
    
    CContainerUI::DoEvent(event);
}

void CPencileUI::AddWorldPt(POINT pt, bool fresh, int offsety, float scale)
{
    vt_pt_.push_back(pt);

    if (fresh)
    { 
         POINT srcpt;
         static POINT lastpt{ -100, -100 };

         srcpt.x = pt.x * scale;
         srcpt.y = pt.y * scale - offsety;
         //ÎªÁË·ÀÖ¹
         if (fabsl(pt.x - srcpt.x) > 1 || fabsl(pt.y - srcpt.y) > 1)
         {
             AddPt(srcpt);
             lastpt = srcpt;
         }  
    }
  
}

void CPencileUI::AddPt(POINT pt)
{
    vt_src_pt_.push_back(pt);
    static RECT rect;
    if (vt_src_pt_.size() == 1)
    {
       
        rect.left = pt.x;
        rect.right = pt.x;
        rect.top = pt.y;
        rect.bottom = pt.y;
    }
    else
    {
        if (rect.left > pt.x)
            rect.left = pt.x;
        if (rect.right < pt.x)
            rect.right = pt.x;
        if (rect.top > pt.y)
            rect.top = pt.y;
        if (rect.bottom < pt.y)
            rect.bottom = pt.y; 
        SetPos(rect);
    }
 
}

void CPencileUI::AddPath(POINT from, POINT to)
{
  //  char buffer[512] = { 0 };
  //  sprintf(buffer, "from: [%d, %d] to: [%d, %d]\n", from.x, from.y, to.x, to.y);
  //  OutputDebugStringA(buffer);
    AddPt(from);
    AddPt(to);
    //vt_src_pt_.push_back(to);


}


void CPencileUI::SetOffsetY(int basey)
{
    //ToolBase::SetOffsetY(basey);
    vt_src_pt_.clear();
    RECT rect;
    POINT pt;
    for (int i = 0; i < vt_pt_.size(); ++i)
    {
        pt.x = vt_pt_[i].x*scale_;
        pt.y = vt_pt_[i].y*scale_ - basey;
        vt_src_pt_.push_back(pt);
    }
}

#if 0
void CPencileUI::ResetToolsMap(int basey, float scale, bool flag)
{   
    POINT pt;
    if (flag)
    {   
        vt_src_pt_.clear();
        RECT rect = {0,0,0,0};
        for (int i = 0; i < vt_pt_.size();++i)
        {
            pt.x = vt_pt_[i].x*scale;
            pt.y = vt_pt_[i].y*scale-basey;
            
            vt_src_pt_.push_back(pt);
            
            if (i == 0)
            {
                rect.left = rect.right = pt.x;
                rect.top = rect.bottom = pt.y;
            }
            else
            {
                if (rect.left > pt.x)
                    rect.left = pt.x;
                if (rect.right < pt.x)
                    rect.right = pt.x;
                if (rect.top > pt.y)
                    rect.top = pt.y;
                if (rect.bottom < pt.y)
                    rect.bottom = pt.y;
            }
        }
        scale_ = scale;
        SetPos(rect);
    }
    else
    {
        vt_pt_.clear();
        for (int i = 0; i < vt_src_pt_.size(); ++i)
        {
            pt.x = vt_src_pt_[i].x/scale;
            pt.y = (vt_src_pt_[i].y + basey)/scale;
            vt_pt_.push_back(pt);
        }
    }
}
#endif
void CPencileUI::SetToolsState(ToolsStats status)
{
    tools_status_ = status;
    Invalidate();
}

void CPencileUI::SetToolsScale(float scale)
{
    scale_ = scale;
}


int CPencileUI::ToolHitTest(POINT pt)
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
}



//void CPencileUI::SetToolsOffset2(int offsetx, int offsety)
//{  
//    for (size_t i = 0; i < vt_src_pt_.size(); i++)
//    {
//        vt_src_pt_[i].x += offsetx;
//        vt_src_pt_[i].y += offsety;
//    }
//    RECT rect;
//    rect = GetPos();
//    rect.left += offsetx;
//    rect.right += offsetx;
//    rect.top += offsety;
//    rect.bottom += offsety;
//    SetPos(rect);
//}

CPencileUI *CPencileUI::CreateTools(DWORD color, int pen_size)
{  
    CPencileUI *pControl = new CPencileUI;
    pControl->SetFloat(true);
    pControl->SetInset(RECT{ pen_size, pen_size, pen_size, pen_size });
    pControl->SetBorderColor(0xFFFF00FF);
    pControl->SetBorderSize(2);
    pControl->SetMouseEnabled(false);
    pControl->SetPenColor(color);
    pControl->SetPenSize(pen_size);
    pControl->SetPos(RECT{0,0,0,0});

    return pControl;
         
}
