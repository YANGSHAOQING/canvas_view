#include "StdAfx.h"
#include "select_task_impl.h"
#include "windows/main_canvas.h"
#include <tools/select_rect_tool.h>

SelectTaskImpl::SelectTaskImpl(MainCanvas *main_canvas) :
main_canvas_(main_canvas)
{
    left_button_pos_.x = -10;
    left_button_pos_.y = -10;
    mouse_mode_ = SelectMode;
    select_rect_ = NULL;
    PreSelectRect();
};



SelectTaskImpl::~SelectTaskImpl()
{
    ClearContext();
    if (select_rect_)
        main_canvas_->GetCanvasContainer()->Remove(select_rect_);
}
void SelectTaskImpl::ClearContext()
{
    if (main_canvas_)
    {
        main_canvas_->GetToolsManager()->ClearSelectedList();
    }
}


bool SelectTaskImpl::PreSelectRect()
{  
    assert(main_canvas_);

    if (select_rect_ == NULL)
    {
        select_rect_ = new CSelectRectToolUI;
        if (select_rect_ == NULL)
            return false;
        select_rect_->SetFloat(true);
        select_rect_->SetInset(RECT{ 1, 1,1, 1 });
        select_rect_->SetBorderColor(0xFFFF00FF);
        select_rect_->SetBorderSize(2);
        select_rect_->SetMouseEnabled(false);
        select_rect_->SetPos(RECT{ 0, 0, 0, 0 });
        main_canvas_->GetCanvasContainer()->Add(select_rect_);
    }
    return true;
}


LRESULT SelectTaskImpl::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
    if (wParam == VK_DELETE)
    {
        if (main_canvas_)
            main_canvas_->OnControlRemoved();
        
    }
    return S_OK;
}
LRESULT SelectTaskImpl::OnKeyUp(WPARAM wParam, LPARAM lParam)
{   
   
    return S_OK;
}
LRESULT SelectTaskImpl::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    SetCapture(main_canvas_->GetHWND());
    left_button_pos_ = pt;
    last_pos_ = pt;

    //CControlUI *new_control = main_canvas_->GetToolsManager()->GetControlUI(pt);
    CControlUI *new_control = main_canvas_->GetPaintMangerUI()->FindControl(pt);
    
    //点击空白区域
    //if (new_control == main_canvas_->GetCanvasContainer())
    if (new_control->GetName() == L"desktopmask")
    {
        mouse_mode_ = SelectMode;
        return S_OK;
    }
    else
    {
        ToolBase* tool = dynamic_cast<ToolBase*>(new_control);
        if (tool)
        {
            int pos = tool->ToolHitTest(pt);
            if (pos == 4)
                mouse_mode_ = DragMode;
            else
                mouse_mode_ = SingleChangeMode;
        }
        

        if (!main_canvas_->GetToolsManager()->IsControlSelected(new_control))
        {
            main_canvas_->GetToolsManager()->ClearSelectedList();
            main_canvas_->GetToolsManager()->AddToSelectedList(new_control);
        }
        else
        {
            main_canvas_->GetToolsManager()->AddToSelectedList(new_control);
        }


        return S_OK;
    }
}


