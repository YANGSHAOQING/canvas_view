#pragma once
#include "tools_types.h"

#if defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
#include <string>

class EXPORT CanvasView
{
public:
    //回调第三方的接口
    class Delegate
    {
    public:
        //控件的更改 添加 删除 移动等操作
        virtual void OnControlChange(ChangeType type,const char *context) {};

        virtual void OnAddImage(const char*url, const char *context) {};

        //发送添加的笔迹数据给第三方调用者
        virtual void SendPenPath(InnerPenInfo &penInfo) {};

        //鼠标以及按键事件的回传
        virtual void OnMouseKeyMessage(unsigned int uMsg, unsigned int wParam,long lParam) {};

        virtual void OnMouseWheelMessage(unsigned int uMsg, unsigned int wParam, long lParam) {};

        virtual ~Delegate(){}

    };

    //供第三方调用的接口剖
public: 
    virtual ~CanvasView(){}

    virtual void SetPenColor(DWORD color) = 0;

    virtual void SetPenSize( int pen_size ) = 0;

    //设置选择的工具
    virtual void SelectTools(ToolTypes tool_type) = 0;

    //创建画布
    virtual bool CreateCanvas(int width, int height) = 0;

    //重新设置映射范围 
    virtual void ResetCanvas(int width, int height) = 0;

    //设置画板的位置
    virtual bool SetCanvasViewPos(int x, int y, int width, int height) = 0;

    virtual long GetCanvasViewHwnd() = 0;
    
    virtual void PageTo(int page) = 0;

    //设置纵向偏移量  scroll  true 滚动条引起的 false 翻页引起的
    virtual void SetOffset(int offsety, bool scroll=true) = 0;

    virtual void AddImage(char *url, char *filepath,char *info) = 0;

    virtual void Hide() = 0;
    virtual void Show() = 0;

    virtual void UnMarshal(std::string grapics_data) = 0;
    virtual std::string Marshal() = 0;

    virtual void ChangeControls(ChangeType type, std::string change_infos) = 0;

    virtual void SetEditable(bool enable) = 0;

};

EXPORT CanvasView* CreateCanvasView(void* hwnd = nullptr, CanvasView::Delegate* delegate = nullptr);
EXPORT void DestroyCanvasView(CanvasView* screen_shot);



