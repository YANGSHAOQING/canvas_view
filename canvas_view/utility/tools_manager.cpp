#include <StdAfx.h>
#include "tools_manager.h"
#include "tools/rectangle_tool.h"
#include "tools/arrow_tool.h"
#include "tools/ellipse_tool.h"
#include "tools/pencile_tool.h"
#include "tools/image_tool.h"
#include "tools/triangle_tool.h"
#include "tools/diamon_tool.h"
#include "tools/tool.h"
#include "tools/pen_icon_tool.h"
#include <Util.h>

ToolsManager::ToolsManager()
{
    offset_y_ = 0;
    scale_ = 1.0f;
    page_id_ = 0;
}

ToolsManager::~ToolsManager()
{
    
}

void ToolsManager::Add(CControlUI* control, bool is_new)
{
    ToolBase* tool_base = dynamic_cast<ToolBase*>(control);
    if (tool_base)
    {
        if (is_new)
        {
            std::string temp = GenUUID();
            std::string uuid = temp.substr(0, 4) + temp.substr(temp.size() - 4, temp.size());
            tool_base->SetControlUuid(uuid);
            current_page_controls_list_.push_back(control);
        }
        else
        {
            current_page_controls_list_.push_back(control);
        }
    }
      
}

void ToolsManager::Remove(CControlUI* control)
{
    current_page_controls_list_.remove(control);
}

std::string ToolsManager::GetUUID(CControlUI* control)
{   
    ToolBase* tool_base = dynamic_cast<ToolBase*>(control);
    if (tool_base == NULL)
        return "";
    return tool_base->GetControlUuid();
}


CControlUI* ToolsManager::GetControlUI(POINT pt)
{
    ControlListType::iterator it = current_page_controls_list_.begin();
    for (; it != current_page_controls_list_.end(); it++)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(*it);
        if (tool_base == NULL)
            continue;
        if (tool_base->ToolHitTest(pt) != -1)
            return *it;
    }
    return NULL;
}





ToolsStats ToolsManager::GetControlUISatus(CControlUI *pControl)
{
    if (pControl == NULL)
        return TOOLS_NORMAL;
    ToolBase* tool_base = dynamic_cast<ToolBase*>(pControl);
    if (tool_base == NULL)
        return TOOLS_NORMAL;
    return tool_base->GetToolsState();
}


bool ToolsManager::SetSelectedToolsOffset(int offset_x, int offset_y, RECT border)
{
    RECT controls_border = GetSelectedControlsBorder();
    int left = controls_border.left - border.left;
    int right = border.right - controls_border.right;
    int top = controls_border.top - border.top;
    int bottom = border.bottom - controls_border.bottom;
    
    //向右移动
    if (offset_x < 0)
    {
        if (left < -offset_x)
        {
            offset_x = -left;
        }
    }
    else
    {
        if (right < offset_x)
            offset_x = right;
    }
    //向上移动
    if (offset_y < 0)
    {
        if ( top < -offset_y)
        {
            offset_y = -top;
        }
    }
    else
    {
        if (bottom < offset_y)
        {
            offset_y = bottom;
        }
    }

    for (size_t n = 0; n < selected_controls_list_.size(); n++)
    {   
        ToolBase* tool_base = dynamic_cast<ToolBase*>(selected_controls_list_[n]);
        if (tool_base == NULL)
            continue;
        tool_base->SetToolsOffset(offset_x, offset_y);
    }
    return true;
}

void ToolsManager::ClearSelectedList()
{
    for (int i = 0; i < selected_controls_list_.size(); ++i)
    {
        if (std::find(current_page_controls_list_.begin(), current_page_controls_list_.end(), selected_controls_list_[i]) != current_page_controls_list_.end())
        {
            ToolBase* tool_base = dynamic_cast<ToolBase*>(selected_controls_list_[i]);
            if (tool_base != NULL)
            {
                tool_base->SetToolsState(TOOLS_NORMAL);
            }
        }
        
    }
    selected_controls_list_.clear();
}


void ToolsManager::DisableMouseAll()
{
    ControlListType::iterator it = current_page_controls_list_.begin();
    for (; it != current_page_controls_list_.end(); it++)
    {
        //it->second->SetMouseEnabled(false);
    }
}

void ToolsManager::EnableMouseAll()
{
    ControlListType::iterator it = current_page_controls_list_.begin();
    for (; it != current_page_controls_list_.end(); it++)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(*it);
        if (tool_base)
        {
            //it->second->SetMouseEnabled(true);
        }
    }
}


void ToolsManager::SetScale(double scale)
{
    scale_ = scale;

    ControlListType::iterator it = current_page_controls_list_.begin();
    for (; it != current_page_controls_list_.end(); it++)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(*it);
        if (tool_base)
        {
            tool_base->SetToolsScale(scale);
        }
    }
}

