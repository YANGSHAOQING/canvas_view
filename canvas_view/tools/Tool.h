#pragma once
#include "jsoncpp/json.h"
#include "tools_types.h"

class MainCanvas;

class ToolBase
{
public:
    ToolBase();
    virtual ~ToolBase();

    void SetToolsType(ToolTypes type);

    ToolTypes GetToolsType();

    std::string GetControlUuid();

    void SetControlUuid(std::string uuid);
    
public:
    //将元素信息转换成json串
    virtual Json::Value Marshal();

    virtual bool UnMarshal(Json::Value &tool);

    //设置元素移动的偏移量
    virtual void SetToolsOffset(int offsetx, int offsety){}
    
    //设置元素的参照值(针对滚动条纵向值)
    virtual void SetOffsetY(int basey) = 0;

    //设置元素的缩放比例
    virtual void SetToolsScale(float scale) = 0;

    //根据点进行元素是否选中
    //-1 未选中
    // 0    1    2
    // 3    4    5
    // 6    7    8
    virtual int ToolHitTest(POINT pt) = 0;
    
    //设置元素的选择状态(单击选中状态)
    virtual void SetToolsState(ToolsStats status) = 0;
    
    //得到元素是否框选中
    virtual ToolsStats GetToolsState(){ return tools_status_; }

    //设置画笔/字体的颜色
    virtual void SetPenColor(DWORD pen_color);
    
    //设置画笔/字体的大小
    virtual void SetPenSize(int pen_size);

    RECT DisplayToWorld(RECT rt);
    RECT WorldToDisplay(RECT rt);

    RECT GetDisplay();

    virtual void SetPosition(RECT rt) {};

protected:

    // 元素的uuid
    std::string control_uuid_;

    //元素的缩放比
    float scale_;
 
    //屏幕映射的纵向最小值
    int  base_y_;

    int offset_y_;

    int left_;
    int top_;
    int right_;
    int bottom_;

    //世界坐标
    RECT world_rect_;

    //颜色
    DWORD pen_color_;

    //画笔的大小或者是字体的大小
    int pen_size_;

    //元素的状态
    ToolsStats tools_status_;

    //元素的类型
    ToolTypes type_;
    
};