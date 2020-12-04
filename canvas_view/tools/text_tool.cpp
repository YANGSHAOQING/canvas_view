#include "stdafx.h"
#include "text_tool.h"
#include "windows/main_canvas.h"
#pragma comment(lib, "Msimg32.lib")

// These constants are for backward compatibility. They are the 
// sizes used for initialization and reset in RichEdit 1.0

namespace DuiLibEx {

const LONG cInitTextMax = (32 * 1024) - 1;

EXTERN_C const IID IID_ITextServices3 = { // 8d33f740-cf58-11ce-a89d-00aa006cadc5
    0x8d33f740,
    0xcf58,
    0x11ce,
    {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

EXTERN_C const IID IID_ITextHost3 = { /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
    0xc5bdd8d0,
    0xd26e,
    0x11ce,
    {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

#ifndef LY_PER_INCH
#define LY_PER_INCH 1440
#endif

#ifndef HIMETRIC_PER_INCH
#define HIMETRIC_PER_INCH 2540
#endif

#include <textserv.h>
#if 1
class CTxtWinHost : public ITextHost
{
public:
    CTxtWinHost();
    BOOL Init(CTextToolUI *re , const CREATESTRUCT *pcs);
    virtual ~CTxtWinHost();

    ITextServices* GetTextServices(void) { return pserv; }
    void SetClientRect(RECT *prc);
    RECT* GetClientRect() { return &rcClient; }
    BOOL GetWordWrap(void) { return fWordWrap; }
    void SetWordWrap(BOOL fWordWrap);
    BOOL GetReadOnly();
    void SetReadOnly(BOOL fReadOnly);
    void SetFont(HFONT hFont);
    void SetColor(DWORD dwColor);
    SIZEL* GetExtent();
    void SetExtent(SIZEL *psizelExtent);
    void LimitText(LONG nChars);
    BOOL IsCaptured();

    BOOL GetAllowBeep();
    void SetAllowBeep(BOOL fAllowBeep);
    WORD GetDefaultAlign();
    void SetDefaultAlign(WORD wNewAlign);
    BOOL GetRichTextFlag();
    void SetRichTextFlag(BOOL fNew);
    LONG GetDefaultLeftIndent();
    void SetDefaultLeftIndent(LONG lNewIndent);
    BOOL SetSaveSelection(BOOL fSaveSelection);
    HRESULT OnTxInPlaceDeactivate();
    HRESULT OnTxInPlaceActivate(LPCRECT prcClient);
    BOOL GetActiveState(void) { return fInplaceActive; }
    BOOL DoSetCursor(RECT *prc, POINT *pt);
    void SetTransparent(BOOL fTransparent);
    void GetControlRect(LPRECT prc);
    LONG SetAccelPos(LONG laccelpos);
    WCHAR SetPasswordChar(WCHAR chPasswordChar);
    void SetDisabled(BOOL fOn);
    LONG SetSelBarWidth(LONG lSelBarWidth);
    BOOL GetTimerState();

    void SetCharFormat(CHARFORMAT2W &c);
    void SetParaFormat(PARAFORMAT2 &p);

    // -----------------------------
    //	IUnknown interface
    // -----------------------------
    virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG _stdcall AddRef(void);
    virtual ULONG _stdcall Release(void);

    // -----------------------------
    //	ITextHost interface
    // -----------------------------
    virtual HDC TxGetDC();
    virtual INT TxReleaseDC(HDC hdc);
    virtual BOOL TxShowScrollBar(INT fnBar, BOOL fShow);
    virtual BOOL TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags);
    virtual BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw);
    virtual BOOL TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw);
    virtual void TxInvalidateRect(LPCRECT prc, BOOL fMode);
    virtual void TxViewChange(BOOL fUpdate);
    virtual BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);
    virtual BOOL TxShowCaret(BOOL fShow);
    virtual BOOL TxSetCaretPos(INT x, INT y);
    virtual BOOL TxSetTimer(UINT idTimer, UINT uTimeout);
    virtual void TxKillTimer(UINT idTimer);
    virtual void TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll);
    virtual void TxSetCapture(BOOL fCapture);
    virtual void TxSetFocus();
    virtual void TxSetCursor(HCURSOR hcur, BOOL fText);
    virtual BOOL TxScreenToClient (LPPOINT lppt);
    virtual BOOL TxClientToScreen (LPPOINT lppt);
    virtual HRESULT TxActivate( LONG * plOldState );
    virtual HRESULT TxDeactivate( LONG lNewState );
    virtual HRESULT TxGetClientRect(LPRECT prc);
    virtual HRESULT TxGetViewInset(LPRECT prc);
    virtual HRESULT TxGetCharFormat(const CHARFORMATW **ppCF );
    virtual HRESULT TxGetParaFormat(const PARAFORMAT **ppPF);
    virtual COLORREF TxGetSysColor(int nIndex);
    virtual HRESULT TxGetBackStyle(TXTBACKSTYLE *pstyle);
    virtual HRESULT TxGetMaxLength(DWORD *plength);
    virtual HRESULT TxGetScrollBars(DWORD *pdwScrollBar);
    virtual HRESULT TxGetPasswordChar(TCHAR *pch);
    virtual HRESULT TxGetAcceleratorPos(LONG *pcp);
    virtual HRESULT TxGetExtent(LPSIZEL lpExtent);
    virtual HRESULT OnTxCharFormatChange (const CHARFORMATW * pcf);
    virtual HRESULT OnTxParaFormatChange (const PARAFORMAT * ppf);
    virtual HRESULT TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits);
    virtual HRESULT TxNotify(DWORD iNotify, void *pv);
    virtual HIMC TxImmGetContext(void);
    virtual void TxImmReleaseContext(HIMC himc);
    virtual HRESULT TxGetSelectionBarWidth (LONG *lSelBarWidth);

private:
    CTextToolUI *m_re;
    ULONG	cRefs;					// Reference Count
    ITextServices	*pserv;		    // pointer to Text Services object
    // Properties

    DWORD		dwStyle;				// style bits

    unsigned	fEnableAutoWordSel	:1;	// enable Word style auto word selection?
    unsigned	fWordWrap			:1;	// Whether control should word wrap
    unsigned	fAllowBeep			:1;	// Whether beep is allowed
    unsigned	fRich				:1;	// Whether control is rich text
    unsigned	fSaveSelection		:1;	// Whether to save the selection when inactive
    unsigned	fInplaceActive		:1; // Whether control is inplace active
    unsigned	fTransparent		:1; // Whether control is transparent
    unsigned	fTimer				:1;	// A timer is set
    unsigned    fCaptured           :1;

    LONG		lSelBarWidth;			// Width of the selection bar
    LONG  		cchTextMost;			// maximum text size
    DWORD		dwEventMask;			// DoEvent mask to pass on to parent window
    LONG		icf;
    LONG		ipf;
    RECT		rcClient;				// Client Rect for this control
    SIZEL		sizelExtent;			// Extent array
    CHARFORMAT2W cf;					// Default character format
    PARAFORMAT2	pf;					    // Default paragraph format
    LONG		laccelpos;				// Accelerator position
    WCHAR		chPasswordChar;		    // Password character
};

// Convert Pixels on the X axis to Himetric
LONG DXtoHimetricX(LONG dx, LONG xPerInch)
{
    return (LONG) MulDiv(dx, HIMETRIC_PER_INCH, xPerInch);
}

// Convert Pixels on the Y axis to Himetric
LONG DYtoHimetricY(LONG dy, LONG yPerInch)
{
    return (LONG) MulDiv(dy, HIMETRIC_PER_INCH, yPerInch);
}

HRESULT InitDefaultCharFormat(CTextToolUI* re, CHARFORMAT2W* pcf, HFONT hfont) 
{
    memset(pcf, 0, sizeof(CHARFORMAT2W));
    LOGFONT lf;
    if( !hfont )
        hfont = re->GetManager()->GetFont(re->GetFont());
    ::GetObject(hfont, sizeof(LOGFONT), &lf);

    DWORD dwColor = re->GetTextColor();
	if(re->GetManager()->IsBackgroundTransparent())
	{
		CRenderEngine::CheckAlphaColor(dwColor);
	}
    pcf->cbSize = sizeof(CHARFORMAT2W);
    pcf->crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
    LONG yPixPerInch = GetDeviceCaps(re->GetManager()->GetPaintDC(), LOGPIXELSY);
    pcf->yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
    pcf->yOffset = 0;
    pcf->dwEffects = 0;
    pcf->dwMask = CFM_SIZE | CFM_OFFSET | CFM_FACE | CFM_CHARSET | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
    if(lf.lfWeight >= FW_BOLD)
        pcf->dwEffects |= CFE_BOLD;
    if(lf.lfItalic)
        pcf->dwEffects |= CFE_ITALIC;
    if(lf.lfUnderline)
        pcf->dwEffects |= CFE_UNDERLINE;
    pcf->bCharSet = lf.lfCharSet;
    pcf->bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
    _tcscpy(pcf->szFaceName, lf.lfFaceName);
#else
    //need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
    MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, pcf->szFaceName, LF_FACESIZE) ;
#endif

    return S_OK;
}

HRESULT InitDefaultParaFormat(CTextToolUI* re, PARAFORMAT2* ppf) 
{	
    memset(ppf, 0, sizeof(PARAFORMAT2));
    ppf->cbSize = sizeof(PARAFORMAT2);
    ppf->dwMask = PFM_ALL;
    ppf->wAlignment = PFA_LEFT;
    ppf->cTabCount = 1;
    ppf->rgxTabs[0] = lDefaultTab;

    return S_OK;
}

HRESULT CreateHost(CTextToolUI *re, const CREATESTRUCT *pcs, CTxtWinHost **pptec)
{
    HRESULT hr = E_FAIL;
    //GdiSetBatchLimit(1);

    CTxtWinHost *phost = new CTxtWinHost();
    if(phost)
    {
        if (phost->Init(re, pcs))
        {
            *pptec = phost;
            hr = S_OK;
        }
    }

    if (FAILED(hr))
    {
        delete phost;
    }

    return TRUE;
}

CTxtWinHost::CTxtWinHost() : m_re(NULL)
{
    ::ZeroMemory(&cRefs, sizeof(CTxtWinHost) - offsetof(CTxtWinHost, cRefs));
    cchTextMost = cInitTextMax;
    laccelpos = -1;
}

CTxtWinHost::~CTxtWinHost()
{
    pserv->OnTxInPlaceDeactivate();
    pserv->Release();
}

////////////////////// Create/Init/Destruct Commands ///////////////////////

BOOL CTxtWinHost::Init(CTextToolUI *re, const CREATESTRUCT *pcs)
{
    IUnknown *pUnk;
    HRESULT hr;

    //fTransparent = true;

    m_re = re;
    // Initialize Reference count
    cRefs = 1;

    // Create and cache CHARFORMAT for this control
    if(FAILED(InitDefaultCharFormat(re, &cf, NULL)))
        goto err;

    // Create and cache PARAFORMAT for this control
    if(FAILED(InitDefaultParaFormat(re, &pf)))
        goto err;

    // edit controls created without a window are multiline by default
    // so that paragraph formats can be
    dwStyle = ES_MULTILINE;

    // edit controls are rich by default
    fRich = re->IsRich();

    cchTextMost = re->GetLimitText();

    if (pcs )
    {
        dwStyle = pcs->style;

        if ( !(dwStyle & (ES_AUTOHSCROLL | WS_HSCROLL)) )
        {
            fWordWrap = TRUE;
        }
    }

    if( !(dwStyle & ES_LEFT) )
    {
        if(dwStyle & ES_CENTER)
            pf.wAlignment = PFA_CENTER;
        else if(dwStyle & ES_RIGHT)
            pf.wAlignment = PFA_RIGHT;
    }

    fInplaceActive = TRUE;

    // Create Text Services component
    //if(FAILED(CreateTextServices(NULL, this, &pUnk)))
    //    goto err;

	PCreateTextServices TextServicesProc;
	HMODULE hmod = LoadLibrary(_T("msftedit.dll"));
	if (hmod)
	{
		TextServicesProc = (PCreateTextServices)GetProcAddress(hmod,"CreateTextServices");
	}

	if (TextServicesProc)
	{
		HRESULT hr = TextServicesProc(NULL, this, &pUnk);
	}

    hr = pUnk->QueryInterface(IID_ITextServices,(void **)&pserv);

    // Whether the previous call succeeded or failed we are done
    // with the private interface.
    pUnk->Release();

    if(FAILED(hr))
    {
        goto err;
    }

    // Set window text
    if(pcs && pcs->lpszName)
    {
#ifdef _UNICODE		
        if(FAILED(pserv->TxSetText((TCHAR *)pcs->lpszName)))
            goto err;
#else
        size_t iLen = _tcslen(pcs->lpszName);
        LPWSTR lpText = new WCHAR[iLen + 1];
        ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
        ::MultiByteToWideChar(CP_ACP, 0, pcs->lpszName, -1, (LPWSTR)lpText, iLen) ;
        if(FAILED(pserv->TxSetText((LPWSTR)lpText))) {
            delete[] lpText;
            goto err;
        }
        delete[] lpText;
#endif
    }

    return TRUE;

err:
    return FALSE;
}

/////////////////////////////////  IUnknown ////////////////////////////////


