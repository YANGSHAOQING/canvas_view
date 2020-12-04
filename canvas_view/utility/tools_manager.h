#pragma once
#include <map>
#include <list>
#include "tools_types.h"
#include <tools/pencile_tool.h>
#include <tools/pen_icon_tool.h>
#include <tools/image_tool.h>

using namespace DuiLib;


//typedef map<std::string, CControlUI*> ControlListType;

//typedef std::map<int, std::map<int, CControlUI*>> ControlListType;

//typedef std::map<int, std::list<CControlUI*>> ControlListType;

typedef std::list<CControlUI*> ControlListType;
typedef std::map<int, Json::Value> PageListType;


class ToolsManager
{
public:
    ToolsManager();
    ~ToolsManager();

    void Add(CControlUI* control, bool is_new = true);
    void Remove(CControlUI* control);
    CControlUI* Get(std::string uuid);
    std::string GetUUID(CControlUI* control);
    ControlListType current_page_controls_list_;
 
public: //屏幕的映射

    int   offset_y_;  //纵向偏移量
    float scale_;     //缩放比例
    int page_id_;

    /* 设置纵向偏移量*/
    bool SetOffsetY(int offset,bool scroll);
    void SetScale(double scale);
    void PageTo(int page);
    int GetOffset();

    /*指定空间进行映射
      flag  true   将世界坐标映射成屏幕坐标 
            false  将屏幕坐标映射成世界坐标
    */
    void ResizeAllElemsInCanvas(CControlUI *pControl, bool flag);

    /*   将所有控件映射屏幕坐标下*/
    bool ResizeAllElemsInCanvas();
public:
    //删除画笔的数据
    void OnDeletePen(std::vector<std::string> &vtUuid, CControlUI *pControl);
    CImageToolUI *AddImage(CContainerUI *pContainer, char *url,char *filepath, char *Info);

public:

    ToolsStats GetControlUISatus(CControlUI *pControl);
    //通过鼠标获取元素
    CControlUI* GetControlUI(POINT pt);

    void EnableMouseAll();
    void DisableMouseAll();
    
    //识别得到的数据
    std::vector<CControlUI*> selected_controls_list_;
    vector<CControlUI*> Search(RECT selct_rc);
    //设置识别到的元素的移动
    bool SetSelectedToolsOffset(int offset_x, int offset_y, RECT border);
    //添加控件到控件选中列表
    void AddToSelectedList(CControlUI* control);
    //移除控件从选中列表中
    void RemoveFromSelectedList(CControlUI* control);
    //移除所有控件从选中列表中
    void ClearSelectedList();
    //获取选中列表中控件个数
    int GetSelectedListCount();
    //选中区域内所有控件
    void SelectControlsInRect(RECT rect);
    //控件是否被选中
    bool IsControlSelected(CControlUI* control);
    //更新所有选中控件状态
    void UpdateSelectedListStatus();
    //更新单个控件状态，会引起UI改变
    bool SetControlUIStatus(CControlUI *pControl, ToolsStats status);
    //获取选中的图像
    std::vector<CControlUI*> GetSelectedControlsList();
    //获取所有选中控件的外边框
    RECT GetSelectedControlsBorder();



    //序列化状态改变的小工具
    Json::Value MarshalModifiedControls();
    //反序列化状态改变的小工具，用来被动改变状态
    bool UnMarshalModifiedControls(Json::Value controls);

    //序列化被删除的小工具
    Json::Value MarshalDeletedControls();
    //反序列化被删除的小工具
    bool UnMarshalDeletedControls(Json::Value controls);


    //序列化所有的小工具，用来保存现场
    Json::Value MarshalAllTools();
    //反序列化所有小工具，用来恢复现场
    bool UnMarshalAllTools(Json::Value& root);

    void OnControlChanged();

    std::list<CControlUI*> GetCurrentPageControls();

    Json::Value GetPageGraphics( int page_id);

    PageListType page_list_;
    

};