LRESULT SelectTaskImpl::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{   
    ReleaseCapture();
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    CControlUI *new_control = main_canvas_->GetToolsManager()->GetControlUI(pt);

    //在空白的区域抬起左键为取消所有
    if (new_control == NULL)
    {
        if (left_button_pos_.x == pt.x && left_button_pos_.y == pt.y)
        {

            main_canvas_->GetToolsManager()->ClearSelectedList();
        }
        else
        {
            if (mouse_mode_ == DragMode || mouse_mode_ == SingleChangeMode)
            {
                main_canvas_->OnControlModified();
            }
            else if (mouse_mode_ == SelectMode)
            {
                if (select_rect_)
                {
                    select_rect_->SetPos(RECT{ 0, 0, 0, 0 });
                }
            }
        }
        return S_OK;
    }

    //在某个控件上抬起右键并且没有移动，则为单选
    if (left_button_pos_.x == pt.x && left_button_pos_.y == pt.y)
    {
        //如果控件不在选中的列表中
        if (!main_canvas_->GetToolsManager()->IsControlSelected(new_control))
        {
            if (::GetKeyState(VK_LCONTROL) < 0)
            {
                main_canvas_->GetToolsManager()->AddToSelectedList(new_control);
                return S_OK;
            }
            else
            {
                main_canvas_->GetToolsManager()->ClearSelectedList();
                main_canvas_->GetToolsManager()->AddToSelectedList(new_control);
                return S_OK;
            }
        }
        else
        {
            //按control后反选
            if (::GetKeyState(VK_LCONTROL) < 0)
            {
                main_canvas_->GetToolsManager()->RemoveFromSelectedList(new_control);
                return S_OK;
            }
            else
            {
                return S_OK;
            }
        }
    }
    else
    {
        if (mouse_mode_ == DragMode || mouse_mode_ == SingleChangeMode)
        {
            main_canvas_->OnControlModified();
        }
        else if (mouse_mode_ == SelectMode)
        {
            if (select_rect_)
            {
                select_rect_->SetPos(RECT{ 0, 0, 0, 0 });
            }
        }
        
    }
    
    return S_OK;

}
LRESULT SelectTaskImpl::OnMouseMove(WPARAM wParam, LPARAM lParam)
{   
    if (lParam == -1)
    {
        return S_OK;
    } 

    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    if (::GetKeyState(VK_LBUTTON) < 0)
    {
         return LBtnDownMove(pt);
    }
    else
    {
        MouseHover(pt);
    }
  

    return S_OK;
}
LRESULT SelectTaskImpl::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{   

    return S_OK;
}
LRESULT SelectTaskImpl::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT SelectTaskImpl::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}

void SelectTaskImpl::MouseHover(POINT pt)
{   
    std::list<CControlUI*>controls = main_canvas_->GetToolsManager()->GetCurrentPageControls();

    CControlUI *new_control = main_canvas_->GetToolsManager()->GetControlUI(pt);
    if (new_control)
    {
        std::list<CControlUI*>::iterator it = controls.begin();
        for (; it != controls.end(); it++)
        {
            if (*it != new_control)
            {
                ToolBase* tool = dynamic_cast<ToolBase*>(*it);
                if (tool)
                {
                    if (tool->GetToolsState() != TOOLS_EDITABLE && tool->GetToolsState() != TOOLS_SELECTED)
                    {
                        tool->SetToolsState(TOOLS_NORMAL);
                    }
                }
            }
            else
            {
                ToolBase* tool = dynamic_cast<ToolBase*>(new_control);
                if (tool && tool->ToolHitTest(pt) != -1)
                {
                    if (tool->GetToolsState() != TOOLS_EDITABLE && tool->GetToolsState() != TOOLS_SELECTED)
                    {
                        tool->SetToolsState(TOOLS_HOVER);
                    }

                    //tool->SetToolsState(TOOLS_HOVER);
                }
            }
        }
    }
    else
    {
        std::list<CControlUI*>::iterator it = controls.begin();
        for (; it != controls.end(); it++)
        {
            ToolBase* tool = dynamic_cast<ToolBase*>(*it);
            if (tool)
            {
                if (tool->GetToolsState() != TOOLS_EDITABLE && tool->GetToolsState() != TOOLS_SELECTED)
                {
                    tool->SetToolsState(TOOLS_NORMAL);
                }
            }
        }
    }
}

LRESULT SelectTaskImpl::LBtnDownMove(POINT pt)
{
    if ((fabsl(last_pos_.x - pt.x) < 3 && fabsl(last_pos_.y - pt.y) < 3))
        return S_OK;
    //移动状态
    if (mouse_mode_ == DragMode)
    {
        if (main_canvas_->GetToolsManager()->GetSelectedListCount())
        {
            RECT container_border = main_canvas_->GetCanvasContainerRect();

            int offset_x = pt.x - last_pos_.x;
            int offset_y = pt.y - last_pos_.y;
            main_canvas_->GetToolsManager()->SetSelectedToolsOffset(offset_x, offset_y, container_border);
        }

    }
    //框选状态
    else if (mouse_mode_ == SelectMode)
    {
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

        if (select_rect_)
            select_rect_->SetPos(rect);

        main_canvas_->GetToolsManager()->ClearSelectedList();
        main_canvas_->GetToolsManager()->SelectControlsInRect(rect);
    }
    else if (mouse_mode_ == SingleChangeMode)
    {
        RECT crt = main_canvas_->GetCanvasContainerRect();
        char info[256] = { 0 };
        sprintf(info, "%d-%d \r\n", pt.x, pt.y);
        OutputDebugStringA(info);

        if (pt.x > crt.right || pt.y > crt.bottom)
            return S_FALSE;
    }
    last_pos_ = pt;
    return S_OK;
}








