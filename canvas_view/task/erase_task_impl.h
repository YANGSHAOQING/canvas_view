#pragma once
#include "task.h"
#include "windows/main_canvas.h"

class EraseTaskImpl :public Task
{
public:
    EraseTaskImpl(MainCanvas *whiteboard);
    ~EraseTaskImpl();
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
    void   LBtnDownMove(POINT pt);
    void   MouseMove(POINT pt);
    bool   PreSelectRect();
private:
    CContainerUI *pSelect_Rect_;
    POINT       last_pos_;
    POINT   left_button_pos_;
    MainCanvas *main_canvas_;
};
