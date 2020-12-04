#include "StdAfx.h"
#include "pen_icon_tool.h"

CPenIconUI::CPenIconUI()
{
    lable_icon_ = NULL;
    lable_name_ = NULL;

}

LPCTSTR CPenIconUI::GetClass() const
{
    return  DUI_CTR_PENICONTOOL;
}

LPVOID CPenIconUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_PENICONTOOL) == 0) return static_cast<CPenIconUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CPenIconUI::GetControlFlags() const
{
    return UIFLAG_SETCURSOR;
}


void CPenIconUI::Notify(TNotifyUI& msg)
{
   
}

void CPenIconUI::DoInit()
{
    CDialogBuilder builder;                   
    CContainerUI* pChildWindow = static_cast<CVerticalLayoutUI*>(builder.Create(_T("pen_icon.xml"), (UINT)0, NULL, m_pManager));
    if (pChildWindow)
    {
        this->Add(pChildWindow);
        //this->SetChildPadding(3);
        m_pManager->AddNotifier(this);
    }
    else
    {
        this->RemoveAll();
    }

    lable_icon_ = static_cast<CLabelUI*>(m_pManager->FindControl(_T("lable_icon")));
    lable_name_ = static_cast<CLabelUI*>(m_pManager->FindControl(_T("label_name")));



}


void CPenIconUI::PaintBorder(HDC hDC) 
{
     
    return;

}

void CPenIconUI::SetNameText(std::wstring name)
{
    if (lable_name_)
    {
        lable_name_->SetText(name.c_str());
        lable_name_->SetTextColor(0xFFFF0000);
        RECT rect{0};
        HFONT hFont = GetManager()->GetFont(lable_name_->GetFont());
        if (hFont == NULL)
            return;
        HDC hDC = GetManager()->GetPaintDC();
        if (hDC == NULL)
            return;
        DWORD color = lable_name_->GetTextColor();

        GetTextRectSize(rect, name, hFont, hDC, color);

        name_size_.cx = rect.right - rect.left;
        name_size_.cy = rect.bottom - rect.top;
    }
}


std::wstring CPenIconUI::getPenIconPath(int clrIndex)
{
    switch (clrIndex)
    {
        //  case 1:
        //      break;
    default:
        return L"penIcon/pen_black.png";
    }
}
void CPenIconUI::SetIconImage(int clrIndex)
{
    if (lable_icon_)
    {   
        std::wstring path = getPenIconPath(clrIndex);
        int w = 0, h = 0;
        GetImageSize(path, w, h);
        icon_size_.cx = w;
        icon_size_.cy = h;
        lable_icon_->SetFixedWidth(w);
        lable_icon_->SetFixedHeight(h);
        lable_icon_->SetBkImage(path.c_str());
    }     
}

void CPenIconUI::SetBasePos(POINT pt)
{ 
    base_pos_ = pt; 
    RECT rect{ pt.x, pt.y, 0, 0 };
    rect.right = pt.x + icon_size_.cx + name_size_.cx;
    rect.bottom = pt.y + icon_size_.cy + name_size_.cy;
    SetPos(rect);   
}


void CPenIconUI::SetPenIconHide()
{   
    if (GetTickCount() - last_showtime_ > 5000)
    {
       CContainerUI::SetPos(RECT{ 0, 0, 0, 0 });
    }
    return;

}

CPenIconUI* CPenIconUI::CreateTools()
{
     
    CPenIconUI *pPenTool = new CPenIconUI;
    if (pPenTool == NULL)
        return NULL;
    pPenTool->SetFloat(true);
   // pPenTool->SetInset(RECT{ 2, 2, 2, 2 });
    pPenTool->SetMouseEnabled(false);
    pPenTool->SetPenIconHide();
    return pPenTool;
}

