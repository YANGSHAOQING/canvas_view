#include "StdAfx.h"
#include "tool.h"

ToolBase::ToolBase()
{
    tools_status_ = TOOLS_NORMAL;
    base_y_ = 0;
    scale_ = 1.0;
    world_rect_ = { 0 };
}

ToolBase::~ToolBase()
{
    
}

void ToolBase::SetToolsType(ToolTypes type)
{
    type_ = type;
}

ToolTypes ToolBase::GetToolsType()
{
    return type_;
}

void ToolBase::SetPenColor(DWORD pen_color)
{
    pen_color_ = pen_color;
}

std::string ToolBase::GetControlUuid()
{
    return control_uuid_;
}

void ToolBase::SetControlUuid(std::string uuid)
{
    control_uuid_ = uuid;
}

void ToolBase::SetPenSize(int pen_size)
{
    pen_size_ = pen_size;
}


Json::Value ToolBase::Marshal()
{
    Json::Value tool;
    tool["shape"] = type_;
    tool["uuid"] = control_uuid_;
    tool["color"] = (Json::Int64)pen_color_;
    tool["width"] = pen_size_;
    tool["left"] = left_;
    tool["top"] = top_;
    tool["right"] = right_;
    tool["bottom"] = bottom_;

    /*tool["left"] = world_rect_.left;
    tool["top"] = world_rect_.top;
    tool["right"] = world_rect_.right;
    tool["bottom"] = world_rect_.bottom;*/
    return tool;

}

bool ToolBase::UnMarshal(Json::Value &tool)
{  
    if (tool["left"].isNull() || tool["top"].isNull() 
        || tool["right"].isNull() || tool["bottom"].isNull())
        return false;

    control_uuid_ = tool["uuid"].asString();
    type_ = (ToolTypes)tool["shape"].asInt();
    pen_color_ = tool["color"].asInt64();
    pen_size_ = tool["width"].asInt();
    
    left_ = tool["left"].asInt();
    top_ = tool["top"].asInt();
    right_ = tool["right"].asInt();
    bottom_ = tool["bottom"].asInt();

    world_rect_.left = tool["left"].asInt();
    world_rect_.top = tool["top"].asInt();
    world_rect_.right = tool["right"].asInt();
    world_rect_.bottom = tool["bottom"].asInt();

    return true;
}

RECT ToolBase::DisplayToWorld(RECT disp)
{
    RECT world;
    world.left = disp.left / scale_;
    world.top = disp.top / scale_;
    world.right = disp.right / scale_;
    world.bottom = disp.bottom / scale_;
    return world;
}

RECT ToolBase::WorldToDisplay(RECT world)
{
    RECT disp;
    disp.left = world.left * scale_;
    disp.top = world.top * scale_;
    disp.right = world.right * scale_;
    disp.bottom = world.bottom * scale_;
    return disp;
}

RECT ToolBase::GetDisplay()
{
    RECT disp;
    disp.left = world_rect_.left * scale_;
    disp.top = world_rect_.top * scale_;
    disp.right = world_rect_.right * scale_;
    disp.bottom = world_rect_.bottom * scale_;
    return disp;

    /*RECT disp;
    disp.left = left_ * scale_;
    disp.top = top_ * scale_;
    disp.right = right_ * scale_;
    disp.bottom = bottom_ * scale_;
    return disp;*/
}



