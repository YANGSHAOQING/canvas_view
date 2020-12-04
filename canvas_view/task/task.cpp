#pragma once
#include "StdAfx.h"
#include "task.h"

Task::Task()
{  

}
Task::~Task()
{

}

LRESULT Task::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}

LRESULT Task::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}
LRESULT Task::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}

LRESULT Task::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}

LRESULT Task::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}

LRESULT Task::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}

LRESULT Task::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}

LRESULT Task::OnLButtonDClick(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}


LRESULT Task::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
    return S_OK;
}

LRESULT Task::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
      return  OnLButtonDown(wParam, lParam);
    case WM_LBUTTONUP:
        return OnLButtonUp(wParam, lParam);
    case WM_RBUTTONDOWN:
        return OnRButtonDown(wParam, lParam);
    case WM_RBUTTONUP:
        return OnRButtonUp(wParam, lParam);
    case WM_MOUSEMOVE:
        return OnMouseMove(wParam, lParam);
    case WM_MOUSEWHEEL:
        return OnMouseWheel(wParam, lParam);
    case WM_KEYDOWN:
        return OnKeyDown(wParam, lParam);
    case WM_KEYUP:
        return OnKeyUp(wParam, lParam);
    case WM_LBUTTONDBLCLK:
        return OnLButtonDClick(wParam, lParam);
    default:
        break;
    }
    return S_OK;
}
