#pragma once

class CPenIconUI :
    public CContainerUI,
    public INotifyUI
{
public:
    CPenIconUI();
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;
    void DoInit() override;
    void Notify(TNotifyUI& msg) ;

    void PaintBorder(HDC hDC);
    void SetBasePos(POINT pt);
    void SetNameText(std::wstring name);
    void SetIconImage(int clrIndex);
    void SetPenIconHide();

    void SetImageSize(int w, int h){ icon_size_.cx = w; icon_size_.cy = h; }
    void SetNameSize(int w, int h){ name_size_.cx = w; name_size_.cy = h; }

    static std::wstring getPenIconPath(int clrIndex);
    static  CPenIconUI* CreateTools();
public:
    POINT base_pos_;
    SIZE  icon_size_;
    SIZE  name_size_;
    CLabelUI *lable_icon_;
    CLabelUI *lable_name_;
    DWORD         last_showtime_;

};
