#pragma once
#include "task.h"
#include "windows/main_canvas.h"
#include <tools/text_tool.h>

class TextTaskImpl :
    public Task
{
public:
    TextTaskImpl(MainCanvas *main_canvas);
    ~TextTaskImpl();
public:
    LRESULT OnKeyDown( WPARAM wParam, LPARAM lParam) override;
    LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnRButtonDown( WPARAM wParam, LPARAM lParam) override;
    LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnLButtonDClick(WPARAM wParam, LPARAM lParam) override;
    void    ClearContext() override;
    void    AddTextTool( POINT pt);

private:
    //DuiLibEx::CTextTool2UI *text_tool_;
    //CTextToolUI *text_tool_;
    MainCanvas *main_canvas_;

    DuiLibEx::CTextToolUI * text_tool_;

    CControlUI * last_focused_tool_;

};
