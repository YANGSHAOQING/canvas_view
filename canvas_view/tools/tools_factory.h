#pragma once
#include "tool.h"


class ToolsFactory
{
public:
    static CControlUI* CreateTools(ToolTypes tool_type, DWORD color, int pen_size, float scale);
};
