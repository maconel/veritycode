
#ifndef __VERITYCODEDRAWER_H__
#define __VERITYCODEDRAWER_H__

class CVerityCodeDrawer
{
public:
	CVerityCodeDrawer();
	virtual ~CVerityCodeDrawer();
	void SetVerityCode(const char* verityCode);
	void Draw(HDC dc, const RECT& rect, COLORREF backColor = RGB(0xFF, 0xFF, 0xFF));
protected:
	CString m_verityCode;
	bool DrawChar(HDC dc, const RECT& rect, char c, COLORREF color, int angle, const char* fontName);
	COLORREF GetRandColor(BYTE minR, BYTE maxR, BYTE minG, BYTE maxG, BYTE minB, BYTE maxB);
};

#endif //__VERITYCODEDRAWER_H__
