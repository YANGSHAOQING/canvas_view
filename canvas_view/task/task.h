#pragma once

class MainCanvas;

class Task 
{
public:
    Task();
    virtual ~Task();
public:
   virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
   virtual LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam);
   virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
   virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
   virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
   virtual LRESULT OnRButtonDown(WPARAM wParam, LPARAM lParam);
   virtual LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam);
   virtual LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam);
   virtual LRESULT OnLButtonDClick(WPARAM wParam, LPARAM lParam);
   virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
   virtual void ClearContext() = 0;

};