HRESULT CTxtWinHost::QueryInterface(REFIID riid, void **ppvObject)
{
    HRESULT hr = E_NOINTERFACE;
    *ppvObject = NULL;

    if (IsEqualIID(riid, IID_IUnknown) 
        || IsEqualIID(riid, IID_ITextHost)) 
    {
        AddRef();
        *ppvObject = (ITextHost *) this;
        hr = S_OK;
    }

    return hr;
}

ULONG CTxtWinHost::AddRef(void)
{
    return ++cRefs;
}

ULONG CTxtWinHost::Release(void)
{
    ULONG c_Refs = --cRefs;

    if (c_Refs == 0)
    {
        delete this;
    }

    return c_Refs;
}

/////////////////////////////////  Far East Support  //////////////////////////////////////

HIMC CTxtWinHost::TxImmGetContext(void)
{
    return NULL;
}

void CTxtWinHost::TxImmReleaseContext(HIMC himc)
{
    //::ImmReleaseContext( hwnd, himc );
}

//////////////////////////// ITextHost Interface  ////////////////////////////

HDC CTxtWinHost::TxGetDC()
{
    return m_re->GetManager()->GetPaintDC();
}

int CTxtWinHost::TxReleaseDC(HDC hdc)
{
    return 1;
}

BOOL CTxtWinHost::TxShowScrollBar(INT fnBar, BOOL fShow)
{
    CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        pVerticalScrollBar->SetVisible(fShow == TRUE);
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        pHorizontalScrollBar->SetVisible(fShow == TRUE);
    }
    else if( fnBar == SB_BOTH ) {
        if( pVerticalScrollBar ) pVerticalScrollBar->SetVisible(fShow == TRUE);
        if( pHorizontalScrollBar ) pHorizontalScrollBar->SetVisible(fShow == TRUE);
    }
    return TRUE;
}

BOOL CTxtWinHost::TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags)
{
    if( fuSBFlags == SB_VERT ) {
        m_re->EnableScrollBar(true, m_re->GetHorizontalScrollBar() != NULL);
        m_re->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
    }
    else if( fuSBFlags == SB_HORZ ) {
        m_re->EnableScrollBar(m_re->GetVerticalScrollBar() != NULL, true);
        m_re->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
    }
    else if( fuSBFlags == SB_BOTH ) {
        m_re->EnableScrollBar(true, true);
        m_re->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
        m_re->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
    }
    return TRUE;
}

BOOL CTxtWinHost::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)
{
    CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        if( nMaxPos - nMinPos - rcClient.bottom + rcClient.top <= 0 ) {
            pVerticalScrollBar->SetVisible(false);
        }
        else {
            pVerticalScrollBar->SetVisible(true);
            pVerticalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.bottom + rcClient.top);
        }
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        if( nMaxPos - nMinPos - rcClient.right + rcClient.left <= 0 ) {
            pHorizontalScrollBar->SetVisible(false);
        }
        else {
            pHorizontalScrollBar->SetVisible(true);
            pHorizontalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.right + rcClient.left);
        }   
    }
    return TRUE;
}

BOOL CTxtWinHost::TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw)
{
    CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        pVerticalScrollBar->SetScrollPos(nPos);
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        pHorizontalScrollBar->SetScrollPos(nPos);
    }
    return TRUE;
}

void CTxtWinHost::TxInvalidateRect(LPCRECT prc, BOOL fMode)
{
    if( prc == NULL ) {
        m_re->GetManager()->Invalidate(rcClient);
        return;
    }
    RECT rc = *prc;
    m_re->GetManager()->Invalidate(rc);
}

void CTxtWinHost::TxViewChange(BOOL fUpdate) 
{
    if( m_re->OnTxViewChanged() ) m_re->Invalidate();
}

BOOL CTxtWinHost::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)
{
	if (m_re->GetManager()->IsBackgroundTransparent())
	{
		return m_re->CreateCaret(hbmp, xWidth, yHeight);
	}
	else
	{
		return ::CreateCaret(m_re->GetManager()->GetPaintWindow(), hbmp, xWidth, yHeight);
	}
}

BOOL CTxtWinHost::TxShowCaret(BOOL fShow)
{
	if (m_re->GetManager()->IsBackgroundTransparent())
	{
		//if(m_re->GetCurrentCaretObject() == m_re)
		{
			if((m_re->IsReadOnly() || !m_re->Activate()))
			{
				m_re->ShowCaret(false);
				return TRUE;
			}
		}

		return m_re->ShowCaret(fShow == TRUE);
	}
	else
	{
		if(fShow)
			return ::ShowCaret(m_re->GetManager()->GetPaintWindow());
		else
			return ::HideCaret(m_re->GetManager()->GetPaintWindow());
	}
}

BOOL CTxtWinHost::TxSetCaretPos(INT x, INT y)
{
	if (m_re->GetManager()->IsBackgroundTransparent())
	{
		m_re->SetCaretPos(x, y);
		return true;
	}
	else
		return ::SetCaretPos(x, y);

}

BOOL CTxtWinHost::TxSetTimer(UINT idTimer, UINT uTimeout)
{
    fTimer = TRUE;
    return m_re->GetManager()->SetTimer(m_re, idTimer, uTimeout) == TRUE;
}

void CTxtWinHost::TxKillTimer(UINT idTimer)
{
    m_re->GetManager()->KillTimer(m_re, idTimer);
    fTimer = FALSE;
}

void CTxtWinHost::TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll,	LPCRECT lprcClip,	HRGN hrgnUpdate, LPRECT lprcUpdate,	UINT fuScroll)	
{
    return;
}

void CTxtWinHost::TxSetCapture(BOOL fCapture)
{
    if (fCapture) m_re->GetManager()->SetCapture();
    else m_re->GetManager()->ReleaseCapture();
    fCaptured = fCapture;
}

void CTxtWinHost::TxSetFocus()
{
    m_re->SetFocus();
}

void CTxtWinHost::TxSetCursor(HCURSOR hcur,	BOOL fText)
{
    ::SetCursor(hcur);
}

BOOL CTxtWinHost::TxScreenToClient(LPPOINT lppt)
{
    return ::ScreenToClient(m_re->GetManager()->GetPaintWindow(), lppt);	
}

BOOL CTxtWinHost::TxClientToScreen(LPPOINT lppt)
{
    return ::ClientToScreen(m_re->GetManager()->GetPaintWindow(), lppt);
}

HRESULT CTxtWinHost::TxActivate(LONG *plOldState)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxDeactivate(LONG lNewState)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxGetClientRect(LPRECT prc)
{
    *prc = rcClient;
    GetControlRect(prc);
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetViewInset(LPRECT prc) 
{
    prc->left = prc->right = prc->top = prc->bottom = 0;
    return NOERROR;	
}

HRESULT CTxtWinHost::TxGetCharFormat(const CHARFORMATW **ppCF)
{
    *ppCF = &cf;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetParaFormat(const PARAFORMAT **ppPF)
{
    *ppPF = &pf;
    return NOERROR;
}

COLORREF CTxtWinHost::TxGetSysColor(int nIndex) 
{
	DWORD dwColor = ::GetSysColor(nIndex);
	CRenderEngine::CheckAlphaColor(dwColor);
    return dwColor;
}

HRESULT CTxtWinHost::TxGetBackStyle(TXTBACKSTYLE *pstyle)
{
    *pstyle = !fTransparent ? TXTBACK_OPAQUE : TXTBACK_TRANSPARENT;
    *pstyle = TXTBACK_TRANSPARENT;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetMaxLength(DWORD *pLength)
{
    *pLength = cchTextMost;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetScrollBars(DWORD *pdwScrollBar)
{
    *pdwScrollBar =  dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | 
        ES_AUTOHSCROLL | ES_DISABLENOSCROLL);

    return NOERROR;
}

HRESULT CTxtWinHost::TxGetPasswordChar(TCHAR *pch)
{
#ifdef _UNICODE
    *pch = chPasswordChar;
#else
    ::WideCharToMultiByte(CP_ACP, 0, &chPasswordChar, 1, pch, 1, NULL, NULL) ;
#endif
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetAcceleratorPos(LONG *pcp)
{
    *pcp = laccelpos;
    return S_OK;
} 										   

HRESULT CTxtWinHost::OnTxCharFormatChange(const CHARFORMATW *pcf)
{
    return S_OK;
}

HRESULT CTxtWinHost::OnTxParaFormatChange(const PARAFORMAT *ppf)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits) 
{
    DWORD dwProperties = 0;

    if (fRich)
    {
        dwProperties = TXTBIT_RICHTEXT;
    }

    if (dwStyle & ES_MULTILINE)
    {
        dwProperties |= TXTBIT_MULTILINE;
    }

    if (dwStyle & ES_READONLY)
    {
        dwProperties |= TXTBIT_READONLY;
    }

    if (dwStyle & ES_PASSWORD)
    {
        dwProperties |= TXTBIT_USEPASSWORD;
    }

    if (!(dwStyle & ES_NOHIDESEL))
    {
        dwProperties |= TXTBIT_HIDESELECTION;
    }

    if (fEnableAutoWordSel)
    {
        dwProperties |= TXTBIT_AUTOWORDSEL;
    }

    if (fWordWrap)
    {
        dwProperties |= TXTBIT_WORDWRAP;
    }

    if (fAllowBeep)
    {
        dwProperties |= TXTBIT_ALLOWBEEP;
    }

    if (fSaveSelection)
    {
        dwProperties |= TXTBIT_SAVESELECTION;
    }

    *pdwBits = dwProperties & dwMask; 
    return NOERROR;
}


HRESULT CTxtWinHost::TxNotify(DWORD iNotify, void *pv)
{
    if( iNotify == EN_REQUESTRESIZE ) {
        RECT rc;
        REQRESIZE *preqsz = (REQRESIZE *)pv;
        GetControlRect(&rc);
        rc.bottom = rc.top + preqsz->rc.bottom;
        rc.right  = rc.left + preqsz->rc.right;
        SetClientRect(&rc);
    }
    m_re->OnTxNotify(iNotify, pv);
    return S_OK;
}

HRESULT CTxtWinHost::TxGetExtent(LPSIZEL lpExtent)
{
    *lpExtent = sizelExtent;
    return S_OK;
}

HRESULT	CTxtWinHost::TxGetSelectionBarWidth (LONG *plSelBarWidth)
{
    *plSelBarWidth = lSelBarWidth;
    return S_OK;
}

void CTxtWinHost::SetWordWrap(BOOL fWordWrap)
{
    fWordWrap = fWordWrap;
    pserv->OnTxPropertyBitsChange(TXTBIT_WORDWRAP, fWordWrap ? TXTBIT_WORDWRAP : 0);
}

BOOL CTxtWinHost::GetReadOnly()
{
    return (dwStyle & ES_READONLY) != 0;
}

void CTxtWinHost::SetReadOnly(BOOL fReadOnly)
{
    if (fReadOnly)
    {
        dwStyle |= ES_READONLY;
    }
    else
    {
        dwStyle &= ~ES_READONLY;
    }

    pserv->OnTxPropertyBitsChange(TXTBIT_READONLY, 
        fReadOnly ? TXTBIT_READONLY : 0);
}

void CTxtWinHost::SetFont(HFONT hFont) 
{
    if( hFont == NULL ) return;
    LOGFONT lf;
    ::GetObject(hFont, sizeof(LOGFONT), &lf);
    LONG yPixPerInch = ::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSY);
    cf.yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
    if(lf.lfWeight >= FW_BOLD)
        cf.dwEffects |= CFE_BOLD;
    if(lf.lfItalic)
        cf.dwEffects |= CFE_ITALIC;
    if(lf.lfUnderline)
        cf.dwEffects |= CFE_UNDERLINE;
    cf.bCharSet = lf.lfCharSet;
    cf.bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
    _tcscpy(cf.szFaceName, lf.lfFaceName);
#else
    //need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
    MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, cf.szFaceName, LF_FACESIZE) ;
#endif

    pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
        TXTBIT_CHARFORMATCHANGE);
}

void CTxtWinHost::SetColor(DWORD dwColor)
{
	CRenderEngine::CheckAlphaColor(dwColor);
    cf.crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
    pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
        TXTBIT_CHARFORMATCHANGE);
}

SIZEL* CTxtWinHost::GetExtent() 
{
    return &sizelExtent;
}

void CTxtWinHost::SetExtent(SIZEL *psizelExtent) 
{ 
    sizelExtent = *psizelExtent; 
    pserv->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE, TXTBIT_EXTENTCHANGE);
}

void CTxtWinHost::LimitText(LONG nChars)
{
    cchTextMost = nChars;
    if( cchTextMost <= 0 ) cchTextMost = cInitTextMax;
    pserv->OnTxPropertyBitsChange(TXTBIT_MAXLENGTHCHANGE, TXTBIT_MAXLENGTHCHANGE);
}

BOOL CTxtWinHost::IsCaptured()
{
    return fCaptured;
}

BOOL CTxtWinHost::GetAllowBeep()
{
    return fAllowBeep;
}

void CTxtWinHost::SetAllowBeep(BOOL fAllowBeep)
{
    fAllowBeep = fAllowBeep;

    pserv->OnTxPropertyBitsChange(TXTBIT_ALLOWBEEP, 
        fAllowBeep ? TXTBIT_ALLOWBEEP : 0);
}

