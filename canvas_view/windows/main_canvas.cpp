#include "StdAfx.h"
#include "main_canvas.h"
#include "controls/canvas.h"
#include "controls/canvas_container.h"
#include "tools/rectangle_tool.h"
#include "tools/pencile_tool.h"
#include "tools/arrow_tool.h"
#include "tools/ellipse_tool.h"
#include "tools/image_tool.h"
#include "tools/line_tool.h"
#include <iostream>
#include "utility/tools_manager.h"
#include "tools/tools_factory.h"


class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
    CControlUI* CreateControl(LPCTSTR pstrClass)
    {
        if (_tcscmp(pstrClass, DUI_CTR_CANVAS) == 0) return new CCanvasUI;
        else if (_tcscmp(pstrClass, DUI_CTR_CANVASCONTAINER) == 0) return new CCanvasContainerUI;
        else if (_tcscmp(pstrClass, DUI_CTR_RECTANGLETOOL) == 0)  return new CRectangleToolUI;
        else if (_tcscmp(pstrClass, DUI_CTR_ELLIPSETOOL) == 0)  return new CEllipseToolUI;
        else if (_tcscmp(pstrClass, DUI_CTR_PENCILETOOL) == 0)  return new CPencileUI;
        else if (_tcscmp(pstrClass, DUI_CTR_ARROWTOOL) == 0)  return new CArrowToolUI;
        else if (_tcscmp(pstrClass, DUI_CTR_IMAGETOOL) == 0)  return new CImageToolUI;
        else if (_tcscmp(pstrClass, DUI_CTR_PENICONTOOL) == 0)  return new CPenIconUI;
        else if (_tcscmp(pstrClass, DUI_CTR_LINETOOL) == 0)  return new CLineToolUI;
        return NULL;
    }
};

MainCanvas::MainCanvas() 
{
    delegate_ = nullptr;
    tool_type_ = PENCILE;
    pen_color_ = 0xff00ffff;
    pen_size_ = 1;
    ::ZeroMemory(&m_rcWindow, sizeof(m_rcWindow));
    m_hMosicaBitmap = NULL;
    m_hDcOffscreen = NULL;
    m_hbmpOffscreen = NULL;
    m_pCanvasLayout = NULL;
    m_pDesktopImage = NULL;
    m_pDesktopMask = NULL;
    task_view_ = NULL;
    page_id_ = -1;
    editable_ = true;
}

MainCanvas::~MainCanvas()
{
    m_pm.RemoveTranslateAccelerator(this);

    delete rect_task_;
    delete select_task_;
    delete polyline_task_;
    delete text_task_;
    delete canvas_task_;
}

LPCTSTR MainCanvas::GetWindowClassName() const
{
    return _T("MainCanvasView");
}

UINT MainCanvas::GetClassStyle() const
{
    return UI_CLASSSTYLE_DIALOG | CS_DBLCLKS;
}

void MainCanvas::OnFinalMessage(HWND /*hWnd*/)
{
    delete this;
};

void MainCanvas::Init()
{
    m_pDesktopImage = m_pm.FindControl(_T("desktopimage"));
    m_pDesktopMask = m_pm.FindControl(_T("desktopmask"));
    m_pCanvasLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("canvas_layout")));
    m_pDesktopCanvasContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("desktopcanvascontainer")));
    if (m_pDesktopImage == NULL || m_pDesktopMask == NULL ||
         m_pDesktopCanvasContainer == NULL) ::PostQuitMessage(0L);

    m_rcWindow.left = 0;;
    m_rcWindow.top = 0;
    m_rcWindow.right = m_rcWindow.left + 640;
    m_rcWindow.bottom = m_rcWindow.top + 464;

    ::SetWindowPos(GetHWND(), NULL, m_rcWindow.left, m_rcWindow.top, m_rcWindow.right - m_rcWindow.left, m_rcWindow.bottom - m_rcWindow.top, SWP_NOMOVE);
    m_pDesktopCanvasContainer->SetVisible(true);
    m_pDesktopCanvasContainer->SetEnabled(false);
    m_pDesktopCanvasContainer->SetMouseEnabled(false);
    
    m_pCanvasLayout->SetMouseEnabled(false);


    rect_task_ = new RectTaskImpl(this);
    select_task_ = new SelectTaskImpl(this);
    polyline_task_ = new PolylineTaskImpl(this);
    text_task_ = new TextTaskImpl(this);
    eraser_task_ = new EraseTaskImpl(this);
    canvas_task_ = new CanvasTaskImpl(this);

}

