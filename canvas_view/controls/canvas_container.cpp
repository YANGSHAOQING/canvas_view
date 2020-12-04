#include "StdAfx.h"
#include "windows/main_canvas.h"
#include "canvas_container.h"

CCanvasContainerUI::CCanvasContainerUI() : m_iCursor(0), m_uButtonState(0)
{
    m_ptClipBasePoint.x = m_ptClipBasePoint.y = 0;
    m_ptLastMouse.x = m_ptLastMouse.y = 0;
}

LPCTSTR CCanvasContainerUI::GetClass() const
{
    return DUI_CTR_CANVASCONTAINER;
}

LPVOID CCanvasContainerUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_CANVASCONTAINER) == 0) return static_cast<CCanvasContainerUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CCanvasContainerUI::GetControlFlags() const
{
    return UIFLAG_SETCURSOR;
}

RECT CCanvasContainerUI::GetSizerRect(int iIndex)
{
    LONG lMiddleX = (m_rcItem.left + m_rcItem.right) / 2;
    LONG lMiddleY = (m_rcItem.top + m_rcItem.bottom) / 2;
    LONG SIZER_WIDTH = m_rcInset.left * 2+1;
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

RECT CCanvasContainerUI::GetSizerOutRect(RECT rect)
{
    return CDuiRect(rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1);
}

LPTSTR CCanvasContainerUI::GetSizerCursor(POINT& pt, int& iCursor)
{
    LONG SIZER_WIDTH = m_rcInset.left * 2 +1;
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
    case 0://左上
        return IDC_SIZENWSE;
    case 1://上中
        return IDC_SIZENS;
    case 2://右上
        return IDC_SIZENESW;
    case 3://左中
        return IDC_SIZEWE;
    case 4://中
        return IDC_SIZEALL;
    case 5://右中
        return IDC_SIZEWE;
    case 6://左下
        return IDC_SIZENESW;
    case 7://下中
        return IDC_SIZENS;
    case 8://右下
        return IDC_SIZENWSE;
    default:
        return IDC_SIZEALL;
    }
}

//m_nBorderSize 为-1 的时候会隐藏边框，用来在保存图片时隐藏
void CCanvasContainerUI::PaintBorder(HDC hDC) {
    if (m_nBorderSize == -1)
        return;
    for (int i = 0; i < 9; ++i) {
        CRenderEngine::DrawColor(hDC, GetSizerRect(i), m_dwBorderColor);
    }
    RECT rc = m_rcItem;
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;
    
    CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, m_dwBorderColor);
}

void CCanvasContainerUI::DoEvent(TEventUI& event)
{
    if (event.Type == UIEVENT_SETCURSOR && IsEnabled())
    {
        if ((m_uButtonState & UISTATE_CAPTURED) != 0)
            return;
        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(GetSizerCursor(event.ptMouse, m_iCursor))));
        return;
    }
    
    CContainerUI::DoEvent(event);
}
