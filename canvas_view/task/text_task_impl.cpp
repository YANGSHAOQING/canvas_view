#include "StdAfx.h"
#include "text_task_impl.h"
#include <tools/text_tool.h>
#include <tools/tools_factory.h>


TextTaskImpl::TextTaskImpl(MainCanvas *main_canvas) :
main_canvas_(main_canvas)
{
    //  SetClassLong(hWnd_,GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));
    text_tool_ = NULL;
    last_focused_tool_ = NULL;
};

TextTaskImpl::~TextTaskImpl()
{
    //  SetClassLong(hWnd_, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
}

void TextTaskImpl::ClearContext()
{
    //AddData(false);
    last_focused_tool_ = NULL;
    SetClassLong(main_canvas_->GetHWND(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_IBEAM));
    
}

void TextTaskImpl::AddTextTool(POINT pt)
{   
    CContainerUI* canvas_container = main_canvas_->GetCanvasContainer();

    int pen_size = main_canvas_->GetPenSize();
    int color = main_canvas_->GetPenColor();
    float scale = main_canvas_->GetScale();

    //scale = 1.0f;
    //text_tool_ = DuiLibEx::CTextTool2UI::CreateTools(color, pen_size, scale);
    //CTextToolUI *text_tool_ = CTextToolUI::CreateTools(color, pen_size, scale);
    //DuiLibEx::CTextTool2UI *text_tool_ = DuiLibEx::CTextTool2UI::CreateTools(0xff00ffff, pen_size, scale);
    //text_tool_ = DuiLib3::CTextTool3UI::CreateTools(color, pen_size, scale);
    text_tool_ = (DuiLibEx::CTextToolUI*)ToolsFactory::CreateTools(ToolTypes::TEXT, color, pen_size, scale);
    
    ToolBase* tool_base = dynamic_cast<ToolBase*>(text_tool_);
    if (tool_base)
    {
        tool_base->SetOffsetY(main_canvas_->GetToolsManager()->GetOffset());
    }
    
    canvas_container->Add(text_tool_);
    RECT rt_container = main_canvas_->GetCanvasContainerRect();
    RECT rcText = text_tool_->GetMinRect();
    RECT inset = text_tool_->GetInset();
    RECT text_rt = { pt.x - inset.left, pt.y - inset.top, pt.x + 100, pt.y + rcText.bottom + inset.bottom };
    int width = rt_container.right - pt.x;
    if (width < 100)
    {
       // text_rt = { text_rt.left, text_rt.top, text_rt.left + width, text_rt.top + 35 };
    }

    text_tool_->SetPos(text_rt);
    //text_tool_->SetMinWidth(text_rt.right - text_rt.left);
    //text_tool_->SetFocus();
    text_tool_->SetMainCanvas(main_canvas_);
    last_focused_tool_ = text_tool_;

}


LRESULT TextTaskImpl::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
   
    return S_OK;
}
LRESULT TextTaskImpl::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT TextTaskImpl::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    if (main_canvas_ == NULL)
        return S_OK;
    SetCapture(main_canvas_->GetHWND());
    //
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    //CControlUI* control = main_canvas_->GetPaintMangerUI()->FindControl(pt);
    CControlUI* control = main_canvas_->GetToolsManager()->GetControlUI(pt);
    if (control != NULL)
    {
        WCHAR info[256] = { 0 };
        swprintf(info, L"OnLButtonDown %s \n", control->GetClass());
        OutputDebugStringW(info);

        if (wcscmp(control->GetClass(), L"TextTool") == 0)
        {
            if (control->IsFocused())
            {
                OutputDebugStringW(L"11111\r\n");
                return S_OK;
            }
            else
            {
                OutputDebugStringW(L"22222\r\n");
                //control->SetFocus();
                return S_FALSE;
            }
            //::SetFocus(main_canvas_->GetPaintMangerUI()->GetPaintWindow());
        }
        else
        {
            /*if (text_tool_->IsFocused())
            {

            }*/
            OutputDebugStringW(L"33333\r\n");
        }
    }
    else
    {
        if (last_focused_tool_ != NULL && last_focused_tool_->IsFocused())
        {
            //::SetFocus(main_canvas_->GetPaintMangerUI()->GetPaintWindow());

            //DuiLib3::CTextTool3UI* tool = dynamic_cast<DuiLib3::CTextTool3UI*>(control);
            //tool->SetFocusOuter(false);

            main_canvas_->SetCanvasFocus();
            last_focused_tool_ = NULL;
            return S_FALSE;
        }
        else
        {
            AddTextTool(pt);
        }

    }

    OutputDebugStringW(L"OnLButtonDown add text tool\n");

    

    return S_OK;

}

LRESULT TextTaskImpl::OnLButtonDClick(WPARAM wParam, LPARAM lParam)
{
    //CControlUI* focused_ui = main_canvas_->GetPaintMangerUI()->GetFocus();

    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    CControlUI* control = main_canvas_->GetPaintMangerUI()->FindControl(pt);

    if (control != NULL)
    {
        if (wcscmp(control->GetClass(), L"TextTool") == 0)
        {
            WCHAR info[256] = { 0 };
            swprintf(info, L"OnLButtonDClick %s \n", control->GetClass());
            //OutputDebugStringW(info);
            //::SetFocus(main_canvas_->GetPaintMangerUI()->GetPaintWindow());
            //control->SetFocus();

            //DuiLib3::CTextTool3UI* tool = dynamic_cast<DuiLib3::CTextTool3UI*>(control);
            //tool->SetFocusOuter(true);


            last_focused_tool_ = control;
            return S_OK;
        }
    }

    return S_OK;
}


LRESULT TextTaskImpl::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
    ReleaseCapture();

    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    //如果没有输入文字
    /*if (wcscmp(text_tool_->GetText(), L"") == 0)
    {
        if (text_tool_)
            main_canvas_->GetCanvasContainer()->Remove(text_tool_);

        return S_OK;
    }*/

    //添加数据
    if (text_tool_)
    {
        main_canvas_->GetToolsManager()->Add(text_tool_);
        ToolBase* tool_base = dynamic_cast<ToolBase*>(text_tool_);
        if (tool_base)
        {
            //tool_base->SetOffsetY(main_canvas_->GetToolsManager()->GetOffset());
            main_canvas_->OnControlAdded(text_tool_);
        }
    }
    text_tool_ = NULL;

    return S_OK;

}


LRESULT TextTaskImpl::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    if (::GetKeyState(VK_LBUTTON) >= 0 || lParam == -1 || lParam == -1)
    {
        return S_OK;
    }
    return S_OK;
}
LRESULT TextTaskImpl::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT TextTaskImpl::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT TextTaskImpl::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}







