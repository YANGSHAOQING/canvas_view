#pragma once

typedef unsigned long DWORD;

enum ToolTypes
{
    UNKNOW = -1,
    RECTANGLE,//矩形
    ELLIPSE, //椭圆
    LINE, //直线
    ARROW, //箭头
    POLYGON,
    TEXT, //文本
    TRIANGLE, //三角行
    DAIMOND, //菱形
    IMAGE, //图片
    PENCILE, //笔迹
    ERASER,//橡皮
    NORMAL, //待选 此时鼠标事件不处理
    SELECT, //选择工具
    CANVAS,//画布模式下会把鼠标当作铅笔，同时回调笔迹信息
};

enum PenState  //最多16种
{
    PEN_STATE_UP = 0,  //现在标志是传输滚轮滚动后的位置PEN_SCROLL
    PEN_STATE_DOWN, 
    PEN_STATE_HOVER,
    PEN_STATE_SCRLL,    //滚动操作
    PEN_STATE_EREASE,

};

struct InnerPenInfo
{
    PenState  state;
    int          x;
    int          y;
    DWORD       ts;
    InnerPenInfo()
    {
        state = PEN_STATE_HOVER;
        x = 0;
        y = 0;
        ts = 0;

    }
};
enum ChangeType
{  
    ADD_TYPE,    //添加
    MODEFY_TYPE, //修改
    DELETE_TYPE,  //删除
};

enum ToolsStats{

    TOOLS_NORMAL,   //常态
    TOOLS_HOVER,    //选中可以进行移动 本身不接受鼠标事件
    TOOLS_EDITABLE, //可编辑的状态  
    TOOLS_SELECTED, //本身接受鼠标事件
    TOOLS_DELETE    //删除状态
};


struct ImageInfo
{
    int type; //0 添加要回调 1 添加不需要回调 
    char *uuid;
    int  x;
    int  y;
    int  width;
    int  height;
    char *url;
    char *localpath;
    ImageInfo()
    {
        x = 0;
        y = 0;
        width = 100;
        height = 100;
        url = nullptr;
        localpath = nullptr;

    }
};