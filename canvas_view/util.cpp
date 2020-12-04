#include "StdAfx.h"
#include "Util.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION

#pragma comment(lib, "Rpcrt4.lib") 




std::string GenUUID()
{
    std::string guid(16, 0);

    UUID uuid;
    UuidCreate(&uuid);
    unsigned char* str;
    UuidToStringA(&uuid, &str);
    guid = reinterpret_cast<char*>(str);
    RpcStringFreeA(&str);

    std::string::size_type idx = 0;
    while ((idx = guid.find("-", idx)) != std::string::npos)
    {
        guid.erase(idx, 1);
    }

    return guid;

}

#define EPS		1.0e-2f
#define PAI     3.1415926

long MapFun_Point2Line(POINT pt1, POINT pt2, POINT pnt, POINT &sp, double *hdist)
{
    double   dx, dy, sq, t;
    int      ier = -1;

    *hdist = 0.0;
    dx = pt2.x - pt1.x;    dy = pt2.y - pt1.y;
    sq = (dx * dx) + (dy * dy);

    if (fabs(sqrt(sq)) < EPS)  goto OVER;

    t = (dy * (pnt.x - pt1.x) - dx * (pnt.y - pt1.y)) / sq;

    *hdist = sqrt(sq) * fabs(t);
    sp.x = pnt.x - dy * t;   sp.y = pnt.y + dx * t;
    t = (sp.x - pt1.x) * (sp.x - pt2.x) +
        (sp.y - pt1.y) * (sp.y - pt2.y);
    if (t <= EPS)   ier = 0;
    else            ier = 1;

OVER:
    return (ier);
}

bool MapFun_Point2Rect(RECT rect, POINT pt, int ignore)
{   

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int ignorex = ignore;
    int ignorey = ignore;
    if (ignorex>width/2.0)
    {
        ignorex = width / 2.0;
    }
    if (ignorey > height / 2.0)
    {
        ignorey = height / 2.0;
    }

    RECT rectBig = { rect.left - ignorex, rect.top - ignorey, rect.right + ignorex, rect.bottom + ignorey };
    RECT rectMin = { rect.left + ignorex, rect.top + ignorey, rect.right - ignorex, rect.bottom - ignorey };


    if (!::PtInRect(&rectBig, pt))
        return false;
    if (::PtInRect(&rectMin, pt))
        return false;
    return true;
   
   
}

bool GetImageSize(wstring filepath, int &width, int &height)
{
    TImageInfo* pImage = CRenderEngine::LoadImage(filepath.c_str());
    if (pImage == NULL)
        return false;
    int picW = pImage->nX;
    int picH = pImage->nY;

    if (picW * picH == 0)
    {
        return false;
    }

    //不做映射 得到图片的原始大小
    if (width * height == 0)
    {
        width = picW;
        height = picH;
    }
    else
    {
        int dstW = width;
        int dstH = height;

        height = dstW * picH / picW;
        if (height > dstH)
        {
            height = dstH;
            width = height * picW / picH;
        }
    }
    


    delete pImage;
    pImage = NULL;
    return true;
}

bool GetTextRectSize(RECT &rect, std::wstring text, HFONT hFontSrc, HDC hDC, DWORD color, int size)
{
    if (hFontSrc == NULL || hDC == NULL)
        return false;
    LOGFONT lf = { 0 };
    ::GetObject(hFontSrc, sizeof(LOGFONT), &lf);
    if (size > 0)
    {
        lf.lfHeight = -size;
    }
    HFONT hFont = ::CreateFontIndirect(&lf);

    Gdiplus::Graphics graphics(hDC);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
    Gdiplus::Font font(hDC, hFont);
    Gdiplus::RectF rectF((Gdiplus::REAL)rect.left, (Gdiplus::REAL)rect.top, (Gdiplus::REAL)(rect.right - rect.left), (Gdiplus::REAL)(rect.bottom - rect.top));
    Gdiplus::SolidBrush brush(Gdiplus::Color(254, GetBValue(color), GetGValue(color), GetRValue(color)));
    Gdiplus::StringFormat stringFormat = Gdiplus::StringFormat::GenericTypographic();

    stringFormat.SetFormatFlags(0);
    stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
    stringFormat.GenericTypographic();
    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
    Gdiplus::RectF bounds;
    graphics.MeasureString(text.c_str(), -1, &font, rectF, &stringFormat, &bounds);

    // MeasureString存在计算误差，这里加一像素
    rect.bottom = rect.top + (long)bounds.Height + 1;
    rect.right = rect.left + (long)bounds.Width + 1;
    if (hFont)
        ::DeleteObject(hFont);

    return true;
   
}