WORD CTxtWinHost::GetDefaultAlign()
{
    return pf.wAlignment;
}

void CTxtWinHost::SetDefaultAlign(WORD wNewAlign)
{
    pf.wAlignment = wNewAlign;

    // Notify control of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

BOOL CTxtWinHost::GetRichTextFlag()
{
    return fRich;
}

void CTxtWinHost::SetRichTextFlag(BOOL fNew)
{
    fRich = fNew;

    pserv->OnTxPropertyBitsChange(TXTBIT_RICHTEXT, 
        fNew ? TXTBIT_RICHTEXT : 0);
}

LONG CTxtWinHost::GetDefaultLeftIndent()
{
    return pf.dxOffset;
}

void CTxtWinHost::SetDefaultLeftIndent(LONG lNewIndent)
{
    pf.dxOffset = lNewIndent;

    pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

void CTxtWinHost::SetClientRect(RECT *prc) 
{
    rcClient = *prc;

    LONG xPerInch = ::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSX); 
    LONG yPerInch =	::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSY); 
    sizelExtent.cx = DXtoHimetricX(rcClient.right - rcClient.left, xPerInch);
    sizelExtent.cy = DYtoHimetricY(rcClient.bottom - rcClient.top, yPerInch);

    pserv->OnTxPropertyBitsChange(TXTBIT_VIEWINSETCHANGE, TXTBIT_VIEWINSETCHANGE);
}

BOOL CTxtWinHost::SetSaveSelection(BOOL f_SaveSelection)
{
    BOOL fResult = f_SaveSelection;

    fSaveSelection = f_SaveSelection;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_SAVESELECTION, 
        fSaveSelection ? TXTBIT_SAVESELECTION : 0);

    return fResult;		
}

HRESULT	CTxtWinHost::OnTxInPlaceDeactivate()
{
    HRESULT hr = pserv->OnTxInPlaceDeactivate();

    if (SUCCEEDED(hr))
    {
        fInplaceActive = FALSE;
    }

    return hr;
}

HRESULT	CTxtWinHost::OnTxInPlaceActivate(LPCRECT prcClient)
{
    fInplaceActive = TRUE;

    HRESULT hr = pserv->OnTxInPlaceActivate(prcClient);

    if (FAILED(hr))
    {
        fInplaceActive = FALSE;
    }

    return hr;
}

BOOL CTxtWinHost::DoSetCursor(RECT *prc, POINT *pt)
{
    RECT rc = prc ? *prc : rcClient;

    // Is this in our rectangle?
    if (PtInRect(&rc, *pt))
    {
        RECT *prcClient = (!fInplaceActive || prc) ? &rc : NULL;
        pserv->OnTxSetCursor(DVASPECT_CONTENT,	-1, NULL, NULL,  m_re->GetManager()->GetPaintDC(),
            NULL, prcClient, pt->x, pt->y);

        return TRUE;
    }

    return FALSE;
}

void CTxtWinHost::GetControlRect(LPRECT prc)
{
    prc->top = rcClient.top;
    prc->bottom = rcClient.bottom;
    prc->left = rcClient.left;
    prc->right = rcClient.right;
}

void CTxtWinHost::SetTransparent(BOOL f_Transparent)
{
    fTransparent = f_Transparent;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_BACKSTYLECHANGE, 0);
}

LONG CTxtWinHost::SetAccelPos(LONG l_accelpos)
{
    LONG laccelposOld = l_accelpos;

    laccelpos = l_accelpos;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_SHOWACCELERATOR, 0);

    return laccelposOld;
}

WCHAR CTxtWinHost::SetPasswordChar(WCHAR ch_PasswordChar)
{
    WCHAR chOldPasswordChar = chPasswordChar;

    chPasswordChar = ch_PasswordChar;

    // notify text services of property change
    pserv->OnTxPropertyBitsChange(TXTBIT_USEPASSWORD, 
        (chPasswordChar != 0) ? TXTBIT_USEPASSWORD : 0);

    return chOldPasswordChar;
}

void CTxtWinHost::SetDisabled(BOOL fOn)
{
    cf.dwMask	 |= CFM_COLOR | CFM_DISABLED;
    cf.dwEffects |= CFE_AUTOCOLOR | CFE_DISABLED;

    if( !fOn )
    {
        cf.dwEffects &= ~CFE_DISABLED;
    }

    pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
        TXTBIT_CHARFORMATCHANGE);
}

LONG CTxtWinHost::SetSelBarWidth(LONG l_SelBarWidth)
{
    LONG lOldSelBarWidth = lSelBarWidth;

    lSelBarWidth = l_SelBarWidth;

    if (lSelBarWidth)
    {
        dwStyle |= ES_SELECTIONBAR;
    }
    else
    {
        dwStyle &= (~ES_SELECTIONBAR);
    }

    pserv->OnTxPropertyBitsChange(TXTBIT_SELBARCHANGE, TXTBIT_SELBARCHANGE);

    return lOldSelBarWidth;
}

BOOL CTxtWinHost::GetTimerState()
{
    return fTimer;
}

void CTxtWinHost::SetCharFormat(CHARFORMAT2W &c)
{
    cf = c;
}

void CTxtWinHost::SetParaFormat(PARAFORMAT2 &p)
{
    pf = p;
}

#endif

/////////////////////////////////////////////////////////////////////////////////////
//
//

CTextToolUI::CTextToolUI() : m_pTwh(NULL), m_bVScrollBarFixing(false), m_bWantTab(true), m_bWantReturn(true), 
    m_bWantCtrlReturn(true), m_bRich(true), m_bReadOnly(false), m_bWordWrap(false), m_dwTextColor(0), m_iFont(-1), 
    m_iLimitText(cInitTextMax), m_lTwhStyle(ES_MULTILINE), m_bInited(false), m_chLeadByte(0),m_uButtonState(0),
	m_dwTipValueColor(0xFFBAC0C5)
{
#ifndef _UNICODE
	m_fAccumulateDBC =true;
#else
	m_fAccumulateDBC= false;
#endif
	::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
    change_notify_enable_ = true;
    show_border_ = false;
    offset_y_ = 0;
}

CTextToolUI::~CTextToolUI()
{
    if( m_pTwh ) {
        m_pTwh->Release();
        m_pManager->RemoveMessageFilter(this);
    }
    m_pManager->RemoveNotifier(this);
    type_ = TEXT;
    main_canvas_ = NULL;
}

LPCTSTR CTextToolUI::GetClass() const
{
    return _T("TextTool");
}

LPVOID CTextToolUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, L"TextTool") == 0 ) return static_cast<CTextToolUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CTextToolUI::GetControlFlags() const
{
    if( !IsEnabled() ) return CControlUI::GetControlFlags();

    return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
}

bool CTextToolUI::IsWantTab()
{
    return m_bWantTab;
}

void CTextToolUI::SetWantTab(bool bWantTab)
{
    m_bWantTab = bWantTab;
}

bool CTextToolUI::IsWantReturn()
{
    return m_bWantReturn;
}

void CTextToolUI::SetWantReturn(bool bWantReturn)
{
    m_bWantReturn = bWantReturn;
}

bool CTextToolUI::IsWantCtrlReturn()
{
    return m_bWantCtrlReturn;
}

void CTextToolUI::SetWantCtrlReturn(bool bWantCtrlReturn)
{
    m_bWantCtrlReturn = bWantCtrlReturn;
}

bool CTextToolUI::IsRich()
{
    return m_bRich;
}

void CTextToolUI::SetRich(bool bRich)
{
    m_bRich = bRich;
    if( m_pTwh ) m_pTwh->SetRichTextFlag(bRich);
}

bool CTextToolUI::IsReadOnly()
{
    return m_bReadOnly;
}

void CTextToolUI::SetReadOnly(bool bReadOnly)
{
    m_bReadOnly = bReadOnly;
    if( m_pTwh ) m_pTwh->SetReadOnly(bReadOnly);
}

bool CTextToolUI::GetWordWrap()
{
    return m_bWordWrap;
}

void CTextToolUI::SetWordWrap(bool bWordWrap)
{
    m_bWordWrap = bWordWrap;
    if( m_pTwh ) m_pTwh->SetWordWrap(bWordWrap);
}

int CTextToolUI::GetFont()
{
    return m_iFont;
}

void CTextToolUI::SetFont(int index)
{
    m_iFont = index;
    if( m_pTwh ) {
        m_pTwh->SetFont(GetManager()->GetFont(m_iFont));
    }
}

void CTextToolUI::SetFontSize(int size)
{
    LOGFONT lf = { 0 };
    ::GetObject(GetManager()->GetFont(m_iFont), sizeof(LOGFONT), &lf);
    lf.lfHeight = -size;
    HFONT hFont = ::CreateFontIndirect(&lf);
    if (hFont == NULL) return;
    m_pTwh->SetFont(hFont);
    ::DeleteObject(hFont);

    /*LOGFONT lf = { 0 };
    ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
    _tcsncpy(lf.lfFaceName, L"微软雅黑", LF_FACESIZE);
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfHeight = -size;
    lf.lfQuality = CLEARTYPE_QUALITY;
    HFONT hFont = ::CreateFontIndirect(&lf);
    if (hFont == NULL) return;
    m_pTwh->SetFont(hFont);
    ::DeleteObject(hFont);*/


}

void CTextToolUI::SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
{
    if( m_pTwh ) {
        LOGFONT lf = { 0 };
        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        _tcsncpy(lf.lfFaceName, pStrFontName, LF_FACESIZE);
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfHeight = -nSize;
        if( bBold ) lf.lfWeight += FW_BOLD;
        if( bUnderline ) lf.lfUnderline = TRUE;
        if( bItalic ) lf.lfItalic = TRUE;
        HFONT hFont = ::CreateFontIndirect(&lf);
        if( hFont == NULL ) return;
        m_pTwh->SetFont(hFont);
        ::DeleteObject(hFont);
    }
}

void CTextToolUI::SetEnabled(bool bEnabled)
{
	if (m_bEnabled == bEnabled) return;

	if( m_pTwh ) {
		m_pTwh->SetColor(bEnabled ? m_dwTextColor : m_pManager->GetDefaultDisabledColor());
	}
	
	CContainerUI::SetEnabled(bEnabled);
}

LONG CTextToolUI::GetWinStyle()
{
    return m_lTwhStyle;
}

void CTextToolUI::SetWinStyle(LONG lStyle)
{
    m_lTwhStyle = lStyle;
}

DWORD CTextToolUI::GetTextColor()
{
    return m_dwTextColor;
}

void CTextToolUI::SetTextColor(DWORD dwTextColor)
{
    m_dwTextColor = dwTextColor;
    if( m_pTwh ) {
        m_pTwh->SetColor(dwTextColor);
    }
}

int CTextToolUI::GetLimitText()
{
    return m_iLimitText;
}

void CTextToolUI::SetLimitText(int iChars)
{
    m_iLimitText = iChars;
    if( m_pTwh ) {
        m_pTwh->LimitText(m_iLimitText);
    }
}

long CTextToolUI::GetTextLength(DWORD dwFlags) const
{
    GETTEXTLENGTHEX textLenEx;
    textLenEx.flags = dwFlags;
#ifdef _UNICODE
    textLenEx.codepage = 1200;
#else
    textLenEx.codepage = CP_ACP;
#endif
    LRESULT lResult;
    TxSendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&textLenEx, 0, &lResult);
    return (long)lResult;
}

CDuiString CTextToolUI::GetText() const
{
    long lLen = GetTextLength(GTL_DEFAULT);
    LPTSTR lpText = NULL;
    GETTEXTEX gt;
    gt.flags = GT_DEFAULT;
#ifdef _UNICODE
    gt.cb = sizeof(TCHAR) * (lLen + 1) ;
    gt.codepage = 1200;
    lpText = new TCHAR[lLen + 1];
    ::ZeroMemory(lpText, (lLen + 1) * sizeof(TCHAR));
#else
    gt.cb = sizeof(TCHAR) * lLen * 2 + 1;
    gt.codepage = CP_ACP;
    lpText = new TCHAR[lLen * 2 + 1];
    ::ZeroMemory(lpText, (lLen * 2 + 1) * sizeof(TCHAR));
#endif
    gt.lpDefaultChar = NULL;
    gt.lpUsedDefChar = NULL;
    TxSendMessage(EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)lpText, 0);
    CDuiString sText(lpText);
    delete[] lpText;
    return sText;
}

void CTextToolUI::SetText(LPCTSTR pstrText)
{
    m_sText = pstrText;
    if( !m_pTwh ) return;
	m_pTwh->SetColor(m_dwTextColor);
    SetSel(0, -1);
    ReplaceSel(pstrText, FALSE);
}

bool CTextToolUI::GetModify() const
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_GETMODIFY, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

void CTextToolUI::SetModify(bool bModified) const
{ 
    TxSendMessage(EM_SETMODIFY, bModified, 0, 0);
}

void CTextToolUI::GetSel(CHARRANGE &cr) const
{ 
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0); 
}

void CTextToolUI::GetSel(long& nStartChar, long& nEndChar) const
{
    CHARRANGE cr;
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0); 
    nStartChar = cr.cpMin;
    nEndChar = cr.cpMax;
}

int CTextToolUI::SetSel(CHARRANGE &cr)
{ 
    LRESULT lResult;
    TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult); 
    return (int)lResult;
}