bool ToolsManager::SetOffsetY(int offset, bool scroll)
{   
    offset_y_ = offset;
    
    if (scroll)
    {
        ControlListType::iterator it = current_page_controls_list_.begin();
        for (; it != current_page_controls_list_.end(); ++it)
        {
            ToolBase* tool_base = dynamic_cast<ToolBase*>(*it);
            tool_base->SetOffsetY(offset_y_);
        }
    }

    return true;
}

int ToolsManager::GetOffset()
{
    return offset_y_;
}



void ToolsManager::ResizeAllElemsInCanvas(CControlUI *pControl,bool flag)
{
    ToolBase* tool_base = dynamic_cast<ToolBase*>(pControl);
    if (tool_base == NULL)
        return ;
    //todo: 重新设置窗口的位置
    //tool_base->ResetToolsMap(offset_y_, scale_, flag);
}

bool ToolsManager::ResizeAllElemsInCanvas()
{
    ControlListType::iterator it = current_page_controls_list_.begin();
    for (; it != current_page_controls_list_.end(); ++it)
    {      
        //ResizeAllElemsInCanvas(*it, true);
    }
    return true;
    
}


void ToolsManager::OnDeletePen(std::vector<std::string> &vtUuid, CControlUI *pControl)
{
    int index = 0, size = -1;
    if (pControl == NULL)
    {
        size = selected_controls_list_.size();
        if (size > 0)
            pControl = selected_controls_list_[0];
    }

    while (pControl)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(pControl);
        if (tool_base != NULL || tool_base->GetToolsType() == PENCILE)
        {
            vtUuid.push_back(tool_base->GetControlUuid());
        }
        if (++index < size)
        {
            pControl = selected_controls_list_[index];
        }
        else
            break;

    }
    

}



CImageToolUI *ToolsManager::AddImage(CContainerUI *pContainer, char *url,char *filepath, char *info)
{
    if (pContainer == NULL)
        return NULL;
    CImageToolUI *pImage = CImageToolUI::CreateTools();
    if (pImage == NULL)
        return NULL;

    SIZE size{ 100, 100 };
    if (info == NULL)
    {
        int width = 100;
        int height = 100;
        if (GetImageSize(AnsiToUnicode(info), width, height))
        {
            size.cx = width;
            size.cy = height;
        }
        pImage->Setfilepath(AnsiToUnicode(info));
        pImage->SetImageSize(size);
        pImage->SetBasePos(POINT{ 0, offset_y_ / scale_ }); 
        Add(pImage);
    }
    else
    {
        Add(pImage,false);
    }
    pImage->SetPicUrl(url);
    pContainer->Add(pImage);

    ResizeAllElemsInCanvas(pImage, true);
    return pImage;
}


void ToolsManager::AddToSelectedList(CControlUI* control)
{
    if (std::find(selected_controls_list_.begin(), 
        selected_controls_list_.end(), control) == selected_controls_list_.end())
        selected_controls_list_.push_back(control);
    UpdateSelectedListStatus();
}

void ToolsManager::RemoveFromSelectedList(CControlUI* control)
{
    std::vector<CControlUI*>::iterator it = std::find(selected_controls_list_.begin(), 
        selected_controls_list_.end(), control);
    if (it != selected_controls_list_.end())
    {
        SetControlUIStatus(control, ToolsStats::TOOLS_NORMAL);
        selected_controls_list_.erase(it);
    }
    UpdateSelectedListStatus();
}


int ToolsManager::GetSelectedListCount()
{
    return selected_controls_list_.size();
}

void ToolsManager::SelectControlsInRect(RECT rect)
{
    selected_controls_list_ = Search(rect);
    UpdateSelectedListStatus();
}



std::vector<CControlUI*> ToolsManager::Search(RECT selct_rc)
{
    std::vector<CControlUI*> selected_controls;
    ControlListType::iterator it = current_page_controls_list_.begin();
    for (; it != current_page_controls_list_.end(); it++)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(*it);
        if (tool_base == NULL)
            continue;
        RECT item_rc = (*it)->GetPos();
        if (item_rc.left >= selct_rc.left && item_rc.right <= selct_rc.right&&
            item_rc.top >= selct_rc.top && item_rc.bottom <= selct_rc.bottom)
        {
            selected_controls.push_back((*it));
        }
    }

    return selected_controls;
}

RECT ToolsManager::GetSelectedControlsBorder()
{
    RECT border = { 0, 0, 0, 0 };
    std::vector<CControlUI*>::iterator it = selected_controls_list_.begin();
    for ( size_t n = 0; it != selected_controls_list_.end(); it++,n++)
    {
        
        ToolBase* tool_base = dynamic_cast<ToolBase*>(*it);
        if (tool_base)
        {
            RECT item_rc = (*it)->GetPos();
            if (n == 0)
            {
                border = item_rc;
            }
            else
            {
                if (item_rc.left < border.left)
                    border.left = item_rc.left;
                if (item_rc.right > border.right)
                    border.right = item_rc.right;
                if (item_rc.top < border.top)
                    border.top = item_rc.top;
                if (item_rc.bottom > border.bottom)
                    border.bottom = item_rc.bottom;
            }
            
            
        }
    }

    return border;
}


