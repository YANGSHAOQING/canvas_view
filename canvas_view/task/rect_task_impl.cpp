#include "StdAfx.h"
#include "rect_task_impl.h"
#include "windows/main_canvas.h"
#include "tools/ellipse_tool.h"
#include "tools/arrow_tool.h"
#include "tools/rectangle_tool.h"
#include "tools/triangle_tool.h"
#include <tools/image_tool.h>
#include "tools/diamon_tool.h"
#include <tools/tools_factory.h>
#include <tools/line_tool.h>


RectTaskImpl::RectTaskImpl( MainCanvas* main_canvas) :
main_canvas_(main_canvas)
{
    left_button_pos_.x = -10;
    left_button_pos_.y = -10;
    tool_type_ = ToolTypes::RECTANGLE;
    tool_ = NULL;

  //  SetClassLong(hWnd_,GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));
};

RectTaskImpl::~RectTaskImpl()
{
    ClearContext();
}

void RectTaskImpl::ClearContext()
{
    if (tool_ != NULL)
    {
        delete tool_;
        tool_ = NULL;
    }
    SetClassLong(main_canvas_->GetHWND(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));
}

void RectTaskImpl::SetTaskStatus(ToolTypes tool_type)
{
    left_button_pos_.x = -10;
    left_button_pos_.y = -10;
    tool_type_ = tool_type;
    tool_ = NULL;
}

void RectTaskImpl::AddNewTools()
{  
    int pen_size = main_canvas_->GetPenSize();
    DWORD color = main_canvas_->GetPenColor();
    double scale = main_canvas_->GetScale();

    tool_ = ToolsFactory::CreateTools(tool_type_, color, pen_size, scale);
    ToolBase* tool_base = dynamic_cast<ToolBase*>(tool_);
    if (tool_base)
    {
        tool_base->SetOffsetY(main_canvas_->GetToolsManager()->GetOffset());
    }

    if (tool_)
        main_canvas_->GetCanvasContainer()->Add(tool_);
    
}


LRESULT RectTaskImpl::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
   
    return S_OK;
}
LRESULT RectTaskImpl::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT RectTaskImpl::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    SetCapture(main_canvas_->GetHWND());
    //
    left_button_pos_.x = GET_X_LPARAM(lParam);
    left_button_pos_.y = GET_Y_LPARAM(lParam);
    return S_OK;

}

LRESULT RectTaskImpl::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
    ReleaseCapture();
 
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    //线段较短不添加
    if (fabsl(left_button_pos_.x - pt.x) < 10 && fabsl(fabsl(left_button_pos_.y - pt.y) < 10))
    {   
        if (tool_)
            main_canvas_->GetCanvasContainer()->Remove(tool_);
        tool_ = NULL;
        return S_OK;
    }
    
    //添加数据
    if (tool_)
    {
        main_canvas_->GetToolsManager()->Add(tool_);
        ToolBase* tool_base = dynamic_cast<ToolBase*>(tool_);
        if (tool_base)
        {
            main_canvas_->OnControlAdded(tool_);
        }
    }
    tool_ = NULL;
    return S_OK;

}


void RectTaskImpl::ResetArrowPos(POINT endpt,RECT aRea)
{
    if (endpt.x < aRea.left) endpt.x = aRea.left + 15;
    if (endpt.x > aRea.right) endpt.x = aRea.right - 15;
    if (endpt.y < aRea.top) endpt.y = aRea.top + 15;
    if (endpt.y > aRea.bottom) endpt.y = aRea.bottom - 15;

    ((CArrowToolUI*)tool_)->SetEndPoint(endpt,true);

}
LRESULT RectTaskImpl::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
   
    if (::GetKeyState(VK_LBUTTON) >= 0 || lParam == -1 || lParam == -1)
    {
        return S_OK;
    }

    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    //if (fabsl(left_button_pos_.x - pt.x) > 6 && fabsl(left_button_pos_.y - pt.y) > 6)
    if (tool_ == NULL)
    {
        AddNewTools();
        if (tool_ == NULL)
            return S_OK;
        if (tool_type_ == ARROW)
        {
            ((CArrowToolUI *)tool_)->SetStartPoint(left_button_pos_);
        }
        else if (tool_type_ == LINE)
        {
            ((CLineToolUI *)tool_)->SetStartPoint(left_button_pos_);
        }
    }
    RECT crt = main_canvas_->GetCanvasContainerRect();
    crt.left += 3;
    crt.top += 3;
    crt.right -= 3;
    crt.bottom -= 3;

    if (tool_type_ == ARROW)
    {
        if (pt.x < crt.left) pt.x = crt.left + 15;
        if (pt.x > crt.right) pt.x = crt.right - 15;
        if (pt.y < crt.top) pt.y = crt.top + 15;
        if (pt.y > crt.bottom) pt.y = crt.bottom - 15;

        ((CArrowToolUI*)tool_)->SetEndPoint(pt, true);

    }
    else if (tool_type_ == LINE)
    {
        if (pt.x < crt.left) pt.x = crt.left ;
        if (pt.x > crt.right) pt.x = crt.right ;
        if (pt.y < crt.top) pt.y = crt.top;
        if (pt.y > crt.bottom) pt.y = crt.bottom;

        ((CLineToolUI*)tool_)->SetEndPoint(pt, true);
    }
    else
    {  
        RECT rect;
        rect.left = left_button_pos_.x >= pt.x ? pt.x : left_button_pos_.x;
        rect.top = left_button_pos_.y >= pt.y ? pt.y : left_button_pos_.y;

        rect.right = left_button_pos_.x <= pt.x ? pt.x : left_button_pos_.x;
        rect.bottom = left_button_pos_.y <= pt.y ? pt.y : left_button_pos_.y;
        if (rect.left < crt.left)
            rect.left = crt.left;
        if (rect.top < crt.top)
            rect.top = crt.top;
        if (rect.right > crt.right)
            rect.right = crt.right;
        if (rect.bottom > crt.bottom)
            rect.bottom = crt.bottom;

        rect.left -= 3;
        rect.right += 3;
        rect.top -= 3;
        rect.bottom += 3;

        /*ToolBase* tool_base = dynamic_cast<ToolBase*>(tool_);
        if (tool_base)
        {
        tool_base->SetPosition(rect);
        }*/
        tool_->SetPos(rect);
    }
 
    return S_OK;
}
LRESULT RectTaskImpl::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT RectTaskImpl::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT RectTaskImpl::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}