void MainCanvas::ResizeAllElemsInCanvas()
{  
    if (m_pDesktopMask == NULL || m_pDesktopImage == NULL || m_pDesktopCanvasContainer == NULL )
        return;

    GetClientRect(m_hWnd, &m_rcWindow);

    int width = m_rcWindow.right - m_rcWindow.left;
    int height = m_rcWindow.bottom - m_rcWindow.top;

    coordinate_system_.SetDisplaySize(width, height);

    double canvas_size[2];
    coordinate_system_.GetDisplaySize(canvas_size);

    
    
    m_pDesktopCanvasContainer->SetFixedHeight(canvas_size[1]);
    m_pDesktopCanvasContainer->SetMaxHeight(canvas_size[1]);


    SIZE sz = m_pCanvasLayout->GetScrollPos();
    sz.cy = coordinate_system_.GetOffset()*coordinate_system_.GetSacle();
    m_pCanvasLayout->SetScrollPos(sz);

    tools_manager_.SetScale(coordinate_system_.GetSacle());

}

void MainCanvas::RefreshView()
{
    tools_manager_.ResizeAllElemsInCanvas();
}

void MainCanvas::Notify(TNotifyUI& msg)
{
    if (msg.sType == _T("windowrelayout")) {
        tools_manager_.SetScale(coordinate_system_.GetSacle());
    }
    else if (msg.sType == DUI_MSGTYPE_SCROLL)
    {
        if (m_pCanvasLayout)
        {
          //  SIZE sz = m_pCanvasLayout->GetScrollPos();
       //     tools_manager_.SetOffsetY(sz.cy);
        }
    }

}

RECT MainCanvas::GetWindowRect() const
{
    return m_rcWindow;
}

CContainerUI* MainCanvas::GetCanvasContainer()
{
    return m_pDesktopCanvasContainer;
}

RECT MainCanvas::GetCanvasContainerRect() const
{
    RECT rcInset = m_pDesktopCanvasContainer->GetInset();
    RECT rc = {0,0,0,0};

    rc.left = rc.left - rcInset.left; if (rc.left < 0) rc.left = -rcInset.left;
    rc.top = rc.top - rcInset.left; if (rc.top < 0) rc.top = -rcInset.left;
    rc.right = m_rcWindow.right - m_rcWindow.left - rc.right + rcInset.left;
    rc.bottom = m_rcWindow.bottom - m_rcWindow.top - rc.bottom + rcInset.left;
    return rc;
}


