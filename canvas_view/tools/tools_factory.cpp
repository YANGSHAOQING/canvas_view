#include "stdafx.h"
#include "tools_factory.h"
#include "rectangle_tool.h"
#include "ellipse_tool.h"
#include "pencile_tool.h"
#include "arrow_tool.h"
#include "image_tool.h"
#include "triangle_tool.h"
#include "diamon_tool.h"
#include "text_tool.h"
#include "line_tool.h"


CControlUI* ToolsFactory::CreateTools(ToolTypes tool_type, DWORD color, int pen_size, float scale)
{
    CControlUI* tool = nullptr;
    switch (tool_type)
    {
    case RECTANGLE:
        //tool = CTriangleToolUI::CreateTools(color, 5, scale);
        tool = CRectangleToolUI::CreateTools(color, pen_size, scale);
        break;
    case ELLIPSE:
        tool = CEllipseToolUI::CreateTools(color, pen_size, scale);
        break;
    case PENCILE:
        //tool = CPencileUI::CreateTools(color, pen_size, scale);
        break;
    case ARROW:
        tool = CArrowToolUI::CreateTools(color, pen_size, scale);
        break;
    case TEXT:
        //tool = CTextToolUI::CreateTools(color, pen_size, scale);
        tool = DuiLibEx::CTextToolUI::CreateTools(color, pen_size, scale);
        
        break;
    case IMAGE:
        //tool = CImageToolUI::CreateTools(color, pen_size, scale);
        break;
    case TRIANGLE:
        tool = CTriangleToolUI::CreateTools(color, pen_size, scale);
        break;
    case DAIMOND:
        tool = CDiamondToolUI::CreateTools(color, pen_size, scale);
        break;
    case LINE:
        tool = CLineToolUI::CreateTools(color, pen_size, scale);
        break;
    case ERASER:
        break;
    default:
        break;

    }

    ToolBase* tool_base = dynamic_cast<ToolBase*>(tool);
    if (tool_base)
        tool_base->SetToolsType(tool_type);

    return tool;
}
