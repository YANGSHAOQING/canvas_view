#pragma once

std::string GenUUID();

long MapFun_Point2Line(POINT pt1, POINT pt2, POINT pnt, POINT &sp, double *hdist);

bool MapFun_Point2Rect(RECT rect, POINT pt, int ignore);

//得到图像在指定范围内的实际大小
bool GetImageSize(wstring filepath, int &width, int &height);

bool GetTextRectSize(RECT &rect, std::wstring text, HFONT hFontSrc, HDC hDC, DWORD color, int size=-1);




/**	@fn	UnicodeToAnsi
*	@brief	Unicode转Ansi
*	@param strSrc Unicode编码格式的源字符串
*	@return	 转码后的Ansi字符串
*/
std::string UnicodeToAnsi(const WCHAR* strSrc);

/**	@fn	AnsiToUnicode
*	@brief	Ansi转Unicode
*	@param strSrc Ansi编码格式的源字符串
*	@return	 转码后的Unicode字符串
*/
std::wstring AnsiToUnicode(const char* strSrc);

/**	@fn	UnicodeToUTF8
*	@brief	Unicode转UTF8
*	@param strSrc Unicode编码格式的源字符串
*	@return	 转码后的UTF8字符串
*/
std::string UnicodeToUTF8(const WCHAR* strSrc);

/**	@fn	UTF8ToUnicode
*	@brief	UTF8转Unicode
*	@param strSrc UTF8编码格式的源字符串
*	@return	 转码后的Unicode字符串
*/
std::wstring UTF8ToUnicode(const char* strSrc);

/**@fn	UTF8ToAnsi
*	@brief 	UTF8转Ansi
*	@param strSrc UTF8编码格式的源字符串
*	@return	 转码后的UTF8字符串
*/
std::string UTF8ToAnsi(const char* strSrc);


/**@fn	AnsiToUTF8
*	@brief 	Ansi转UTF8
*	@param strSrc Ansi编码格式的源字符串
*	@return	 转码后的UTF8字符串
*/
std::string AnsiToUTF8(const char* strSrc);


std::vector<std::string> Split(std::string src, const char* str);
std::vector<std::wstring> Splitw(std::wstring src, std::wstring str);

//
DWORD StrColorToDword(std::string color );

void MeasureStringExtended(RECT &rect, std::wstring text, HFONT hFontSrc, HDC hDC,
    DWORD color, int size, int desWidth);