bool ToolsManager::IsControlSelected(CControlUI* control)
{
    if (control == NULL)
        return false;

    if (std::find(selected_controls_list_.begin(), 
        selected_controls_list_.end(), 
        control) != selected_controls_list_.end())
    {
        return true;
    }
    return false;
}

void ToolsManager::UpdateSelectedListStatus()
{
    /*if (selected_controls_list_.size() == 1)
    {
    SetControlUIStatus(selected_controls_list_[0], TOOLS_EDITABLE);
    }
    else
    {
    for (size_t n = 0; n < selected_controls_list_.size(); n++)
    {
    SetControlUIStatus(selected_controls_list_[n], TOOLS_SELECTED);
    }
    }*/

    for (size_t n = 0; n < selected_controls_list_.size(); n++)
    {
        SetControlUIStatus(selected_controls_list_[n], TOOLS_EDITABLE);
    }
}



bool ToolsManager::SetControlUIStatus(CControlUI *control, ToolsStats status)
{
    ToolBase* tool_base = dynamic_cast<ToolBase*>(control);
    if (tool_base == NULL)
        return false;
    tool_base->SetToolsState(status);
    if (status == TOOLS_EDITABLE)
    {
        control->SetMouseEnabled(true);
    }
    else
    {
        control->SetMouseEnabled(false);
    }
    control->Invalidate();
    return true;
}

std::vector<CControlUI*> ToolsManager::GetSelectedControlsList()
{
    return selected_controls_list_;
}


Json::Value ToolsManager::MarshalModifiedControls()
{
    Json::Value controls;
    for (size_t n = 0; n < selected_controls_list_.size(); n++)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(selected_controls_list_[n]);
        if (tool_base)
        {
            Json::Value control = tool_base->Marshal();
            controls.append(control);
        }
    }

    return controls;
}


bool ToolsManager::UnMarshalModifiedControls(Json::Value controls)
{
    return true;
}


Json::Value ToolsManager::MarshalAllTools()
{
    Json::Value page_list;

    PageListType::iterator it = page_list_.begin();
    for(;it != page_list_.end(); it++)
    {
        Json::Value page;
        page["page_id"] = it->first;
        page["graphics"] = it->second;
        page_list.append(page);
    }

    return page_list;
}

bool ToolsManager::UnMarshalAllTools(Json::Value& root)
{
    /*OutputDebugStringA(root.toStyledString().c_str());
    OutputDebugStringA("\r\n");*/
    for (size_t n = 0; n < root.size(); n++)
    {
        Json::Value graphic = root[n];
        int page_id = graphic["page_id"].asInt();
        page_list_[page_id] = graphic["graphics"];
    }
    return true;
}


Json::Value ToolsManager::MarshalDeletedControls()
{
    Json::Value controls;
    for (size_t n = 0; n < selected_controls_list_.size(); n++)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(selected_controls_list_[n]);
        if (tool_base)
        {
            Json::Value control = tool_base->Marshal();

            std::string uuid = tool_base->GetControlUuid();
            controls.append(uuid);
            Remove(selected_controls_list_[n]);
        }
    }
    ClearSelectedList();
    return controls;
}

bool ToolsManager::UnMarshalDeletedControls(Json::Value controls)
{
    return true;
}

void ToolsManager::PageTo(int page)
{
    page_id_ = page;
    ClearSelectedList();
    current_page_controls_list_.clear();
}

std::list<CControlUI*> ToolsManager::GetCurrentPageControls()
{
    return current_page_controls_list_;
}

Json::Value ToolsManager::GetPageGraphics(int page_id)
{
    if (page_list_.find(page_id) == page_list_.end())
        return Json::Value::null;
    return page_list_[page_id];
}

void ToolsManager::OnControlChanged()
{
    Json::Value controls;
    ControlListType::iterator it = current_page_controls_list_.begin();
    for (; it != current_page_controls_list_.end(); it++)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(*it);
        if (tool_base)
        {
            Json::Value control = tool_base->Marshal();
            controls.append(control);
        }
    }

    Json::FastWriter fw;
    std::string hello = fw.write(controls);
    OutputDebugStringA(hello.c_str());
    OutputDebugStringA("\r\n");

    page_list_[page_id_] = controls;
}

CControlUI* ToolsManager::Get(std::string uuid)
{
    ControlListType::iterator it = current_page_controls_list_.begin();
    for (; it != current_page_controls_list_.end(); it++)
    {
        ToolBase* tool_base = dynamic_cast<ToolBase*>(*it);
        if (tool_base)
        {
            if (tool_base->GetControlUuid() == uuid)
                return *it;
        }
    }
    return NULL;
}
