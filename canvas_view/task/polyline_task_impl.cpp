#include "StdAfx.h"
#include "windows/main_canvas.h"
#include "polyline_task_impl.h"



PolylineTaskImpl::PolylineTaskImpl(MainCanvas* main_canvas) :
    main_canvas_(main_canvas)
{
    last_pos_.x = -10;
    last_pos_.y = -10;
    send_last_pos_ = false;

};

PolylineTaskImpl::~PolylineTaskImpl()
{
    ClearContext();

}

void PolylineTaskImpl::ClearContext()
{

}

LRESULT PolylineTaskImpl::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
   
    return S_OK;
}
LRESULT PolylineTaskImpl::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT PolylineTaskImpl::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    SetCapture(main_canvas_->GetHWND());
    //
    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    last_pos_ = pt;
    return S_OK;

}
LRESULT PolylineTaskImpl::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
    ReleaseCapture();
    if (send_last_pos_)
    {
        main_canvas_->SendPenPath(PEN_STATE_UP, last_pos_);
        send_last_pos_ = false;
    }
  
    return S_OK;

}
LRESULT PolylineTaskImpl::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    if (lParam == -1 || lParam == -1)
    {
        return S_OK;
    }

    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    if (fabsl(last_pos_.x - pt.x) > 3 || fabsl(last_pos_.y - pt.y) > 3)
    {
        if (::GetKeyState(VK_LBUTTON) < 0)
        {

            if (!send_last_pos_)
            {
                main_canvas_->SendPenPath(PEN_STATE_DOWN, last_pos_);
                send_last_pos_ = true;
            }
            last_pos_ = pt;
            main_canvas_->SendPenPath(PEN_STATE_DOWN, pt);
        }
        else
        {
            main_canvas_->SendPenPath(PEN_STATE_HOVER, pt);
        }
    }
   

    return S_OK;
}


LRESULT PolylineTaskImpl::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT PolylineTaskImpl::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT PolylineTaskImpl::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}