int CTextToolUI::SetSel(long nStartChar, long nEndChar)
{
    CHARRANGE cr;
    cr.cpMin = nStartChar;
    cr.cpMax = nEndChar;
    LRESULT lResult;
    TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult); 
    return (int)lResult;
}

void CTextToolUI::ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo)
{
#ifdef _UNICODE		
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText, 0); 
#else
    int iLen = _tcslen(lpszNewText);
    LPWSTR lpText = new WCHAR[iLen + 1];
    ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
    ::MultiByteToWideChar(CP_ACP, 0, lpszNewText, -1, (LPWSTR)lpText, iLen) ;
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpText, 0); 
    delete[] lpText;
#endif
}

void CTextToolUI::ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo)
{
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText, 0); 
}

CDuiString CTextToolUI::GetSelText() const
{
    if( !m_pTwh ) return CDuiString();
    CHARRANGE cr;
    cr.cpMin = cr.cpMax = 0;
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0);
    LPWSTR lpText = NULL;
    lpText = new WCHAR[cr.cpMax - cr.cpMin + 1];
    ::ZeroMemory(lpText, (cr.cpMax - cr.cpMin + 1) * sizeof(WCHAR));
    TxSendMessage(EM_GETSELTEXT, 0, (LPARAM)lpText, 0);
    CDuiString sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

int CTextToolUI::SetSelAll()
{
    return SetSel(0, -1);
}

int CTextToolUI::SetSelNone()
{
    return SetSel(-1, 0);
}

bool CTextToolUI::GetZoom(int& nNum, int& nDen) const
{
    LRESULT lResult;
    TxSendMessage(EM_GETZOOM, (WPARAM)&nNum, (LPARAM)&nDen, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CTextToolUI::SetZoom(int nNum, int nDen)
{
    if (nNum < 0 || nNum > 64) return false;
    if (nDen < 0 || nDen > 64) return false;
    LRESULT lResult;
    TxSendMessage(EM_SETZOOM, nNum, nDen, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CTextToolUI::SetZoomOff()
{
    LRESULT lResult;
    TxSendMessage(EM_SETZOOM, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

WORD CTextToolUI::GetSelectionType() const
{
    LRESULT lResult;
    TxSendMessage(EM_SELECTIONTYPE, 0, 0, &lResult);
    return (WORD)lResult;
}

bool CTextToolUI::GetAutoURLDetect() const
{
    LRESULT lResult;
    TxSendMessage(EM_GETAUTOURLDETECT, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CTextToolUI::SetAutoURLDetect(bool bAutoDetect)
{
    LRESULT lResult;
    TxSendMessage(EM_AUTOURLDETECT, bAutoDetect, 0, &lResult);
    return (BOOL)lResult == FALSE;
}

DWORD CTextToolUI::GetEventMask() const
{
    LRESULT lResult;
    TxSendMessage(EM_GETEVENTMASK, 0, 0, &lResult);
    return (DWORD)lResult;
}

DWORD CTextToolUI::SetEventMask(DWORD dwEventMask)
{
    LRESULT lResult;
    TxSendMessage(EM_SETEVENTMASK, 0, dwEventMask, &lResult);
    return (DWORD)lResult;
}

CDuiString CTextToolUI::GetTextRange(long nStartChar, long nEndChar) const
{
    TEXTRANGEW tr = { 0 };
    tr.chrg.cpMin = nStartChar;
    tr.chrg.cpMax = nEndChar;
    LPWSTR lpText = NULL;
    lpText = new WCHAR[nEndChar - nStartChar + 1];
    ::ZeroMemory(lpText, (nEndChar - nStartChar + 1) * sizeof(WCHAR));
    tr.lpstrText = lpText;
    TxSendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr, 0);
    CDuiString sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

void CTextToolUI::HideSelection(bool bHide, bool bChangeStyle)
{
    TxSendMessage(EM_HIDESELECTION, bHide, bChangeStyle, 0);
}

void CTextToolUI::ScrollCaret()
{
    TxSendMessage(EM_SCROLLCARET, 0, 0, 0);
}

int CTextToolUI::InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo)
{
    int nRet = SetSel(nInsertAfterChar, nInsertAfterChar);
    ReplaceSel(lpstrText, bCanUndo);
    return nRet;
}

int CTextToolUI::AppendText(LPCTSTR lpstrText, bool bCanUndo)
{
    int nRet = SetSel(-1, -1);
    ReplaceSel(lpstrText, bCanUndo);
    return nRet;
}

DWORD CTextToolUI::GetDefaultCharFormat(CHARFORMAT2 &cf) const
{
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
    return (DWORD)lResult;
}

bool CTextToolUI::SetDefaultCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
    if( (BOOL)lResult == TRUE ) {
        CHARFORMAT2W cfw;
        cfw.cbSize = sizeof(CHARFORMAT2W);
        TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cfw, 0);
        m_pTwh->SetCharFormat(cfw);
        return true;
    }
    return false;
}

DWORD CTextToolUI::GetSelectionCharFormat(CHARFORMAT2 &cf) const
{
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cf, &lResult);
    return (DWORD)lResult;
}

bool CTextToolUI::SetSelectionCharFormat(CHARFORMAT2 &cf)
{
	if(m_pManager->IsBackgroundTransparent())
	{
		CRenderEngine::CheckAlphaColor(cf.crTextColor);
		CRenderEngine::CheckAlphaColor(cf.crBackColor);
	}
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CTextToolUI::SetWordCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION|SCF_WORD, (LPARAM)&cf, &lResult); 
    return (BOOL)lResult == TRUE;
}

DWORD CTextToolUI::GetParaFormat(PARAFORMAT2 &pf) const
{
    pf.cbSize = sizeof(PARAFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
    return (DWORD)lResult;
}

bool CTextToolUI::SetParaFormat(PARAFORMAT2 &pf)
{
    if( !m_pTwh ) return false;
    pf.cbSize = sizeof(PARAFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
    if( (BOOL)lResult == TRUE ) {
        m_pTwh->SetParaFormat(pf);
        return true;
    }
    return false;
}

bool CTextToolUI::Redo()
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_REDO, 0, 0, &lResult);
    return (BOOL)lResult == TRUE; 
}

bool CTextToolUI::Undo()
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_UNDO, 0, 0, &lResult);
    return (BOOL)lResult == TRUE; 
}

void CTextToolUI::Clear()
{ 
    TxSendMessage(WM_CLEAR, 0, 0, 0); 
}

void CTextToolUI::Copy()
{ 
    TxSendMessage(WM_COPY, 0, 0, 0); 
}

void CTextToolUI::Cut()
{ 
    TxSendMessage(WM_CUT, 0, 0, 0); 
}

void CTextToolUI::Paste()
{ 
    TxSendMessage(WM_PASTE, 0, 0, 0); 
}

int CTextToolUI::GetLineCount() const
{ 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_GETLINECOUNT, 0, 0, &lResult);
    return (int)lResult; 
}

CDuiString CTextToolUI::GetLine(int nIndex, int nMaxLength) const
{
    LPWSTR lpText = NULL;
    lpText = new WCHAR[nMaxLength + 1];
    ::ZeroMemory(lpText, (nMaxLength + 1) * sizeof(WCHAR));
    *(LPWORD)lpText = (WORD)nMaxLength;
    TxSendMessage(EM_GETLINE, nIndex, (LPARAM)lpText, 0);
    CDuiString sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

int CTextToolUI::LineIndex(int nLine) const
{
    LRESULT lResult;
    TxSendMessage(EM_LINEINDEX, nLine, 0, &lResult);
    return (int)lResult;
}

int CTextToolUI::LineLength(int nLine) const
{
    LRESULT lResult;
    TxSendMessage(EM_LINELENGTH, nLine, 0, &lResult);
    return (int)lResult;
}

bool CTextToolUI::LineScroll(int nLines, int nChars)
{
    LRESULT lResult;
    TxSendMessage(EM_LINESCROLL, nChars, nLines, &lResult);
    return (BOOL)lResult == TRUE;
}

CPoint CTextToolUI::GetCharPos(long lChar) const
{ 
    CPoint pt; 
    TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, (LPARAM)lChar, 0); 
    return pt;
}

long CTextToolUI::LineFromChar(long nIndex) const
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_EXLINEFROMCHAR, 0, nIndex, &lResult);
    return (long)lResult;
}

CPoint CTextToolUI::PosFromChar(UINT nChar) const
{ 
    POINTL pt; 
    TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, nChar, 0); 
    return CPoint(pt.x, pt.y); 
}

int CTextToolUI::CharFromPos(CPoint pt) const
{ 
    POINTL ptl = {pt.x, pt.y}; 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_CHARFROMPOS, 0, (LPARAM)&ptl, &lResult);
    return (int)lResult; 
}

void CTextToolUI::EmptyUndoBuffer()
{ 
    TxSendMessage(EM_EMPTYUNDOBUFFER, 0, 0, 0); 
}

UINT CTextToolUI::SetUndoLimit(UINT nLimit)
{ 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_SETUNDOLIMIT, (WPARAM) nLimit, 0, &lResult);
    return (UINT)lResult; 
}

long CTextToolUI::StreamIn(int nFormat, EDITSTREAM &es)
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_STREAMIN, nFormat, (LPARAM)&es, &lResult);
    return (long)lResult;
}

long CTextToolUI::StreamOut(int nFormat, EDITSTREAM &es)
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_STREAMOUT, nFormat, (LPARAM)&es, &lResult);
    return (long)lResult; 
}

void CTextToolUI::DoInit()
{
	if(m_bInited)
		return ;

    CREATESTRUCT cs;
    cs.style = m_lTwhStyle;
    cs.x = 0;
    cs.y = 0;
    cs.cy = 0;
    cs.cx = 0;
    cs.lpszName = m_sText.GetData();
    CreateHost(this, &cs, &m_pTwh);
    if( m_pTwh ) {
        m_pTwh->SetTransparent(TRUE);
        LRESULT lResult;
        m_pTwh->GetTextServices()->TxSendMessage(EM_SETLANGOPTIONS, 0, 0, &lResult);
        m_pTwh->OnTxInPlaceActivate(NULL);
        m_pManager->AddMessageFilter(this);
        m_pManager->AddNotifier(this);

        DWORD lMask = GetEventMask();
        lMask |= ENM_CHANGE;
        lMask &= ~ENM_PROTECTED;
        SetEventMask(lMask);

		if (!m_bEnabled)
		{
			m_pTwh->SetColor(m_pManager->GetDefaultDisabledColor());
		}
		else if (GetText() == _T("") && !m_sTipValue.IsEmpty())
		{
			SetText(m_sTipValue);
			m_pTwh->SetColor(m_dwTipValueColor);
		}

        SetFontSize(pen_size_*scale_);

    }

	m_bInited= true;
}

HRESULT CTextToolUI::TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const
{
    if( m_pTwh ) {
        if( msg == WM_KEYDOWN && TCHAR(wparam) == VK_RETURN ) {
            if( !m_bWantReturn || (::GetKeyState(VK_CONTROL) < 0 && !m_bWantCtrlReturn) ) {
                if( m_pManager != NULL ) m_pManager->SendNotify((CControlUI*)this, DUI_MSGTYPE_RETURN);
                return S_OK;
            }
        }
        return m_pTwh->GetTextServices()->TxSendMessage(msg, wparam, lparam, plresult);
    }
    return S_FALSE;
}

IDropTarget* CTextToolUI::GetTxDropTarget()
{
    IDropTarget *pdt = NULL;
    if( m_pTwh->GetTextServices()->TxGetDropTarget(&pdt) == NOERROR ) return pdt;
    return NULL;
}

bool CTextToolUI::OnTxViewChanged()
{
    return true;
}

bool CTextToolUI::SetDropAcceptFile(bool bAccept) 
{
	LRESULT lResult;
	TxSendMessage(EM_SETEVENTMASK, 0,ENM_DROPFILES|ENM_LINK, // ENM_CHANGE| ENM_CORRECTTEXT | ENM_DRAGDROPDONE | ENM_DROPFILES | ENM_IMECHANGE | ENM_LINK | ENM_OBJECTPOSITIONS | ENM_PROTECTED | ENM_REQUESTRESIZE | ENM_SCROLL | ENM_SELCHANGE | ENM_UPDATE,
		&lResult);
	return (BOOL)lResult == FALSE;
}

void CTextToolUI::OnTxNotify(DWORD iNotify, void *pv)
{
	switch(iNotify)
	{ 
	case EN_CHANGE:
		{
			GetManager()->SendNotify(this, DUI_MSGTYPE_TEXTCHANGED);
		}
		break;
	case EN_DROPFILES:   
	case EN_MSGFILTER:   
	case EN_OLEOPFAILED:   
	case EN_PROTECTED:   
	case EN_SAVECLIPBOARD:   
	case EN_SELCHANGE:   
	case EN_STOPNOUNDO:   
	case EN_LINK:   
	case EN_OBJECTPOSITIONS:   
	case EN_DRAGDROPDONE:   
		{
			if(pv)                        // Fill out NMHDR portion of pv   
			{   
				LONG nId =  GetWindowLong(this->GetManager()->GetPaintWindow(), GWL_ID);   
				NMHDR  *phdr = (NMHDR *)pv;   
				phdr->hwndFrom = this->GetManager()->GetPaintWindow();   
				phdr->idFrom = nId;   
				phdr->code = iNotify;  

				if(SendMessage(this->GetManager()->GetPaintWindow(), WM_NOTIFY, (WPARAM) nId, (LPARAM) pv))   
				{   
					//hr = S_FALSE;   
				}   
			}    
		}
		break;
	}
}

