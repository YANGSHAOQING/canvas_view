// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#if 0
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#endif
// TODO: reference additional headers your program requires here
#include <windows.h>
#include <objbase.h>
#include "Resource.h"

#include <UIlib.h>
using namespace DuiLib;

using namespace Gdiplus;



#define  DUI_CTR_CANVAS                          (_T("Canvas"))
#define  DUI_CTR_CANVASCONTAINER                 (_T("CanvasContainer"))
#define  DUI_CTR_RECTANGLETOOL                  (_T("RectangleTool"))
#define  DUI_CTR_TRIANGLETOOL                  (_T("TriangleTool"))
#define  DUI_CTR_SELECTTOOL                  (_T("SelectTool"))
#define  DUI_CTR_ELLIPSETOOL                  (_T("EllipseTool"))
#define  DUI_CTR_TEXTTOOL                       (_T("TextTool"))
#define  DUI_CTR_PENCILETOOL                       (_T("PenclieTool"))
#define  DUI_CTR_ARROWTOOL                       (_T("ArrowTool"))
#define  DUI_CTR_IMAGETOOL                       (_T("ImageTool"))
#define  DUI_CTR_PENICONTOOL                       (_T("PenIconTool"))
#define  DUI_CTR_DAIMONDTOOL                       (_T("DaimondTool"))
#define  DUI_CTR_LINETOOL                       (_T("LineTool"))


#include <Util.h>
