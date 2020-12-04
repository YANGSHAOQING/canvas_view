#pragma once
#include "task.h"
#include "tools/pencile_tool.h"
#include "main_canvas.h"

class PolylineTaskImpl :public Task
{
public:
    PolylineTaskImpl(MainCanvas* main_canvas);
    ~PolylineTaskImpl();
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
    
    bool send_last_pos_;
    POINT   last_pos_;
    MainCanvas *main_canvas_;

};
