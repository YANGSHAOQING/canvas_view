// screen_shot.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "canvas_view.h"
#include "canvas_view_impl.h"
#include "windows/main_canvas.h"

HINSTANCE dll_instance = nullptr;

BOOL WINAPI DllMain(HANDLE instance, ULONG reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        dll_instance = (HINSTANCE)instance;
        return TRUE;
    case DLL_PROCESS_DETACH:
        return TRUE;
    case DLL_THREAD_DETACH:
        return TRUE;
    default:
        return TRUE;
    };
}



CanvasViewImpl::CanvasViewImpl(void*hwnd, Delegate* delegate) :
delegate_(delegate)
{

    hwnd_ = (HWND)hwnd;

    Init();
    ::CoInitialize(NULL);
}

CanvasViewImpl::~CanvasViewImpl()
{
    if (main_canvas_)
        main_canvas_->Close();
    ::CoUninitialize();
}

void CanvasViewImpl::Init()
{
    CPaintManagerUI::SetInstance(dll_instance);
    //CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

    //USE RESOUCE ZIP SKIN
    HRSRC hResource = ::FindResource(dll_instance, MAKEINTRESOURCE(IDS_SKIN), _T("ZIP"));
    if (hResource == NULL)
        return;
    DWORD dwSize = 0;
    HGLOBAL hGlobal = ::LoadResource(dll_instance, hResource);
    if (hGlobal == NULL)
    {
#if defined(WIN32) && !defined(UNDER_CE)
        ::FreeResource(hResource);
#endif
        return;
    }
    dwSize = ::SizeofResource(dll_instance, hResource);
    if (dwSize == 0)
        return;
    CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);

    
}

bool CanvasViewImpl::CreateCanvas(int width, int height)
{
    MainCanvas*main_canvas = new MainCanvas;
    main_canvas->ResetCanvas(width, height);
    main_canvas->SetDelegate(delegate_);
    main_canvas->Create(hwnd_, _T(""), WS_VISIBLE | WS_POPUP, 0 , 0);
    //main_canvas->Create(hwnd_, L"", UI_WNDSTYLE_DIALOG | WS_VISIBLE, WS_EX_NOACTIVATE);
    main_canvas->ShowWindow(true, true);
    main_canvas_ = main_canvas;
    return true;
}

bool CanvasViewImpl::SetCanvasViewPos(int x, int y, int width, int height)
{
    if (main_canvas_ == NULL || main_canvas_->GetHWND() == NULL)
        return false;
    if (width < 20 || height < 20)
        return false;
    MoveWindow(main_canvas_->GetHWND(), x, y, width, height, true);

    main_canvas_->RefreshView();
    return true;
}


void CanvasViewImpl::SelectTools(ToolTypes tool_type)
{
    if (main_canvas_)
        main_canvas_->SelectTools(tool_type);
}

//ÉèÖÃ×ÝÏòÆ«ÒÆÁ¿
void CanvasViewImpl::SetOffset(int offsety, bool scroll)
{
    if (main_canvas_)
    {   
        main_canvas_->SetOffset(offsety,scroll);
    }
}


void CanvasViewImpl::UnMarshal(std::string grapics_data)
{
    if (main_canvas_)
    {
        Json::Value grapics;
        Json::Reader reader;
        if (reader.parse(grapics_data,grapics))
        {
            main_canvas_->UnMarshalGraphics(grapics);
        }
        
    }
        
}

std::string CanvasViewImpl::Marshal()
{
    if (main_canvas_)
    {
        Json::Value grapics = main_canvas_->MarshalGraphics();
        if (!grapics.isNull())
        {
            Json::FastWriter writer;
            std::string str_grapics = writer.write(grapics);
            return str_grapics;
        }
    }
    return "";
}




void CanvasViewImpl::AddImage(char *url, char *filepath, char *info)
{
    if (main_canvas_)
        main_canvas_->AddImage(url,filepath,info);
}


void CanvasViewImpl::Hide()
{
    ShowWindow(main_canvas_->GetHWND(), false);
}

void CanvasViewImpl::Show()
{
    ShowWindow(main_canvas_->GetHWND(), true);
    SetFocus(main_canvas_->GetHWND());
    main_canvas_->RefreshView();
}

void CanvasViewImpl::ResetCanvas(int width, int height)
{
    if (main_canvas_)
        main_canvas_->ResetCanvas(width, height);
} 

long CanvasViewImpl::GetCanvasViewHwnd()
{
    if (main_canvas_ && IsWindow(main_canvas_->GetHWND()))
        return (long)main_canvas_->GetHWND();

    return NULL;
}


void CanvasViewImpl::SetPenColor(DWORD color)
{
    if (main_canvas_)
        main_canvas_->SetPenColor(color);
}

void CanvasViewImpl::SetPenSize(int pen_size)
{
    if (main_canvas_)
        main_canvas_->SetPenSize(pen_size);
}

void CanvasViewImpl::PageTo(int page)
{
    if (main_canvas_)
        main_canvas_->PageTo(page);
}


void CanvasViewImpl::ChangeControls(ChangeType type, std::string change_infos)
{
    if (main_canvas_)
    {
        Json::Value changes;
        Json::Reader reader;
        if (reader.parse(change_infos, changes))
        {
            main_canvas_->ChangeControls(type, changes);
        }

    }
}

void CanvasViewImpl::SetEditable(bool enable)
{
    if (main_canvas_)
        main_canvas_->SetEditable(enable);
}