LRESULT MainCanvas::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    return S_OK;
}
LRESULT MainCanvas::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return S_OK;
}
LRESULT MainCanvas::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    if (uMsg == WM_CREATE) {
        m_pm.Init(m_hWnd);
        m_pm.AddPreMessageFilter(this);
        CDialogBuilder builder;
        CDialogBuilderCallbackEx cb;
        CControlUI* pRoot = builder.Create(_T("main_canvas.xml"), (UINT)0, &cb, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        m_pm.AddTranslateAccelerator(this);
        Init();
        return 0;
    }
    else if (uMsg == WM_DESTROY) {
        //::PostQuitMessage(0L);
    }
    else if (uMsg == WM_SIZE)
    {
        OnSize();  
    }
    else if (uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDOWN || uMsg == WM_MOUSEMOVE ||
             uMsg == WM_RBUTTONDOWN || uMsg==WM_RBUTTONUP || uMsg == WM_KEYDOWN
             || uMsg == WM_KEYUP /*|| uMsg == WM_MOUSEWHEEL*/ || uMsg == WM_LBUTTONDBLCLK)
    {  
        if (tool_type_ == UNKNOW)
        {
            return S_OK;
        }

        

        if (tool_type_ == NORMAL) //将事件回传给调用窗口
        {
            if (delegate_)
                delegate_->OnMouseKeyMessage(uMsg, wParam, lParam);
        }
        else
        {
            if (task_view_)
            {
                LRESULT result = task_view_->HandleMessage(uMsg, wParam, lParam);
				if (tool_type_ == SELECT)
				{
					if (delegate_)
						delegate_->OnMouseKeyMessage(uMsg, wParam, lParam);
				}
                if (result != S_OK)
                    return result;
            }
        }
    }
    else if (uMsg == WM_MOUSEWHEEL)
    {
        if (tool_type_ == UNKNOW)
        {
            return S_OK;
        }

        if (uMsg == WM_MOUSEWHEEL)
        {
            if (delegate_)
                delegate_->OnMouseWheelMessage(uMsg, wParam, lParam);
        }
    }

    LRESULT lRes = 0;
    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void MainCanvas::SelectTools(ToolTypes tool_type)
{
    tool_type_ = tool_type;

    switch (tool_type)
    {
    case NORMAL:
        task_view_ = NULL;
        break;
    case SELECT:
        task_view_ = select_task_;
        break;
    case PENCILE:
        task_view_ = polyline_task_;
        break;
    case TEXT:
        task_view_ = text_task_;
        break;
    case ERASER:
        task_view_ = eraser_task_;
        break;
    case CANVAS:
        task_view_ = canvas_task_;
        break;
    case RECTANGLE:
    case ELLIPSE:
    case DAIMOND:
    case TRIANGLE:
    case LINE:
    case ARROW:
        rect_task_->SetTaskStatus(tool_type);
        task_view_ = rect_task_;
        break;
    default:
        task_view_ = NULL;
        break;
    }

    if (task_view_)
    {
        task_view_->ClearContext();
    }
}


void MainCanvas::ResetCanvas(int width, int height)
{  
    coordinate_system_.SetWorldSize(width, height);
    ResizeAllElemsInCanvas();
}

void MainCanvas::OnSize()
{
    ResizeAllElemsInCanvas();
}


void MainCanvas::SetOffset(int offsety, bool scroll)
{   
    coordinate_system_.SetOffset(offsety);
    if (m_pCanvasLayout)
    {
        SIZE sz = m_pCanvasLayout->GetScrollPos();
        sz.cy = offsety* coordinate_system_.GetSacle();
        //scale_;
        m_pCanvasLayout->SetScrollPos(sz);
        tools_manager_.SetOffsetY(offsety, scroll);
    }
      
}


LRESULT MainCanvas::TranslateAcceleratorW(MSG* pMsg)
{
    return S_FALSE;
}



void MainCanvas::AddImage(char *url, char *filepath, char *info)
{
    CImageToolUI *pImage = tools_manager_.AddImage(m_pDesktopCanvasContainer, url, filepath, info);
    if (pImage == nullptr)
        return;
    if (info==NULL && delegate_)
    {
        //delegate_->OnAddImage(url, pImage->MarshalString().c_str());
    }
}


void MainCanvas::SetDelegate(CanvasView::Delegate *delegate)
{
    delegate_ = delegate;
}


ToolsManager* MainCanvas::GetToolsManager()
{
    return &tools_manager_;
}

CPaintManagerUI* MainCanvas::GetPaintMangerUI()
{
    return &m_pm;
}



void MainCanvas::OnDeletePen(CControlUI *pControl)
{   
    std::vector<std::string> vtUuid;
    tools_manager_.OnDeletePen(vtUuid, pControl);

    InnerPenInfo penInfo;
    penInfo.state = (PenState)PEN_STATE_EREASE;   
    if (delegate_)
        delegate_->SendPenPath(penInfo);
}

void MainCanvas::OnControlAdded(CControlUI* control)
{
    ToolBase* tool_base = dynamic_cast<ToolBase*>(control);
    Json::Value root = tool_base->Marshal();
    Json::FastWriter fast_writer;
    std::string str = fast_writer.write(root);

    OnControlChanged();

    if (delegate_)
        delegate_->OnControlChange(ChangeType::ADD_TYPE, str.c_str());
}

void MainCanvas::OnTextChanged(CControlUI* control)
{
    ToolBase* tool_base = dynamic_cast<ToolBase*>(control);
    
    Json::Value controls;
    if (tool_base)
    {
        Json::Value data = tool_base->Marshal();
        controls.append(data);
    }

    Json::FastWriter fast_writer;

    std::string text_change_info = fast_writer.write(controls);

    OnControlChanged();

    if (delegate_)
        delegate_->OnControlChange(ChangeType::MODEFY_TYPE, text_change_info.c_str());
}

void MainCanvas::OnTextControlRemoved(CControlUI* control)
{
    ToolBase* tool_base = dynamic_cast<ToolBase*>(control);
    Json::Value root = tool_base->Marshal();
    text_task_->ClearContext();
    Json::Value controls;
    std::string uuid = tool_base->GetControlUuid();
    controls.append(uuid);

    Json::FastWriter fast_writer;
    std::string delete_controls_info = fast_writer.write(controls);

    GetToolsManager()->Remove(control);
    GetCanvasContainer()->Remove(control);
    OnControlChanged();

    if (delegate_)
        delegate_->OnControlChange(ChangeType::DELETE_TYPE, delete_controls_info.c_str());
}



void MainCanvas::OnControlModified()
{
    Json::Value root = tools_manager_.MarshalModifiedControls();
    Json::FastWriter fast_writer;
    std::string str = fast_writer.write(root);

    OnControlChanged();

    if (delegate_)
        delegate_->OnControlChange(ChangeType::MODEFY_TYPE, str.c_str());
}

void MainCanvas::OnControlRemoved()
{
    std::vector<CControlUI*> controls = GetToolsManager()->GetSelectedControlsList();
    for (size_t n = 0; n < controls.size(); n++)
    {
        tools_manager_.Remove(controls[n]);
        GetCanvasContainer()->Remove(controls[n]);
        
    }

    Json::Value root = tools_manager_.MarshalDeletedControls();
    Json::FastWriter fast_writer;
    std::string hello = fast_writer.write(root);

    OnControlChanged();

    if (delegate_)
        delegate_->OnControlChange(ChangeType::DELETE_TYPE, hello.c_str());
}

void MainCanvas::OnControlChanged()
{
    tools_manager_.OnControlChanged();
}



void MainCanvas::SendPenPath(int status, POINT pt)
{   
    double scale = coordinate_system_.GetSacle();
    InnerPenInfo penInfo;
    penInfo.state = (PenState)status;
    penInfo.x = pt.x / scale;
    penInfo.y = (pt.y + tools_manager_.offset_y_) / scale;
    if (delegate_)
        delegate_->SendPenPath(penInfo);
}

void MainCanvas::SetPenColor(DWORD color)
{
    pen_color_ = color;
}

DWORD MainCanvas::GetPenColor()
{
    return pen_color_;
}


void MainCanvas::SetPenSize(int pen_size)
{
    pen_size_ = pen_size;
}

int MainCanvas::GetPenSize()
{
    return pen_size_;
}

double MainCanvas::GetScale()
{
    return tools_manager_.scale_;
}

void MainCanvas::ClearControls()
{
    tools_manager_.ClearSelectedList();
    std::list<CControlUI*>controls = tools_manager_.GetCurrentPageControls();
    std::list<CControlUI*>::iterator it = controls.begin();
    for (; it != controls.end(); it++)
    {
        GetCanvasContainer()->Remove(*it);
    }
}


void MainCanvas::PageTo(int page)
{
    /*if (page_id_ == page)
        return;*/
    page_id_ = page;

    //清空当前的图像
    ClearControls();

    //跳页
    tools_manager_.PageTo(page);
    
    //重新绘制翻页后的图像
    Json::Value graphics = tools_manager_.GetPageGraphics(page);
    if (!graphics.isNull())
        UnMarshalPageGraphics(graphics);
    if (task_view_)
        task_view_->ClearContext();

}

void MainCanvas::UnMarshalGraphics(Json::Value& graphics)
{
    tools_manager_.UnMarshalAllTools(graphics);
    if (page_id_ != -1)
        PageTo(page_id_);
}


void MainCanvas::UnMarshalPageGraphics(Json::Value& graphics)
{
    Json::Value graphic;
    for (size_t n = 0; n < graphics.size(); n++)
    {
        graphic = graphics[n];
        int tool_type = graphic["shape"].asInt();
        DWORD color = graphic["color"].asInt64();
        int pen_size = graphic["width"].asInt();

#ifdef _DEBUG
        OutputDebugStringA(graphic.toStyledString().c_str());
        OutputDebugStringA("\r\n");
#endif

        CControlUI* control = ToolsFactory::CreateTools((ToolTypes)tool_type, color, pen_size, coordinate_system_.GetSacle());
        ToolBase* tool_base = dynamic_cast<ToolBase*>(control);
        if (tool_base)
        {
            if (tool_base->GetToolsType() == ToolTypes::TEXT)
            {
                DuiLibEx::CTextToolUI* text_tool = dynamic_cast<DuiLibEx::CTextToolUI*>(control);
                text_tool->SetMainCanvas(this);
                text_tool->SetChangeNotify(false);
            }

            GetCanvasContainer()->Add(control);
            tool_base->UnMarshal(graphic);
            tool_base->SetOffsetY(tools_manager_.GetOffset());
            tools_manager_.Add(control, false);

            if (tool_base->GetToolsType() == ToolTypes::TEXT)
            {
                DuiLibEx::CTextToolUI* text_tool = dynamic_cast<DuiLibEx::CTextToolUI*>(control);
                text_tool->SetChangeNotify(true);
                text_tool->SetToolsState(ToolsStats::TOOLS_NORMAL);
                if (!editable_)
                    text_tool->SetReadOnly(true);
                else
                    text_tool->SetReadOnly(false);
            }
            
        }
    }

    
}


Json::Value MainCanvas::MarshalGraphics()
{
    return tools_manager_.MarshalAllTools();
}

void MainCanvas::ChangeControls(ChangeType change_type, Json::Value changes)
{
#ifdef _DEBUG
    OutputDebugStringA(changes.toStyledString().c_str());
    OutputDebugStringA("\r\n");
#endif

    if (change_type == ChangeType::ADD_TYPE)
    {
        DoAddControl(changes);
    }
    else if (change_type == ChangeType::DELETE_TYPE)
    {
        RemoveControls(changes);
    }
    else if (change_type == ChangeType::MODEFY_TYPE)
    {
        DoModifyControl(changes);
    }
    OnControlChanged();
}


void MainCanvas::DoAddControl(Json::Value graphic)
{
    int tool_type = graphic["shape"].asInt();
    DWORD color = graphic["color"].asInt64();
    int pen_size = graphic["width"].asInt();

    CControlUI* control = ToolsFactory::CreateTools((ToolTypes)tool_type, color, pen_size, coordinate_system_.GetSacle());
    ToolBase* tool_base = dynamic_cast<ToolBase*>(control);
    if (tool_base)
    {
        if (tool_base->GetToolsType() == ToolTypes::TEXT)
        {
            DuiLibEx::CTextToolUI* text_tool = dynamic_cast<DuiLibEx::CTextToolUI*>(control);
            text_tool->SetMainCanvas(this);
            if (!editable_)
                text_tool->SetReadOnly(true);
            else
                text_tool->SetReadOnly(false);
        }

        GetCanvasContainer()->Add(control);
        tool_base->SetOffsetY(tools_manager_.GetOffset());
        tool_base->UnMarshal(graphic);
        tools_manager_.Add(control, false);
    }
}

void MainCanvas::DoModifyControls(Json::Value graphics)
{
    Json::Value graphic;
    for (size_t n = 0; n < graphics.size(); n++)
    {
        graphic = graphics[n];
        std::string uuid = graphic["uuid"].asString();
        CControlUI* control = tools_manager_.Get(uuid);
        if (control)
        {
            ToolBase* tool_base = dynamic_cast<ToolBase*>(control);
            if (tool_base)
            {
                tool_base->UnMarshal(graphic);
            }
        }
    }
}

void MainCanvas::DoModifyControl(Json::Value graphic)
{
    std::string uuid = graphic["uuid"].asString();
    CControlUI* control = tools_manager_.Get(uuid);
    if (control)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(control);
        if (tool_base)
        {
            tool_base->UnMarshal(graphic);
        }
    }
    
}



void MainCanvas::RemoveControls(Json::Value graphics)
{
    for (size_t n = 0; n < graphics.size(); n++)
    {
        std::string uuid = graphics[n].asString();
        CControlUI* control = tools_manager_.Get(uuid);

        if (control)
        {
            GetToolsManager()->Remove(control);
            GetCanvasContainer()->Remove(control);
        }
    }
}

void MainCanvas::SetCanvasFocus()
{
    m_pDesktopCanvasContainer->SetFocus();
}

void MainCanvas::SetEditable(bool enable)
{
    editable_ = enable;
}
