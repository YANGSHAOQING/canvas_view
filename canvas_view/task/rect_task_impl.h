#pragma once
#include "task.h"
#include "tools_types.h"

class RectTaskImpl :public Task
{
public:
    RectTaskImpl(MainCanvas* main_canvas);
    ~RectTaskImpl();
public:
    LRESULT OnKeyDown( WPARAM wParam, LPARAM lParam) override;
    LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnRButtonDown( WPARAM wParam, LPARAM lParam) override;
    LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam) override;
    LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam) override;
    void    ClearContext() override;
public:
    void    AddNewTools();

    void    ResetArrowPos(POINT endpt,RECT aRea);

    void    SetTaskStatus(ToolTypes tool_type);

private:

    CControlUI *tool_;
    ToolTypes tool_type_;
    POINT   left_button_pos_;
    MainCanvas *main_canvas_;
};