std::string UnicodeToAnsi(const WCHAR* strSrc)
{
    std::string result;
    char* szRes;
    int i = 0;
    i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new char[i + 1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

    result = szRes;
    delete[]szRes;
    return result;
}

std::wstring AnsiToUnicode(const char* strSrc)
{
    std::wstring result;
    WCHAR* szRes;
    int i = 0;
    i = MultiByteToWideChar(CP_ACP, 0, strSrc, -1, NULL, 0)*sizeof(WCHAR);
    szRes = new WCHAR[i + 1];
    MultiByteToWideChar(CP_ACP, 0, strSrc, -1, szRes, i);
    result = szRes;;
    delete[]szRes;
    return result;
}

std::string UnicodeToUTF8(const WCHAR* strSrc)
{
    std::string result;
    char* szRes;
    int i = 0;
    i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new char[i + 1];
    WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);
    result = szRes;
    delete[]szRes;
    return result;
}

std::wstring UTF8ToUnicode(const char* strSrc)
{
    std::wstring result;
    WCHAR* szRes;
    int i = 0;
    i = MultiByteToWideChar(CP_UTF8, 0, strSrc, -1, NULL, 0)*sizeof(WCHAR);
    szRes = new WCHAR[i + 1];
    MultiByteToWideChar(CP_UTF8, 0, strSrc, -1, szRes, i);
    result = szRes;;
    delete[]szRes;
    return result;
}

std::vector< std::string > string_split(const std::string& str, char ch, unsigned int maxSplits /*= 0 */)
{
    std::vector<std::string> ret;
    unsigned int numSplits = 0;

    int start = 0;
    int pos = 0;
    do
    {
        pos = str.find(ch, start);
        if (pos == start)
        {
            ret.push_back("");
            start = pos + 1;
        }
        else if (pos == -1 || (maxSplits && numSplits == maxSplits))
        {
            ret.push_back(str.substr(start));
            break;
        }
        else
        {
            ret.push_back(str.substr(start, pos - start));
            start = pos + 1;
        }
        ++numSplits;

    } while (pos != -1);

    return ret;
}



std::string UTF8ToAnsi(const char* strSrc)
{
    return UnicodeToAnsi(UTF8ToUnicode(strSrc).c_str());
}

std::string AnsiToUTF8(const char* strSrc)
{
    return UnicodeToUTF8(AnsiToUnicode(strSrc).c_str());
}

std::vector<std::string> Split(std::string src, const char* str)
{
    std::vector<std::string> vecRet;
    size_t npos = src.find(str);
    while (npos != std::string::npos)
    {
        std::string tmp = src.substr(0, npos);
        vecRet.push_back(tmp);
        src = src.substr(npos + strlen(str));
        //
        npos = src.find(str);
    }
    vecRet.push_back(src);
    return vecRet;
}

std::vector<std::wstring> Splitw(std::wstring src, std::wstring str)
{
    std::vector<std::wstring> vecRet;
    size_t npos = src.find(str);
    while (npos != std::wstring::npos)
    {
        std::wstring tmp = src.substr(0, npos);
        vecRet.push_back(tmp);
        src = src.substr(npos + str.length());
        //
        npos = src.find(str);
    }
    vecRet.push_back(src);
    return vecRet;
}


DWORD StrColorToDword(std::string color)
{
    LPTSTR pstr = NULL;
    DWORD clrColor = _tcstoul(AnsiToUnicode(color.c_str()).c_str() + 1, &pstr, 16);
    return clrColor;
}
