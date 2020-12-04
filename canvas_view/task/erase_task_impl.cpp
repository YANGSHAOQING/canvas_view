#include "StdAfx.h"
#include "erase_task_impl.h"
#include "windows/main_canvas.h"
#include <tools/select_rect_tool.h>

EraseTaskImpl::EraseTaskImpl(MainCanvas *main_canvas) :
main_canvas_(main_canvas)
{
    left_button_pos_.x = -10;
    left_button_pos_.y = -10;
    pSelect_Rect_ = NULL;
    PreSelectRect();
}



EraseTaskImpl::~EraseTaskImpl()
{
    ClearContext(); 
    if (pSelect_Rect_)
        main_canvas_->GetCanvasContainer()->Remove(pSelect_Rect_);
}

void EraseTaskImpl::ClearContext()
{
 
}


bool EraseTaskImpl::PreSelectRect()
{  
   
    if (main_canvas_ == NULL)
        return false;
    if (pSelect_Rect_ == NULL)
    {
        pSelect_Rect_ = new CSelectRectToolUI;
        if (pSelect_Rect_ == NULL)
            return false;
        pSelect_Rect_->SetFloat(true);
        pSelect_Rect_->SetInset(RECT{ 6, 6, 6, 6 });
        pSelect_Rect_->SetBorderColor(0xFFFF00FF);
        pSelect_Rect_->SetBorderSize(2);
        pSelect_Rect_->SetMouseEnabled(false);
        pSelect_Rect_->SetPos(RECT{ 0, 0, 0, 0 });
        main_canvas_->GetCanvasContainer()->Add(pSelect_Rect_);
    }
    return true;
}


LRESULT EraseTaskImpl::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
    if (wParam == VK_DELETE)
    {
        /*if (main_canvas_)
            main_canvas_->OnControlChange(DELETE_TYPE, NULL);*/


    }
    return S_OK;
}
LRESULT EraseTaskImpl::OnKeyUp(WPARAM wParam, LPARAM lParam)
{   
   
    return S_OK;
}
LRESULT EraseTaskImpl::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    if (main_canvas_ == NULL)

        return S_FALSE;

    SetCapture(main_canvas_->GetHWND());

    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    left_button_pos_ = pt;
    last_pos_ = pt;

    return S_OK;

}


LRESULT EraseTaskImpl::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{   

    ReleaseCapture();

    int size = main_canvas_->tools_manager_.selected_controls_list_.size();
    if (size >0 )
    {
        //main_canvas_->OnControlChange(DELETE_TYPE, NULL);
        //main_canvas_->GetToolsManager()->ClearSelectedControls(false);
        //main_canvas_->GetToolsManager()->ClearSelectedList();
    }
    if (pSelect_Rect_)
    {
        pSelect_Rect_->SetPos(RECT{ 0, 0, 0, 0 });
    }
    return S_OK;

}
LRESULT EraseTaskImpl::OnMouseMove(WPARAM wParam, LPARAM lParam)
{   
    if (lParam == -1 || lParam == -1)
    {
        return S_OK;
    } 
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    if (::GetKeyState(VK_LBUTTON) < 0)
    {
        LBtnDownMove(pt);
    }
    else
    {
        MouseMove(pt);
    }
  

    return S_OK;
}
LRESULT EraseTaskImpl::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{   

    return S_OK;
}
LRESULT EraseTaskImpl::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT EraseTaskImpl::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}

void EraseTaskImpl::MouseMove(POINT pt)
{   
    int size = main_canvas_->tools_manager_.selected_controls_list_.size();
    if (size >1)
        return;
  
    CControlUI *pControl = main_canvas_->tools_manager_.GetControlUI(pt);
    if (pControl)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(pControl);
        if (tool_base && tool_base->ToolHitTest(pt))
        {   
            main_canvas_->GetToolsManager()->ClearSelectedList();

            tool_base->SetToolsState(TOOLS_HOVER);
            main_canvas_->GetToolsManager()->AddToSelectedList(pControl);
            pControl->Invalidate();
        }
    }
  
}

void EraseTaskImpl::LBtnDownMove(POINT pt)
{
    if ((fabsl(last_pos_.x - pt.x) < 3 && fabsl(last_pos_.y - pt.y) < 3))
        return;
    RECT rect;
    rect.left = min(left_button_pos_.x, pt.x);
    rect.right = max(left_button_pos_.x, pt.x);
    rect.top = min(left_button_pos_.y, pt.y);
    rect.bottom = max(left_button_pos_.y, pt.y);

    RECT crt = main_canvas_->GetCanvasContainerRect();
    if (rect.left < crt.left)
        rect.left = crt.left;
    if (rect.top < crt.top)
        rect.top = crt.top;

    if (rect.right > crt.right)
        rect.right = crt.right;
    if (rect.bottom > crt.bottom)
        rect.bottom = crt.bottom;

    if (pSelect_Rect_)
        pSelect_Rect_->SetPos(rect);

    main_canvas_->GetToolsManager()->ClearSelectedList();
    main_canvas_->GetToolsManager()->selected_controls_list_ = main_canvas_->tools_manager_.Search(rect);

    last_pos_ = pt;
}








