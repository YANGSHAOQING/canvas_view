#pragma once
#include "task.h"
#include "windows/main_canvas.h"

class SelectTaskImpl :public Task
{
public:
    SelectTaskImpl(MainCanvas *whiteboard);
    ~SelectTaskImpl();

    
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
    LRESULT   LBtnDownMove(POINT pt);
    void   MouseHover(POINT pt);
    bool   PreSelectRect();
private:
    enum MouseMode
    {
        SelectMode,
        DragMode,
        SingleChangeMode,
    };


    CContainerUI *select_rect_;
    POINT       last_pos_;
    POINT   left_button_pos_;
    MainCanvas *main_canvas_;
    MouseMode mouse_mode_;
};
