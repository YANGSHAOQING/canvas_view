#ifndef __UIRICHEDIT3_H__
#define __UIRICHEDIT3_H__

#pragma once
#include <Imm.h>
#pragma comment(lib,"imm32.lib")
#include "tool.h"

namespace DuiLibEx {

class CTxtWinHost;

class CTextToolUI : 
    public CContainerUI, 
    public IMessageFilterUI,
    public INotifyUI,
    public ToolBase
{
public:

    CTextToolUI();
    ~CTextToolUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    bool IsWantTab();
    void SetWantTab(bool bWantTab = true);
    bool IsWantReturn();
    void SetWantReturn(bool bWantReturn = true);
    bool IsWantCtrlReturn();
    void SetWantCtrlReturn(bool bWantCtrlReturn = true);
    bool IsRich();
    void SetRich(bool bRich = true);
    bool IsReadOnly();
    void SetReadOnly(bool bReadOnly = true);
    bool GetWordWrap();
    void SetWordWrap(bool bWordWrap = true);
    int GetFont();
    void SetFont(int index);
    void SetFontSize(int size);
    void SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
	void SetEnabled(bool bEnabled);
    LONG GetWinStyle();
    void SetWinStyle(LONG lStyle);
    DWORD GetTextColor();
    void SetTextColor(DWORD dwTextColor);
    int GetLimitText();
    void SetLimitText(int iChars);
    long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;
    CDuiString GetText() const;
    void SetText(LPCTSTR pstrText);
    bool GetModify() const;
    void SetModify(bool bModified = true) const;
    void GetSel(CHARRANGE &cr) const;
    void GetSel(long& nStartChar, long& nEndChar) const;
    int SetSel(CHARRANGE &cr);
    int SetSel(long nStartChar, long nEndChar);
    void ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo);
    void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false);
    CDuiString GetSelText() const;
    int SetSelAll();
    int SetSelNone();
    WORD GetSelectionType() const;
    bool GetZoom(int& nNum, int& nDen) const;
    bool SetZoom(int nNum, int nDen);
    bool SetZoomOff();
    bool GetAutoURLDetect() const;
    bool SetAutoURLDetect(bool bAutoDetect = true);
    DWORD GetEventMask() const;
    DWORD SetEventMask(DWORD dwEventMask);
    CDuiString GetTextRange(long nStartChar, long nEndChar) const;
    void HideSelection(bool bHide = true, bool bChangeStyle = false);
    void ScrollCaret();
    int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false);
    int AppendText(LPCTSTR lpstrText, bool bCanUndo = false);
    DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;
    bool SetDefaultCharFormat(CHARFORMAT2 &cf);
    DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;
    bool SetSelectionCharFormat(CHARFORMAT2 &cf);
    bool SetWordCharFormat(CHARFORMAT2 &cf);
    DWORD GetParaFormat(PARAFORMAT2 &pf) const;
    bool SetParaFormat(PARAFORMAT2 &pf);
    bool Redo();
    bool Undo();
    void Clear();
    void Copy();
    void Cut();
    void Paste();
    int GetLineCount() const;
    CDuiString GetLine(int nIndex, int nMaxLength) const;
    int LineIndex(int nLine = -1) const;
    int LineLength(int nLine = -1) const;
    bool LineScroll(int nLines, int nChars = 0);
	CPoint GetCharPos(long lChar) const;
    long LineFromChar(long nIndex) const;
    CPoint PosFromChar(UINT nChar) const;
    int CharFromPos(CPoint pt) const;
    void EmptyUndoBuffer();
    UINT SetUndoLimit(UINT nLimit);
    long StreamIn(int nFormat, EDITSTREAM &es);
    long StreamOut(int nFormat, EDITSTREAM &es);
	void SetAccumulateDBCMode(bool bDBCMode);
	bool IsAccumulateDBCMode();

    void DoInit();
    // 注意：TxSendMessage和SendMessage是有区别的，TxSendMessage没有multibyte和unicode自动转换的功能，
    // 而richedit2.0内部是以unicode实现的，在multibyte程序中，必须自己处理unicode到multibyte的转换
	bool SetDropAcceptFile(bool bAccept);
    virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const; 
    IDropTarget* GetTxDropTarget();
    virtual bool OnTxViewChanged();
    virtual void OnTxNotify(DWORD iNotify, void *pv);

    void SetScrollPos(SIZE szPos);
    void LineUp();
    void LineDown();
    void PageUp();
    void PageDown();
    void HomeUp();
    void EndDown();
    void LineLeft();
    void LineRight();
    void PageLeft();
    void PageRight();
    void HomeLeft();
    void EndRight();

    SIZE EstimateSize(SIZE szAvailable);
    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    void DoPaint(HDC hDC, const RECT& rcPaint);

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);
	void PaintStatusImage(HDC hDC);
	RECT GetTextPadding() const;
	void SetTextPadding(RECT rc);

	void SetTipValue(LPCTSTR pStrTipValue);
	LPCTSTR GetTipValue();
	void SetTipValueColor(LPCTSTR pStrColor);
	DWORD GetTipValueColor();

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);


    bool ShowCaret(bool bShow);
    bool SetCaretPos(int x, int y);
    bool CreateCaret(HBITMAP hBmp, int nWidth, int nHeight);
    void DrawCaret(HDC hDC, const RECT& rcPaint);


    static CTextToolUI* CreateTools(DWORD color = 0xffff0000, int pen_size = 1, float scale = 1.0);
    void SetMainCanvas(MainCanvas* main_canvas);

    Json::Value Marshal() override;
    bool UnMarshal(Json::Value& tool) override;

    void SetToolsState(ToolsStats status) override;

    void SetToolsScale(float scale) override;

    void SetToolsOffset(int offsetx, int offsety) override;

    int ToolHitTest(POINT pt) override;

    RECT GetMinRect();
    void GetTextSize(RECT &rect, wstring text);

    void Notify(TNotifyUI& msg);
    void ResizeTextTool();


    void PaintBorder(HDC hDC);
    RECT GetSizerRect(int iIndex);

    void SetChangeNotify(bool enable);

    void SetOffsetY(int offset_y) override;

    void SetFocusOuter(bool focus);


protected:
    CTxtWinHost* m_pTwh;
    bool m_bVScrollBarFixing;
    bool m_bWantTab;
    bool m_bWantReturn;
    bool m_bWantCtrlReturn;
    bool m_bRich;
    bool m_bReadOnly;
    bool m_bWordWrap;
    DWORD m_dwTextColor;
    int m_iFont;
    int m_iLimitText;
    LONG m_lTwhStyle;
	bool m_bInited;
	bool  m_fAccumulateDBC ; // TRUE - need to cumulate ytes from 2 WM_CHAR msgs
	// we are in this mode when we receive VK_PROCESSKEY
	UINT m_chLeadByte; // use when we are in _fAccumulateDBC mode

	UINT m_uButtonState;
	CDuiString m_sNormalImage;
	CDuiString m_sHotImage;
	CDuiString m_sFocusedImage;
	CDuiString m_sDisabledImage;
	RECT m_rcTextPadding;
	CDuiString m_sTipValue;
	DWORD m_dwTipValueColor;

    RECT m_rtCaret;
    bool m_bCaretActive;
    bool m_bCaretShowing;
    CTextToolUI* m_currentCaretObject;
    const UINT kCaretTimerID = 0xF1;

    MainCanvas* main_canvas_;
    int min_width_;
    POINT start_;
    bool show_border_;

    bool change_notify_enable_;
};

} // namespace DuiLib

#endif // __UIRICHEDIT_H__
