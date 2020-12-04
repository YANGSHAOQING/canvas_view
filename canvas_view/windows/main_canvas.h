#pragma once
#include "canvas_view.h"
#include <windows.h>
#include "tools_types.h"
#include "task/task.h"
#include "task/rect_task_impl.h"
#include "task/select_task_impl.h"
#include "task/polyline_task_impl.h"
#include "task/text_task_impl.h"
#include "task/erase_task_impl.h"
#include "task/canvas_task_impl.h"

#include "utility/tools_manager.h"
#include "coordinate_system/coordinate_system.h"

class RectTaskImpl;
class SelectTaskImpl;
class PolylineTaskImpl;
class TextTaskImpl;
class EraseTaskImpl;
class CanvasTaskImpl;

class MainCanvas : public CWindowWnd, 
    public INotifyUI, 
    public IMessageFilterUI,
    public ITranslateAccelerator
{
public:
    MainCanvas();
    virtual ~MainCanvas();

    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    void OnFinalMessage(HWND /*hWnd*/);

    void Init();
    void Notify(TNotifyUI& msg);

    LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT TranslateAccelerator(MSG *pMsg) override;

    void OnSize();
public:

    HDC m_hDcOffscreen;
    HBITMAP m_hbmpOffscreen;
    HBITMAP m_hMosicaBitmap;

private:
    CPaintManagerUI m_pm;

    CControlUI* m_pDesktopImage;
    CControlUI* m_pDesktopMask;
    CVerticalLayoutUI* m_pCanvasLayout;
       //画布控件窗口
    CContainerUI* m_pDesktopCanvasContainer;  //屏幕控件的管理类

    RECT m_rcWindow;

public:

    //接收消息
    RECT GetWindowRect() const;
    RECT GetCanvasContainerRect() const;
    CContainerUI* GetCanvasContainer();

    Task  *task_view_;
    RectTaskImpl* rect_task_;
    SelectTaskImpl* select_task_;
    PolylineTaskImpl* polyline_task_;
    TextTaskImpl* text_task_;
    EraseTaskImpl* eraser_task_;
    CanvasTaskImpl* canvas_task_;

    

    ToolTypes tool_type_;
    DWORD pen_color_;
    int pen_size_;
    int page_id_;
    bool editable_;

    ToolsManager tools_manager_;
    CoordinateSystem coordinate_system_;
    
    //设置屏幕和世界坐标以及画板与屏幕的转换
    void  ResizeAllElemsInCanvas();
    void  RefreshView();
    CanvasView::Delegate* delegate_;

    ToolsManager* GetToolsManager();
    CPaintManagerUI* GetPaintMangerUI();

 //回传给调用者 发送消息到对端
public:

    void OnDeletePen(CControlUI *pControl);

    void OnControlModified();
    void OnControlAdded(CControlUI* control);
    void OnControlRemoved();
    void OnControlChanged();

    void OnTextChanged(CControlUI* control);
    void OnTextControlRemoved(CControlUI* control);


    //颜色和大小  外部有保存
    void SendPenPath(int status, POINT pt);

    //调用者调用接口传递数据
public:

    void AddImage(char *url, char *filepath, char *info);

    void SetDelegate(CanvasView::Delegate *delegate);

    void SelectTools(ToolTypes tool_type);

    void ResetCanvas(int width, int height);

    void SetOffset(int offsety, bool scroll);

    void SetPenColor(DWORD color);
    DWORD GetPenColor();
    
    void SetPenSize(int pen_size);
    int GetPenSize();

    double GetScale();

    void PageTo(int page);

    void ClearControls();

    void UnMarshalGraphics(Json::Value& graphic);
    Json::Value MarshalGraphics();

    void UnMarshalPageGraphics(Json::Value& graphics);

    void ChangeControls(ChangeType change_type, Json::Value changes);


    void DoAddControl(Json::Value graphic);
    void DoModifyControls(Json::Value graphics);
    void RemoveControls(Json::Value graphics);
    void DoModifyControl(Json::Value graphic);

    void SetCanvasFocus();
    void SetEditable(bool enable);
};
