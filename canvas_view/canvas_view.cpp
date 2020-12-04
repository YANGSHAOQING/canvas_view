#include "canvas_view.h"
#include "canvas_view_impl.h"


CanvasView* CreateCanvasView(void *hwnd, CanvasView::Delegate* delegate)
{
    return new CanvasViewImpl(hwnd, delegate);
}

void DestroyCanvasView(CanvasView* canvas_view)
{
    delete canvas_view;
}