// 多行非rich格式的richedit有一个滚动条bug，在最后一行是空行时，LineDown和SetScrollPos无法滚动到最后
// 引入iPos就是为了修正这个bug
void CTextToolUI::SetScrollPos(SIZE szPos)
{
    int cx = 0;
    int cy = 0;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
        m_pVerticalScrollBar->SetScrollPos(szPos.cy);
        cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
    }
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
        m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
        cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
    }
    if( cy != 0 ) {
        int iPos = 0;
        if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
            iPos = m_pVerticalScrollBar->GetScrollPos();
        WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, m_pVerticalScrollBar->GetScrollPos());
        TxSendMessage(WM_VSCROLL, wParam, 0L, 0);
        if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
            if( cy > 0 && m_pVerticalScrollBar->GetScrollPos() <= iPos )
                m_pVerticalScrollBar->SetScrollPos(iPos);
        }
    }
    if( cx != 0 ) {
        WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, m_pHorizontalScrollBar->GetScrollPos());
        TxSendMessage(WM_HSCROLL, wParam, 0L, 0);
    }
}

void CTextToolUI::LineUp()
{
    TxSendMessage(WM_VSCROLL, SB_LINEUP, 0L, 0);
}

void CTextToolUI::LineDown()
{
    int iPos = 0;
    if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
        iPos = m_pVerticalScrollBar->GetScrollPos();
    TxSendMessage(WM_VSCROLL, SB_LINEDOWN, 0L, 0);
    if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        if( m_pVerticalScrollBar->GetScrollPos() <= iPos )
            m_pVerticalScrollBar->SetScrollPos(m_pVerticalScrollBar->GetScrollRange());
    }
}

void CTextToolUI::PageUp()
{
    TxSendMessage(WM_VSCROLL, SB_PAGEUP, 0L, 0);
}

void CTextToolUI::PageDown()
{
    TxSendMessage(WM_VSCROLL, SB_PAGEDOWN, 0L, 0);
}

void CTextToolUI::HomeUp()
{
    TxSendMessage(WM_VSCROLL, SB_TOP, 0L, 0);
}

void CTextToolUI::EndDown()
{
    TxSendMessage(WM_VSCROLL, SB_BOTTOM, 0L, 0);
}

void CTextToolUI::LineLeft()
{
    TxSendMessage(WM_HSCROLL, SB_LINELEFT, 0L, 0);
}

void CTextToolUI::LineRight()
{
    TxSendMessage(WM_HSCROLL, SB_LINERIGHT, 0L, 0);
}

void CTextToolUI::PageLeft()
{
    TxSendMessage(WM_HSCROLL, SB_PAGELEFT, 0L, 0);
}

void CTextToolUI::PageRight()
{
    TxSendMessage(WM_HSCROLL, SB_PAGERIGHT, 0L, 0);
}

void CTextToolUI::HomeLeft()
{
    TxSendMessage(WM_HSCROLL, SB_LEFT, 0L, 0);
}

void CTextToolUI::EndRight()
{
    TxSendMessage(WM_HSCROLL, SB_RIGHT, 0L, 0);
}

void CTextToolUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->DoEvent(event);
        else CControlUI::DoEvent(event);
        return;
    }

    else if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
    {
        if( m_pTwh && m_pTwh->DoSetCursor(NULL, &event.ptMouse) ) {
            return;
        }
    }
#if 1
	if( event.Type == UIEVENT_SETFOCUS ) {
		if( m_pTwh ) {
			m_pTwh->OnTxInPlaceActivate(NULL);
			if (GetText() == m_sTipValue)
			{
				SetText(_T(""));
				m_pTwh->SetColor(m_dwTextColor);
			}
			m_pTwh->GetTextServices()->TxSendMessage(WM_SETFOCUS, 0, 0, 0);
		}
		m_bFocused = true;
		Invalidate();
		return;
	}
#endif
	if( event.Type == UIEVENT_KILLFOCUS )  {
		if( m_pTwh ) {
			m_pTwh->OnTxInPlaceActivate(NULL);
			if (GetText() == _T("") && !m_sTipValue.IsEmpty())
			{
				SetText(m_sTipValue);
				m_pTwh->SetColor(m_dwTipValueColor);
			}
			m_pTwh->GetTextServices()->TxSendMessage(WM_KILLFOCUS, 0, 0, 0);
		}
		m_bFocused = false;
		Invalidate();
		return;
	}

    else if( event.Type == UIEVENT_TIMER ) {
        if( m_pTwh ) {
            m_pTwh->GetTextServices()->TxSendMessage(WM_TIMER, event.wParam, event.lParam, 0);
        } 
        if (event.wParam == kCaretTimerID)
        {
            ::InvalidateRect(GetManager()->GetPaintWindow(), &m_rtCaret, FALSE);
            m_bCaretActive = !m_bCaretActive;
        }
    }

    else if( event.Type == UIEVENT_SCROLLWHEEL ) {
        if( (event.wKeyState & MK_CONTROL) != 0  ) {
            return;
        }
    }
    else if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK ) 
    {
        return;
    }
    else if( event.Type == UIEVENT_MOUSEMOVE ) 
    {
        return;
    }
    else if( event.Type == UIEVENT_BUTTONUP ) 
    {
        return;
    }
	else if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	else if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( IsEnabled() ) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		return;
	}
    if( event.Type > UIEVENT__KEYBEGIN && event.Type < UIEVENT__KEYEND )
    {
        return;
    }
    CContainerUI::DoEvent(event);
}


LPCTSTR CTextToolUI::GetNormalImage()
{
	return m_sNormalImage;
}

void CTextToolUI::SetNormalImage(LPCTSTR pStrImage)
{
	m_sNormalImage = pStrImage;
	Invalidate();
}

LPCTSTR CTextToolUI::GetHotImage()
{
	return m_sHotImage;
}

void CTextToolUI::SetHotImage(LPCTSTR pStrImage)
{
	m_sHotImage = pStrImage;
	Invalidate();
}

LPCTSTR CTextToolUI::GetFocusedImage()
{
	return m_sFocusedImage;
}

void CTextToolUI::SetFocusedImage(LPCTSTR pStrImage)
{
	m_sFocusedImage = pStrImage;
	Invalidate();
}

LPCTSTR CTextToolUI::GetDisabledImage()
{
	return m_sDisabledImage;
}

void CTextToolUI::SetDisabledImage(LPCTSTR pStrImage)
{
	m_sDisabledImage = pStrImage;
	Invalidate();
}

RECT CTextToolUI::GetTextPadding() const
{
	return m_rcTextPadding;
}

void CTextToolUI::SetTextPadding(RECT rc)
{
	m_rcTextPadding = rc;
	Invalidate();
}

void CTextToolUI::SetTipValue( LPCTSTR pStrTipValue )
{
	m_sTipValue	= pStrTipValue;
}

LPCTSTR CTextToolUI::GetTipValue()
{
	return m_sTipValue.GetData();
}

void CTextToolUI::SetTipValueColor( LPCTSTR pStrColor )
{
	if( *pStrColor == _T('#')) pStrColor = ::CharNext(pStrColor);
	LPTSTR pstr = NULL;
	DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

	m_dwTipValueColor = clrColor;
}

DWORD CTextToolUI::GetTipValueColor()
{
	return m_dwTipValueColor;
}

void CTextToolUI::PaintStatusImage(HDC hDC)
{
	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;
	if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~ UISTATE_DISABLED;

	if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
		if( !m_sDisabledImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
		if( !m_sFocusedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_HOT ) != 0 ) {
		if( !m_sHotImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
			else return;
		}
	}

	if( !m_sNormalImage.IsEmpty() ) {
		if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
		else return;
	}
}

SIZE CTextToolUI::EstimateSize(SIZE szAvailable)
{
    //return CSize(m_rcItem); // 这种方式在第一次设置大小之后就大小不变了
    return CContainerUI::EstimateSize(szAvailable);
}

void CTextToolUI::SetPos(RECT rc)
{
    CControlUI::SetPos(rc);
    rc = m_rcItem;

    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;
    bool bVScrollBarVisiable = false;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        bVScrollBarVisiable = true;
        rc.right -= m_pVerticalScrollBar->GetFixedWidth();
    }
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
    }

	if( m_pTwh ) {
		RECT rcRich = rc;
		rcRich.left += m_rcTextPadding.left;
		rcRich.right -= m_rcTextPadding.right;
		rcRich.top += m_rcTextPadding.top;
		rcRich.bottom -= m_rcTextPadding.bottom;
		m_pTwh->SetClientRect(&rcRich);
		if( bVScrollBarVisiable && (!m_pVerticalScrollBar->IsVisible() || m_bVScrollBarFixing) ) {
			LONG lWidth = rcRich.right - rcRich.left + m_pVerticalScrollBar->GetFixedWidth();
			LONG lHeight = 0;
			SIZEL szExtent = { -1, -1 };
			m_pTwh->GetTextServices()->TxGetNaturalSize(
				DVASPECT_CONTENT, 
				GetManager()->GetPaintDC(), 
				NULL,
				NULL,
				TXTNS_FITTOCONTENT,
				&szExtent,
				&lWidth,
				&lHeight);
			if( lHeight > rcRich.bottom - rcRich.top ) {
				m_pVerticalScrollBar->SetVisible(true);
				m_pVerticalScrollBar->SetScrollPos(0);
				m_bVScrollBarFixing = true;
			}
			else {
				if( m_bVScrollBarFixing ) {
					m_pVerticalScrollBar->SetVisible(false);
					m_bVScrollBarFixing = false;
				}
			}
		}
	}

    if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
        RECT rcScrollBarPos = { rc.right, rc.top, rc.right + m_pVerticalScrollBar->GetFixedWidth(), rc.bottom};
        m_pVerticalScrollBar->SetPos(rcScrollBarPos);
    }
    if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
        RECT rcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight()};
        m_pHorizontalScrollBar->SetPos(rcScrollBarPos);
    }

	SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
		szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();

	int nAdjustables = 0;
	int cxFixed = 0;
	int nEstimateNum = 0;
	for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) continue;
		SIZE sz = pControl->EstimateSize(szAvailable);
		if( sz.cx == 0 ) {
			nAdjustables++;
		}
		else {
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
		}
		cxFixed += sz.cx +  pControl->GetPadding().left + pControl->GetPadding().right;
		nEstimateNum++;
	}
	cxFixed += (nEstimateNum - 1) * m_iChildPadding;

	int cxExpand = 0;
    int cxNeeded = 0;
	if( nAdjustables > 0 ) cxExpand = MAX(0, (szAvailable.cx - cxFixed) / nAdjustables);
	// Position the elements
	SIZE szRemaining = szAvailable;
	int iPosX = rc.left;
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
		iPosX -= m_pHorizontalScrollBar->GetScrollPos();
	}
	int iAdjustable = 0;
	int cxFixedRemaining = cxFixed;
	for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) {
			SetFloatPos(it2);
			continue;
		}
		RECT rcPadding = pControl->GetPadding();
		szRemaining.cx -= rcPadding.left;
		SIZE sz = pControl->EstimateSize(szRemaining);
		if( sz.cx == 0 ) {
			iAdjustable++;
			sz.cx = cxExpand;

			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
		}
		else {
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

		}

		sz.cy = pControl->GetFixedHeight();
		if( sz.cy == 0 ) sz.cy = rc.bottom - rc.top - rcPadding.top - rcPadding.bottom;
		if( sz.cy < 0 ) sz.cy = 0;
		if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
		if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();

		RECT rcCtrl = { iPosX + rcPadding.left, rc.top + rcPadding.top, iPosX + sz.cx + rcPadding.left , rc.top + rcPadding.top + sz.cy};
		pControl->SetPos(rcCtrl);
		iPosX += sz.cx + m_iChildPadding + rcPadding.left + rcPadding.right;
        cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
		szRemaining.cx -= sz.cx + m_iChildPadding + rcPadding.right;
	}
    cxNeeded += (nEstimateNum - 1) * m_iChildPadding;
	//reddrain
	if( m_pHorizontalScrollBar != NULL ) {
		if( cxNeeded > rc.right - rc.left ) {
			if( m_pHorizontalScrollBar->IsVisible() ) {
				m_pHorizontalScrollBar->SetScrollRange(cxNeeded - (rc.right - rc.left));
			}
			else {
				m_pHorizontalScrollBar->SetVisible(true);
				m_pHorizontalScrollBar->SetScrollRange(cxNeeded - (rc.right - rc.left));
				m_pHorizontalScrollBar->SetScrollPos(0);
				rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
			}
		}
		else {
			if( m_pHorizontalScrollBar->IsVisible() ) {
				m_pHorizontalScrollBar->SetVisible(false);
				m_pHorizontalScrollBar->SetScrollRange(0);
				m_pHorizontalScrollBar->SetScrollPos(0);
				rc.bottom += m_pHorizontalScrollBar->GetFixedHeight();
			}
		}
	}
	//redrain

}

void CTextToolUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    RECT rcTemp = { 0 };
    if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return;

    CRenderClip clip;
    CRenderClip::GenerateClip(hDC, rcTemp, clip);
    CControlUI::DoPaint(hDC, rcPaint);

    if( m_pTwh ) {

#if 1
        int width = rcPaint.right - rcPaint.left;
        int height = rcPaint.bottom - rcPaint.top;
        BITMAPINFOHEADER          bmih;
        ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));
        bmih.biSize = sizeof(BITMAPINFOHEADER);
        bmih.biWidth = width;
        bmih.biHeight = -height;
        bmih.biPlanes = 1;
        bmih.biBitCount = 32;//这里一定要是   
        bmih.biCompression = BI_RGB;
        bmih.biSizeImage = 0;
        bmih.biXPelsPerMeter = 0;
        bmih.biYPelsPerMeter = 0;
        bmih.biClrUsed = 0;
        bmih.biClrImportant = 0;


        HDC hdcMemory1 = CreateCompatibleDC(hDC);
        BYTE *dst = NULL;
        HBITMAP hBitMap1 = CreateDIBSection(hDC, (BITMAPINFO*)&bmih, DIB_RGB_COLORS, (VOID**)&dst, NULL, 0);
        memset(dst, 0, height * width * 4);
        SelectObject(hdcMemory1, hBitMap1);


        RECT rc;
        m_pTwh->GetControlRect(&rc);
        // Remember wparam is actually the hdc and lparam is the update
        // rect because this message has been preprocessed by the window.
        m_pTwh->GetTextServices()->TxDraw(
            DVASPECT_CONTENT,  		// Draw Aspect
            /*-1*/0,				// Lindex
            NULL,					// Info for drawing optimazation
            NULL,					// target device information
            hdcMemory1,			        // Draw device HDC
            NULL, 				   	// Target device HDC
            (RECTL*)&rc,			// Bounding client rectangle
            NULL, 		            // Clipping rectangle for metafiles
            (RECT*)&rcPaint,		// Update rectangle
            NULL, 	   				// Call back function
            NULL,					// Call back parameter
            0);				        // What view of the object

        //CRenderEngine::RestoreAlphaColor(pBgRgb, width, (RECT*)&rcPaint);

        
        {
            HDC hdcMemory2 = CreateCompatibleDC(hDC);
            BYTE *dstW = NULL;
            HBITMAP hBitMap2 = CreateDIBSection(hDC, (BITMAPINFO*)&bmih, DIB_RGB_COLORS, (VOID**)&dstW, NULL, 0);
            memset(dstW, 254, height * width * 4);
            SelectObject(hdcMemory2, hBitMap2);
            
            m_pTwh->GetTextServices()->TxDraw(
                DVASPECT_CONTENT,  		// Draw Aspect
                /*-1*/0,				// Lindex
                NULL,					// Info for drawing optimazation
                NULL,					// target device information
                hdcMemory2,			        // Draw device HDC
                NULL, 				   	// Target device HDC
                (RECTL*)&rc,			// Bounding client rectangle
                NULL, 		            // Clipping rectangle for metafiles
                (RECT*)&rcPaint,		// Update rectangle
                NULL, 	   				// Call back function
                NULL,					// Call back parameter
                0);				        // What view of the object

            BYTE r, g, b, a, rw, gw, bw, aw, alpha_r, alpha_g, alpha_b, alpha;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    //the idea is that we draw the same data onto black and white DC's
                    //and then calculate per pixel alpha based on difference, produced by alpha blending
                    r = *dst++;
                    g = *dst++;
                    b = *dst++;
                    a = *dst++;
                    rw = *dstW++;
                    gw = *dstW++;
                    bw = *dstW++;
                    aw = *dstW++;
                    alpha_r = rw - r;
                    alpha_g = gw - g;
                    alpha_b = bw - b;
                    //division by 3 is for accuracy and can be replaced by
                    //alpha = alpha_g; for example
                    alpha = (alpha_r + alpha_g + alpha_b) / 3;
                    *(dst - 1) = 255 - alpha;
                    //this algorithm should be optimized for MMX to achieve best performance
                }
            }
            DeleteObject(hBitMap2);
            DeleteDC(hdcMemory2);
        }

        BLENDFUNCTION oBlend;
        oBlend.BlendOp = 0; //theonlyBlendOpdefinedinWindows2000
        oBlend.BlendFlags = 0; //nothingelseisspecial...
        oBlend.SourceConstantAlpha = 255;//0~255//AC_SRC_ALPHA
        oBlend.AlphaFormat = 1; //...
        POINT ptSrc = { 0, 0 };
        POINT ptWinPos = { rcPaint.left, rcPaint.top };
        SIZE sizeWindow = { width, height };
        //::UpdateLayeredWindow(GetManager()->GetPaintWindow(), hDC, &ptWinPos, &sizeWindow, hdcMemory1, &ptSrc, RGB(0, 0, 0), &oBlend, ULW_ALPHA);
        AlphaBlend(hDC, 0, 0, width, height, hdcMemory1, 0, 0, width, height, oBlend);
        DeleteObject(hBitMap1);
        DeleteDC(hdcMemory1);


        //::SetBkColor(hCloneDc, RGB(255, 255, 255));
        //BitBlt(hDC, 0, 0, width, height, hCloneDc, 0, 0, SRCAND);
        

        //::TransparentBlt(hCloneDc, 0, 0, width, height, hCloneDc, 0, 0, width, height, RGB(0xff, 0xff, 0xff));

        BLENDFUNCTION bf;
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.AlphaFormat = AC_SRC_ALPHA;
        bf.SourceConstantAlpha = 255;
        //AlphaBlend(hDC, 0, 0, width, height, hCloneDc, 0, 0, width, height, bf);
        /*POINT pt = { rcPaint.left, rcPaint.top };
        POINT ptSrc = { 0, 0 };
        SIZE szWindow = { rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top };
        BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
        ::UpdateLayeredWindow(GetManager()->GetPaintWindow(), NULL, &pt, &szWindow, hCloneDc,
        &ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

        ::SelectObject(hCloneDc, hOldBitmap);*/
        

        //BitBlt(hDC, 0, 0, width, height, hCloneDc, 0, 0, SRCCOPY);

#endif

#if 0
        LOGFONT lf = { 0 };
        ::GetObject(GetManager()->GetFont(m_iFont), sizeof(LOGFONT), &lf);
        lf.lfHeight = -pen_size_*scale_;
        HFONT hFont = ::CreateFontIndirect(&lf);


        Gdiplus::Graphics graphics(hDC);
        graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

        Gdiplus::Font font(hDC, hFont);
        Gdiplus::RectF rectF((Gdiplus::REAL)rc.left, (Gdiplus::REAL)rc.top, (Gdiplus::REAL)(rc.right - rc.left), (Gdiplus::REAL)(rc.bottom - rc.top));
        Gdiplus::SolidBrush brush(Gdiplus::Color(255, GetBValue(pen_color_), GetGValue(pen_color_), GetRValue(pen_color_)));
        //Gdiplus::SolidBrush brush(Gdiplus::Color(255, GetBValue(0), GetGValue(0), GetRValue(0)));

        Gdiplus::StringFormat stringFormat = Gdiplus::StringFormat::GenericTypographic();
        UINT uStyle = DT_LEFT;
        if ((uStyle & DT_END_ELLIPSIS) != 0) {
            stringFormat.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
        }

        int formatFlags = 0;
        if ((uStyle & DT_NOCLIP) != 0) {
            formatFlags |= Gdiplus::StringFormatFlagsNoClip;
        }
        if ((uStyle & DT_SINGLELINE) != 0) {
            formatFlags |= Gdiplus::StringFormatFlagsNoWrap;
        }

        stringFormat.SetFormatFlags(formatFlags);

        if ((uStyle & DT_LEFT) != 0) {
            stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
        }
        else if ((uStyle & DT_CENTER) != 0) {
            stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
        }
        else if ((uStyle & DT_RIGHT) != 0) {
            stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);
        }
        else {
            stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
        }
        stringFormat.GenericTypographic();
        if ((uStyle & DT_TOP) != 0) {
            stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
        }
        else if ((uStyle & DT_VCENTER) != 0) {
            stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
        }
        else if ((uStyle & DT_BOTTOM) != 0) {
            stringFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);
        }
        else {
            stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
        }
        Gdiplus::SolidBrush bkbrush(Gdiplus::Color(200, GetBValue(135), GetGValue(206), GetRValue(235)));

        //graphics.FillRectangle(&bkbrush, rectF);

        graphics.DrawString(GetText(), -1, &font, rectF, &stringFormat, &brush);
        
        ::DeleteObject(hFont);

#endif
        if (m_bVScrollBarFixing) {
            LONG lWidth = rc.right - rc.left + m_pVerticalScrollBar->GetFixedWidth();
            LONG lHeight = 0;
            SIZEL szExtent = { -1, -1 };
            m_pTwh->GetTextServices()->TxGetNaturalSize(
                DVASPECT_CONTENT,
                GetManager()->GetPaintDC(),
                NULL,
                NULL,
                TXTNS_FITTOCONTENT,
                &szExtent,
                &lWidth,
                &lHeight);
            if (lHeight <= rc.bottom - rc.top) {
                NeedUpdate();
            }
        }

        /*int nSize = GetTextLength();
        if (nSize == 0)
            nSize = 1;

        Edit_SetSel(GetManager()->GetPaintWindow(), 0, nSize);*/

    }

    DrawCaret(hDC, rcPaint);

    if( m_items.GetSize() > 0 ) {
        RECT rc = m_rcItem;
        rc.left += m_rcInset.left;
        rc.top += m_rcInset.top;
        rc.right -= m_rcInset.right;
        rc.bottom -= m_rcInset.bottom;
        if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

        if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
            for( int it = 0; it < m_items.GetSize(); it++ ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                if( !pControl->IsVisible() ) continue;
                if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
                if( pControl ->IsFloat() ) {
                    if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
                    pControl->DoPaint(hDC, rcPaint);
                }
            }
        }
        else {
            CRenderClip childClip;
            CRenderClip::GenerateClip(hDC, rcTemp, childClip);
            for( int it = 0; it < m_items.GetSize(); it++ ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                if( !pControl->IsVisible() ) continue;
                if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
                if( pControl ->IsFloat() ) {
                    if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
                    CRenderClip::UseOldClipBegin(hDC, childClip);
                    pControl->DoPaint(hDC, rcPaint);
                    CRenderClip::UseOldClipEnd(hDC, childClip);
                }
                else {
                    if( !::IntersectRect(&rcTemp, &rc, &pControl->GetPos()) ) continue;
                    pControl->DoPaint(hDC, rcPaint);
                }
            }
        }
    }

    if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
        if( ::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()) ) {
            m_pVerticalScrollBar->DoPaint(hDC, rcPaint);
        }
    }

    if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
        if( ::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos()) ) {
            m_pHorizontalScrollBar->DoPaint(hDC, rcPaint);
        }
    }
}

void CTextToolUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("vscrollbar")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_DISABLENOSCROLL | WS_VSCROLL;
    }
    else if( _tcscmp(pstrName, _T("autovscroll")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_AUTOVSCROLL;
    }
    else if( _tcscmp(pstrName, _T("hscrollbar")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_DISABLENOSCROLL | WS_HSCROLL;
    }
    else if( _tcscmp(pstrName, _T("autohscroll")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_AUTOHSCROLL;
    }
    else if( _tcscmp(pstrName, _T("wanttab")) == 0 ) {
        SetWantTab(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("wantreturn")) == 0 ) {
        SetWantReturn(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("wantctrlreturn")) == 0 ) {
        SetWantCtrlReturn(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("rich")) == 0 ) {
        SetRich(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("multiline")) == 0 ) {
        if( _tcscmp(pstrValue, _T("false")) == 0 ) m_lTwhStyle &= ~ES_MULTILINE;
    }
    else if( _tcscmp(pstrName, _T("readonly")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) { m_lTwhStyle |= ES_READONLY; m_bReadOnly = true; }
    }
    else if( _tcscmp(pstrName, _T("password")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_PASSWORD;
    }
    else if( _tcscmp(pstrName, _T("align")) == 0 ) {
        if( _tcsstr(pstrValue, _T("left")) != NULL ) {
            m_lTwhStyle &= ~(ES_CENTER | ES_RIGHT);
            m_lTwhStyle |= ES_LEFT;
        }
        if( _tcsstr(pstrValue, _T("center")) != NULL ) {
            m_lTwhStyle &= ~(ES_LEFT | ES_RIGHT);
            m_lTwhStyle |= ES_CENTER;
        }
        if( _tcsstr(pstrValue, _T("right")) != NULL ) {
            m_lTwhStyle &= ~(ES_LEFT | ES_CENTER);
            m_lTwhStyle |= ES_RIGHT;
        }
    }
    else if( _tcscmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
        while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetTextColor(clrColor);
    }
	else if( _tcscmp(pstrName, _T("maxchar")) == 0 ) SetLimitText(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
	else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
	else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
	else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
	else if( _tcscmp(pstrName, _T("textpadding")) == 0 ) {
		RECT rcTextPadding = { 0 };
		LPTSTR pstr = NULL;
		rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
		SetTextPadding(rcTextPadding);
	}
	else if( _tcscmp(pstrName, _T("tipvalue")) == 0 ) SetTipValue(pstrValue);
	else if( _tcscmp(pstrName, _T("tipvaluecolor")) == 0 ) SetTipValueColor(pstrValue);
    else CContainerUI::SetAttribute(pstrName, pstrValue);
}

LRESULT CTextToolUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    if( !IsVisible() || !IsEnabled() ) return 0;
    if( !IsMouseEnabled() && uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST ) return 0;
    if( uMsg == WM_MOUSEWHEEL && (LOWORD(wParam) & MK_CONTROL) == 0 ) return 0;

	if (uMsg == WM_IME_COMPOSITION)
	{
		// 解决微软输入法位置异常的问题
		HIMC hIMC = ImmGetContext(GetManager()->GetPaintWindow());
		if (hIMC) 
		{
			// Set composition window position near caret position
			POINT point;
			GetCaretPos(&point);

			COMPOSITIONFORM Composition;
			Composition.dwStyle = CFS_POINT;
			Composition.ptCurrentPos.x = point.x;
			Composition.ptCurrentPos.y = point.y;
			ImmSetCompositionWindow(hIMC, &Composition);

			ImmReleaseContext(GetManager()->GetPaintWindow(),hIMC);
		}

		return 0;
	}

    bool bWasHandled = true;
    if( (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) || uMsg == WM_SETCURSOR ) {
        if( !m_pTwh->IsCaptured() ) {
            switch (uMsg) {
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_LBUTTONDBLCLK:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
                {
                    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                    CControlUI* pHover = GetManager()->FindControl(pt);
                    if(pHover != this) {
                        bWasHandled = false;
                        return 0;
                    }
                }
                break;
            }
        }
        // Mouse message only go when captured or inside rect
        DWORD dwHitResult = m_pTwh->IsCaptured() ? HITRESULT_HIT : HITRESULT_OUTSIDE;
        if( dwHitResult == HITRESULT_OUTSIDE ) {
            RECT rc;
            m_pTwh->GetControlRect(&rc);
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if( uMsg == WM_MOUSEWHEEL ) ::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
            if( ::PtInRect(&rc, pt) && !GetManager()->IsCaptured() ) dwHitResult = HITRESULT_HIT;
        }
        if( dwHitResult != HITRESULT_HIT ) return 0;
        if( uMsg == WM_SETCURSOR ) bWasHandled = false;
        else if( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK || uMsg == WM_RBUTTONDOWN ) {
            SetFocus();
        }
    }
#ifdef _UNICODE
    else if( uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST ) {
#else
    else if( (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) || uMsg == WM_CHAR || uMsg == WM_IME_CHAR ) {
#endif
        if( !IsFocused() ) return 0;
    }
    else if( uMsg == WM_CONTEXTMENU ) {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
        CControlUI* pHover = GetManager()->FindControl(pt);
        if(pHover != this) {
            bWasHandled = false;
            return 0;
        }
    }
    else
    {
        switch( uMsg ) {
        case WM_HELP:
            bWasHandled = false;
            break;
        default:
            return 0;
        }
    }

	if(WM_CHAR == uMsg)
	{
#ifndef _UNICODE
		// check if we are waiting for 2 consecutive WM_CHAR messages
		if ( IsAccumulateDBCMode() )
		{
			if ( (GetKeyState(VK_KANA) & 0x1) )
			{
				// turn off accumulate mode
				SetAccumulateDBCMode ( false );
				m_chLeadByte = 0;
			}
			else
			{
				if ( !m_chLeadByte )
				{
					// This is the first WM_CHAR message, 
					// accumulate it if this is a LeadByte.  Otherwise, fall thru to
					// regular WM_CHAR processing.
					if ( IsDBCSLeadByte ( (WORD)wParam ) )
					{
						// save the Lead Byte and don't process this message
						m_chLeadByte = (WORD)wParam << 8 ;

						//TCHAR a = (WORD)wParam << 8 ;
						return 0;
					}
				}
				else
				{
					// This is the second WM_CHAR message,
					// combine the current byte with previous byte.
					// This DBC will be handled as WM_IME_CHAR.
					wParam |= m_chLeadByte;
					uMsg = WM_IME_CHAR;

					// setup to accumulate more WM_CHAR
					m_chLeadByte = 0; 
				}
			}
		}
#endif
	}

    LRESULT lResult = 0;
    HRESULT Hr = TxSendMessage(uMsg, wParam, lParam, &lResult);
    if( Hr == S_OK ) bHandled = bWasHandled;
    else if( (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) || uMsg == WM_CHAR || uMsg == WM_IME_CHAR )
        bHandled = bWasHandled;
    else if( uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST ) {
        if( m_pTwh->IsCaptured() ) bHandled = bWasHandled;
    }
    return lResult;
}

void CTextToolUI::SetAccumulateDBCMode( bool bDBCMode )
{
	m_fAccumulateDBC = bDBCMode;
}

bool CTextToolUI::IsAccumulateDBCMode()
{
	return m_fAccumulateDBC;
}


bool CTextToolUI::ShowCaret(bool bShow)
{
    if (m_bCaretShowing == bShow)
        return true;

    m_bCaretShowing = bShow;
    if (!bShow)
    {
        GetManager()->KillTimer(this, kCaretTimerID);
        if (m_bCaretActive)
        {
            ::InvalidateRect(GetManager()->GetPaintWindow(), &m_rtCaret, FALSE);
        }
        m_bCaretActive = false;
    }
    else
    {
        m_pManager->SetTimer(this, kCaretTimerID, ::GetCaretBlinkTime());
        if (!m_bCaretActive)
        {
            ::InvalidateRect(GetManager()->GetPaintWindow(), &m_rtCaret, FALSE);
            m_bCaretActive = true;
        }
    }

    return true;
}

bool CTextToolUI::SetCaretPos(int x, int y)
{
    if (!::SetCaretPos(x, y))
        return false;

    RECT tempRt = m_rtCaret;
    int w = m_rtCaret.right - m_rtCaret.left;
    int h = m_rtCaret.bottom - m_rtCaret.top;
    m_rtCaret.left = x;
    m_rtCaret.top = y;
    m_rtCaret.right = x + w;
    m_rtCaret.bottom = y + h;
    ::InvalidateRect(GetManager()->GetPaintWindow(), &tempRt, FALSE);
    ::InvalidateRect(GetManager()->GetPaintWindow(), &m_rtCaret, FALSE);

    return true;
}


bool CTextToolUI::CreateCaret(HBITMAP hBmp, int nWidth, int nHeight)
{
    ::CreateCaret(GetManager()->GetPaintWindow(), hBmp, nWidth, nHeight);
    //TODO hBmp处理位图光标
    m_rtCaret.right = m_rtCaret.left + nWidth;
    m_rtCaret.bottom = m_rtCaret.top + nHeight;
    return true;
}

void CTextToolUI::DrawCaret(HDC hDC, const RECT& rcPaint)
{
    if ((!IsFocused() || m_pManager->GetPaintWindow() != ::GetFocus()))
    {
        GetManager()->KillTimer(this, kCaretTimerID);
        if (m_bCaretActive)
        {
            ::InvalidateRect(m_pManager->GetPaintWindow(), &m_rtCaret, FALSE);
        }
        m_bCaretActive = false;
        return;
    }

    if (m_bCaretActive && m_bCaretShowing)
    {
        RECT temp = {};
        if (::IntersectRect(&temp, &rcPaint, &m_rtCaret))
        {
            DWORD dwColor = GetTextColor();
            if (dwColor == 0)
                dwColor = 0xffff0000;
            CRenderEngine::DrawColor(hDC, temp, dwColor);
        }
    }
}

CTextToolUI* CTextToolUI::CreateTools(DWORD pen_color, int pen_size, float scale)
{
    int font_size = 16;
    if (pen_size == 0)
        font_size = 160;
    else if (pen_size == 1)
        font_size = 200;
    else if (pen_size == 2)
        font_size = 240;
    
    CTextToolUI* text_tool = new CTextToolUI;
    text_tool->SetFloat(true);
    text_tool->SetBorderColor(0xFF2A89FF);
    text_tool->SetBorderSize(1);
    text_tool->SetPenColor(pen_color);
    text_tool->SetTextColor(pen_color);
    text_tool->SetPenSize(font_size);
    text_tool->SetFont(1);
    text_tool->SetRich(false);
    text_tool->SetInset(RECT{ 3, 3, 3, 3 });
    text_tool->SetTextPadding(RECT{ 3, 3, 3, 3 });
    text_tool->SetToolsScale(scale);
    text_tool->SetMouseEnabled(true);
    text_tool->SetLimitText(150);

    DWORD lMask = text_tool->GetEventMask();
    lMask |= ENM_CHANGE;
    lMask &= ~ENM_PROTECTED;
    lMask &= ~ES_MULTILINE;
    text_tool->SetEventMask(lMask);

    return text_tool;
}

void CTextToolUI::SetMainCanvas(MainCanvas* main_canvas)
{
    main_canvas_ = main_canvas;
}



Json::Value CTextToolUI::Marshal()
{
    left_ = m_rcItem.left / scale_;
    top_ = m_rcItem.top / scale_ + offset_y_;
    right_ = m_rcItem.right / scale_;
    bottom_ = m_rcItem.bottom / scale_ + offset_y_;

    Json::Value tool = ToolBase::Marshal();
    Json::Value ext;
    std::wstring str = GetText();
    ext["text_content"] = UnicodeToUTF8(str.c_str());
    if (str != L"")
    {
        int line = GetLineCount();
        for (int n = 0; n < line; n++)
        {
            std::string content = UnicodeToUTF8(GetLine(n, 512));
            ext["text_contents"].append(content);
        }
    }
    tool["ext"] = ext;
    return tool;
}

bool CTextToolUI::UnMarshal(Json::Value& tool)
{
    if (!ToolBase::UnMarshal(tool))
        return false;

    RECT rect;
    rect.left = left_ * scale_;
    rect.top = (top_ - offset_y_) * scale_;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - offset_y_) * scale_;
    SetPos(rect);

    SetFontSize(pen_size_*scale_);

    Json::Value ext = tool["ext"];
    std::string context = ext["text_content"].asString();
    SetText(UTF8ToUnicode(context.c_str()).c_str());
    

    main_canvas_->SetCanvasFocus();

    return true;
}

void CTextToolUI::SetToolsScale(float scale)
{
    scale_ = scale;
    RECT rect;
    rect.left = left_ * scale_;
    rect.top = (top_ - offset_y_) * scale_;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - offset_y_) * scale_;
    SetPos(rect);

    if (!m_bInited)
        return;
    SetFontSize(pen_size_*scale_);
}

void CTextToolUI::SetOffsetY(int offset_y)
{
    offset_y_ = offset_y;
    RECT rect;

    rect.left = left_ * scale_;
    rect.top = (top_ - offset_y) * scale_;
    rect.right = right_ * scale_;
    rect.bottom = (bottom_ - offset_y) * scale_;

    SetPos(rect);
    //ToolBase::SetOffsetY(offset_y);
}

void CTextToolUI::SetToolsOffset(int offsetx, int offsety)
{
    RECT rect;
    rect = GetPos();
    rect.left += offsetx;
    rect.right += offsetx;
    rect.top += offsety;
    rect.bottom += offsety;
    SetPos(rect);

    left_ = m_rcItem.left / scale_;
    top_ = m_rcItem.top / scale_;
    right_ = m_rcItem.right / scale_;
    bottom_ = m_rcItem.bottom / scale_;
}

int CTextToolUI::ToolHitTest(POINT pt)
{
    RECT rc = m_rcItem;
    if (!::PtInRect(&rc, pt))
    {
        return -1;
    }
    else
    {
        for (int i = 0; i < 9; ++i) {
            RECT rt = GetSizerRect(i);
            if (::PtInRect(&rt, pt))
                return i;
        }
        return 4;
    }
    /*if (!::PtInRect(&m_rcItem, pt))
    {
    return false;
    }
    return true;*/
}


void CTextToolUI::SetToolsState(ToolsStats status)
{
    tools_status_ = status;
    Invalidate();
}


RECT CTextToolUI::GetMinRect()
{
    RECT rcText = { 0 };
    RECT rcInset = GetInset();
    GetTextSize(rcText, L"1");
    rcText.bottom += (rcInset.left + rcInset.right);
    return rcText;
}


void CTextToolUI::GetTextSize(RECT &rect, wstring text)
{
    HFONT hFont = GetManager()->GetFont(GetFont());
    HDC hDC = m_pManager->GetPaintDC();
    GetTextRectSize(rect, text, hFont, hDC, pen_color_, pen_size_*scale_);
}



void CTextToolUI::Notify(TNotifyUI& msg)
{
    if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
    {
        if (msg.pSender == this)
        {
            ResizeTextTool();

            if (change_notify_enable_)
            {
                if (main_canvas_)
                    main_canvas_->OnTextChanged(this);
            }
        }
        
    }
    else if (msg.sType == DUI_MSGTYPE_SETFOCUS)
    {
        if (msg.pSender == this)
        {
            msg.pSender->SetFocus();
            //msg.pSender->SetBorderSize(1);
            show_border_ = true;
            Invalidate();
        }
        //if (msg.pSender == this)
        //{
        //    OutputDebugStringA("DUI_MSGTYPE_SETFOCUS \r\n");
        //    msg.pSender->SetFocus();
        //    //SetBorderSize(1);
        //}
    }
    else if (msg.sType == DUI_MSGTYPE_KILLFOCUS)
    {
        //msg.pSender->SetBorderSize(0);
        if (msg.pSender == this)
        {
            show_border_ = false;
            Invalidate();

            if (wcscmp(GetText(), L"") == 0 )
            {
                if (main_canvas_)
                    main_canvas_->OnTextControlRemoved(this);
            }
            
        }

    }

}


void CTextToolUI::ResizeTextTool()
{
#if 0
    RECT rcInset = GetInset();
    RECT rcClipPadding = main_canvas_->GetCanvasContainerRect();
    RECT rcText = { 0 };
    CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, GetText(), GetTextColor(), GetFont(), DT_CALCRECT);
    //m_cxyFixed.cx = rcText.right - rcText.left + m_rcTextPadding.left + m_rcTextPadding.right;

    int text_width = rcText.right - rcText.left;
    int text_height = rcText.bottom - rcText.top;
    int free_widht = rcClipPadding.right - m_rcItem.left - rcInset.left - rcInset.right - m_rcInset.left - m_rcInset.right;
    int row_count = (text_width + free_widht) / free_widht;

    m_rcItem.right = m_rcItem.left + text_width + rcInset.left + rcInset.right + m_rcInset.left + m_rcInset.right + 6;
    m_rcItem.bottom = m_rcItem.top + text_height*row_count + rcInset.top + rcInset.bottom + m_rcInset.bottom + m_rcInset.top + 4;

    //最小宽度
    if (m_rcItem.right - m_rcItem.left <= min_width_)
        m_rcItem.right = m_rcItem.left + min_width_;

    //最大宽度
    if (rcClipPadding.right <= m_rcItem.right)
        m_rcItem.right = rcClipPadding.right;

    //最小高度
    if (rcText.bottom == 0)
    {
        RECT rcMin = GetMinRect();
        m_rcItem.bottom = m_rcItem.top + rcMin.bottom /*+ rcInset.top + rcInset.bottom*/ + m_rcInset.bottom + m_rcInset.top;
        SetPos(m_rcItem);
    }
    SetPos(m_rcItem);
#endif

#if 1

    


    //RECT rcInset = GetInset();
    RECT rcClipPadding = main_canvas_->GetCanvasContainerRect();
    RECT rcText = { 0 };
    
    int row_count_total = 0;
    int text_width = 0;
    int text_height = 0;
    wstring txt = GetText();
    std::vector<std::wstring> strs = Splitw(txt, L"\r");

#if 0
    for (size_t n = 0; n < strs.size(); n++)
    {
        std::wstring str = strs[n];

        RECT rc = { 0 };
        GetTextSize(rc, str);

        int tmp_width = rc.right - rc.left;

        int free_widht = rcClipPadding.right - m_rcItem.left /*- m_rcInset.left - m_rcInset.right - m_rcInset.left - m_rcInset.right*/;
        int row_count = (tmp_width + free_widht) / free_widht;


        

        row_count_total += row_count;

        wchar_t info[256] = {0};
        swprintf(info, L"txt: %s, len: %d, \r\n", str.c_str(), tmp_width);
        OutputDebugStringW(info);

        if (tmp_width > text_width)
            text_width = tmp_width;
            if (rc.bottom - rc.top > text_height)
            text_height = rc.bottom - rc.top;


        

    }
#endif
    int free_widht = rcClipPadding.right - m_rcItem.left - 10;
    LONG lWidth = free_widht;
    LONG lHeight = 0;
    SIZEL szExtent = { -1, -1 };
    m_pTwh->GetTextServices()->TxGetNaturalSize(
        DVASPECT_CONTENT,
        GetManager()->GetPaintDC(),
        NULL,
        NULL,
        TXTNS_FITTOCONTENT,
        &szExtent,
        &lWidth,
        &lHeight);
    text_width = lWidth;
    text_height = lHeight;

    int line_count = GetLineCount();

    if (strs[strs.size() - 1] == L"" && line_count != 1)
    {
        int height_per_line = lHeight / (line_count - 1);
        text_height = height_per_line * line_count;
    }

    //if (text_height < text_line_height * line_count)
    //{
    //    //if (strs[strs.size() - 1] != L"\r")
    //    //{
    //    //    //text_height += text_line_height;
    //    //    line_count--;
    //    //}

    //    text_height = text_line_height * line_count;
    //}

    

    wchar_t info[256] = { 0 };
    swprintf(info, L"[canvas view] txt:  %s, len: %d, widht: %d \r\n", txt.c_str(), lWidth, text_width);
    OutputDebugStringW(info);

    swprintf(info, L"[canvas view] 1111: {%d, %d, %d, %d}, \r\n", m_rcItem.left, text_width, m_rcInset.left, m_rcInset.right);
    OutputDebugStringW(info);

    m_rcItem.right = m_rcItem.left + text_width + 6/*m_rcInset.left + m_rcInset.right*/ + 6/*m_rcTextPadding.left + m_rcTextPadding.right*/;
    //m_rcItem.bottom = m_rcItem.top + text_height*row_count_total + m_rcInset.top + m_rcInset.bottom +  m_rcTextPadding.top + m_rcTextPadding.bottom;
    m_rcItem.bottom = m_rcItem.top + text_height + 6 /*m_rcInset.top + m_rcInset.bottom*/ + 6/*m_rcTextPadding.top + m_rcTextPadding.bottom*/;

    swprintf(info, L"[canvas view] 2222: {%d, %d, %d, %d}, \r\n", m_rcItem.left, m_rcInset.top, m_rcItem.right, m_rcItem.bottom);
    OutputDebugStringW(info);

    //最小宽度
    //if (m_rcItem.right - m_rcItem.left <= min_width_)
        //m_rcItem.right = m_rcItem.left + min_width_;

    //最大宽度
    /*if (rcClipPadding.right <= m_rcItem.right)
        m_rcItem.right = rcClipPadding.right;*/


    //最大高度
    int height = m_rcItem.bottom - m_rcItem.top;
    if (rcClipPadding.bottom <= m_rcItem.bottom)
    {
        m_rcItem.bottom = rcClipPadding.bottom;
        m_rcItem.top = m_rcItem.bottom - height;
    }

    //最小高度
    //if (rcText.bottom == 0)
    //{
    //    RECT rcMin = GetMinRect();
    //    m_rcItem.bottom = m_rcItem.top + rcMin.bottom /*+ rcInset.top + rcInset.bottom*/ + m_rcInset.bottom + m_rcInset.top;
    //    //rich_edit_->SetPos(m_rcItem);
    //}
    swprintf(info, L"[canvas view] 3333: {%d, %d, %d, %d}, \r\n", m_rcItem.left, m_rcInset.top, m_rcItem.right, m_rcItem.bottom);
    OutputDebugStringW(info);
    SetPos(m_rcItem);
    

    //CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, rich_edit_->GetText(), rich_edit_->GetTextColor(), rich_edit_->GetFont(), DT_CALCRECT);
    //m_cxyFixed.cx = rcText.right - rcText.left + m_rcTextPadding.left + m_rcTextPadding.right;
#if 0
    int text_width = rcText.right - rcText.left;
    int text_height = rcText.bottom - rcText.top;
    int free_widht = rcClipPadding.right - m_rcItem.left - m_rcInset.left - m_rcInset.right - m_rcInset.left - m_rcInset.right;
    int row_count = (text_width + free_widht) / free_widht;
    
    m_rcItem.right = m_rcItem.left + text_width + m_rcInset.left + m_rcInset.right + m_rcInset.left + m_rcInset.right;
    m_rcItem.bottom = m_rcItem.top + text_height*row_count + m_rcInset.top + m_rcInset.bottom + m_rcInset.bottom + m_rcInset.top;


    //最小宽度
    if (m_rcItem.right - m_rcItem.left <= min_width_)
        m_rcItem.right = m_rcItem.left + min_width_;

    //最大宽度
    if (rcClipPadding.right <= m_rcItem.right)
        m_rcItem.right = rcClipPadding.right;


    //最大高度
    int height = m_rcItem.bottom - m_rcItem.top;
    if (rcClipPadding.bottom <= m_rcItem.bottom)
    {
        m_rcItem.bottom = rcClipPadding.bottom;
        m_rcItem.top = m_rcItem.bottom - height;
    }

    //最小高度
    if (rcText.bottom == 0)
    {
        RECT rcMin = GetMinRect();
        m_rcItem.bottom = m_rcItem.top + rcMin.bottom /*+ rcInset.top + rcInset.bottom*/ + m_rcInset.bottom + m_rcInset.top;
        //rich_edit_->SetPos(m_rcItem);
    }
    
    SetPos(m_rcItem);

#endif

#endif
}


void CTextToolUI::PaintBorder(HDC hDC)
{
    RECT rc = m_rcItem;
    //CRenderEngine::DrawRect(hDC, rc, 1, 0xff00ff00);
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;
    //CRenderEngine::DrawRect(hDC, rc, 1, 0xff00ff00);
    //矩形
    //if (tools_status == TOOLS_NORMAL)
    if (show_border_)
    {
        Gdiplus::Graphics cs(hDC);
        cs.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
        cs.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQuality);

        //int offset = (pen_size_ + 1)*scale_ / 2;
        //Pen pen(Gdiplus::Color(pen_color_), pen_size_*scale_);
        int offset = 1;
        Pen pen(Gdiplus::Color(0xFF2A89FF), GetBorderSize());
        //cs.DrawRectangle(&pen, rc.left + offset, rc.top + offset, rc.right - rc.left - 2 * offset, rc.bottom - rc.top - 2 * offset);
        cs.DrawRectangle(&pen, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    }

    if (tools_status_ == TOOLS_HOVER)
    {
        CRenderEngine::DrawRect(hDC, rc, 1, m_dwBorderColor);
    }
    else if (tools_status_ == TOOLS_EDITABLE)
    {
        for (int i = 0; i < 9; ++i) {
            CRenderEngine::DrawColor(hDC, GetSizerRect(i), 0xFF2A89FF);
        }
        CRenderEngine::DrawRect(hDC, rc, 1, m_dwBorderColor);
    }
    else if (tools_status_ == TOOLS_SELECTED)
    {
        CRenderEngine::DrawRect(hDC, rc, 3 /*m_rcBorderSize.left*/, m_dwBorderColor);
    }

}


RECT CTextToolUI::GetSizerRect(int iIndex)
{
    LONG lMiddleX = (m_rcItem.left + m_rcItem.right) / 2;
    LONG lMiddleY = (m_rcItem.top + m_rcItem.bottom) / 2;
    LONG SIZER_WIDTH = m_rcInset.left * 2;
    switch (iIndex) {
    case 0:
        return CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left + SIZER_WIDTH, m_rcItem.top + SIZER_WIDTH);
    case 1:
        return CDuiRect(lMiddleX - SIZER_WIDTH / 2, m_rcItem.top, lMiddleX + SIZER_WIDTH / 2, m_rcItem.top + SIZER_WIDTH);
    case 2:
        return CDuiRect(m_rcItem.right - SIZER_WIDTH, m_rcItem.top, m_rcItem.right, m_rcItem.top + SIZER_WIDTH);
    case 3:
        return CDuiRect(m_rcItem.left, lMiddleY - SIZER_WIDTH / 2, m_rcItem.left + SIZER_WIDTH, lMiddleY + SIZER_WIDTH / 2);
    case 4:
        break;
    case 5:
        return CDuiRect(m_rcItem.right - SIZER_WIDTH, lMiddleY - SIZER_WIDTH / 2, m_rcItem.right, lMiddleY + SIZER_WIDTH / 2);
    case 6:
        return CDuiRect(m_rcItem.left, m_rcItem.bottom - SIZER_WIDTH, m_rcItem.left + SIZER_WIDTH, m_rcItem.bottom);
    case 7:
        return CDuiRect(lMiddleX - SIZER_WIDTH / 2, m_rcItem.bottom - SIZER_WIDTH, lMiddleX + SIZER_WIDTH / 2, m_rcItem.bottom);
    case 8:
        return CDuiRect(m_rcItem.right - SIZER_WIDTH, m_rcItem.bottom - SIZER_WIDTH, m_rcItem.right, m_rcItem.bottom);
    default:
        break;
    }
    return CDuiRect();
}

void CTextToolUI::SetChangeNotify(bool enable)
{
    change_notify_enable_ = enable;
}

void CTextToolUI::SetFocusOuter(bool focus)
{
    if (focus) {
        if (m_pTwh) {
            m_pTwh->OnTxInPlaceActivate(NULL);
            if (GetText() == m_sTipValue)
            {
                SetText(_T(""));
                m_pTwh->SetColor(m_dwTextColor);
            }
            m_pTwh->GetTextServices()->TxSendMessage(WM_SETFOCUS, 0, 0, 0);
        }
        m_bFocused = true;
        ShowCaret(true);
        Invalidate();
    }
    else
    {
        if (m_pTwh) {
            m_pTwh->OnTxInPlaceActivate(NULL);
            if (GetText() == _T("") && !m_sTipValue.IsEmpty())
            {
                SetText(m_sTipValue);
                m_pTwh->SetColor(m_dwTipValueColor);
            }
            m_pTwh->GetTextServices()->TxSendMessage(WM_KILLFOCUS, 0, 0, 0);
        }
        m_bFocused = false;
        Invalidate();
    }
}


} // namespace DuiLib
