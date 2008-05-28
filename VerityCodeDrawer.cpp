
#include "stdafx.h"
#include "VerityCodeDrawer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CVerityCodeDrawer::CVerityCodeDrawer()
{
}

CVerityCodeDrawer::~CVerityCodeDrawer()
{
}

void CVerityCodeDrawer::SetVerityCode(const char* verityCode)
{
	m_verityCode = verityCode;
}

void CVerityCodeDrawer::Draw(HDC dc, const RECT& rect, COLORREF backColor)
{
	const float XELASTICRATE = 0.03f;		//x方向伸缩字符宽度的多少倍。
	const float YELASTICRATE = 0.1f;		//y方向伸缩字符宽度的多少倍。
	const BYTE MINR = 0x00;					//红色最小值。
	const BYTE MAXR = 0x7F;					//红色最大值。
	const BYTE MING = 0x00;					//绿色最小值。
	const BYTE MAXG = 0x7F;					//绿色最大值。
	const BYTE MINB = 0x00;					//蓝色最小值。
	const BYTE MAXB = 0x7F;					//蓝色最大值。
	const int MAXANGLE = 10;				//角度偏移绝对值的最大值。单位为度。
	const char* FONTFACENAME = "";			//字体。
	const int MINLINECOUNT = 2;				//最少几根线。
	const int MAXLINECOUNT = 4;				//最多几根线。
	const int MINLINEWIDTH = 2;				//线最小多细。
	const int MAXLINEWIDTH = 3;				//线最大多粗。
	const float XLINEELASTICRATE = 0.2;		//线在x方向伸缩线平均x方向长度的多少倍。
	const float YLINEELASTICRATE = 0.4;		//线在x方向伸缩线平均y方向长度的多少倍。
	int i;
	int width = 0;
	RECT charRect;
	COLORREF color;
	HBRUSH backBrush = NULL;

	srand((unsigned)time(NULL));

	color = GetRandColor(MINR, MAXR, MING, MAXG, MINB, MAXB);

	if (!m_verityCode.IsEmpty())
	{
		//画背景。
		HBRUSH backBrush = CreateSolidBrush(backColor);
		COLORREF oldBkColor = SetBkColor(dc, backColor);
		FillRect(dc, &rect, backBrush);

		//得到每个字符的宽度。
		width = (rect.right - rect.left) / m_verityCode.GetLength();
		charRect.left = rect.left;
		charRect.right = charRect.left;

		//循环画每个字符。
		for (i=0; i<m_verityCode.GetLength(); i++)
		{
			int xElastic = (int)(width * XELASTICRATE);						//x方向的伸缩量。
			int yElastic = (int)((rect.bottom - rect.top) * YELASTICRATE);	//y方向的伸缩量。

			//得到此字符的大小位置，并在上下左右都进行伸缩。
			charRect.left = charRect.right;
			charRect.right = charRect.left + width;
			charRect.top = rect.top;
			charRect.bottom = rect.bottom;
			if (xElastic != 0)
			{
				charRect.left += rand() % (xElastic * 2) - xElastic;
				charRect.right = charRect.right + (rand() % (xElastic * 2) - xElastic);
			}
			if (yElastic != 0)
			{
				charRect.top += rand() % (yElastic * 2) - yElastic;
				charRect.bottom += (rand() % (yElastic * 2) - yElastic);
			}
			if (charRect.left < rect.left)
				charRect.left = rect.left;
			if (charRect.top < rect.top)
				charRect.top = rect.top;
			if (charRect.right > rect.right)
				charRect.right = rect.right;
			if (charRect.bottom > rect.bottom)
				charRect.bottom = rect.bottom;

			//角度。
			int angle = rand() % MAXANGLE * 2 - MAXANGLE;

			//画。
			DrawChar(dc, charRect, m_verityCode[i], color, angle, FONTFACENAME);
		}

		//画线。
		POINT point;
		int x = rect.left;
		int y = rect.top + (rect.bottom - rect.top) / 2;
		int lineCount = MINLINECOUNT + rand() % (MAXLINECOUNT - MINLINECOUNT + 1);
		int lineLength = (rect.right - rect.left) / lineCount;
		int xLineElastic = (int)(lineLength * XLINEELASTICRATE);
		int yLineElastic = (int)((rect.bottom - rect.top) * YLINEELASTICRATE);
		for (i=0; i<lineCount; i++)
		{
			HPEN pen = CreatePen(PS_SOLID, MINLINEWIDTH + rand() % (MAXLINEWIDTH - MINLINEWIDTH + 1), color);
			HPEN oldPen = (HPEN)SelectObject(dc, pen);

			//得到线的起始点。
			x += rand() % xLineElastic * 2 - xLineElastic;
			if (x < rect.left)
				x = rect.left;
			if (x > rect.right)
				x = rect.right;
			y += rand() % yLineElastic * 2 - yLineElastic;
			MoveToEx(dc, x, y, &point);

			//得到线的结束点。
			x += lineLength;
			x += rand() % xLineElastic * 2 - xLineElastic;
			if (x < rect.left)
				x = rect.left;
			if (x > rect.right)
				x = rect.right;
			y = rect.top + rand() % (rect.bottom - rect.top);
			LineTo(dc, x, y);

			SelectObject(dc, oldPen);
			DeleteObject(pen);
			pen = NULL;
		}

		//释放。
		SetBkColor(dc, oldBkColor);
		DeleteObject(backBrush);
		backBrush = NULL;
	}
}

bool CVerityCodeDrawer::DrawChar(HDC dc, const RECT& rect, char c, COLORREF color, int angle, const char* fontName)
{
	HFONT oldFont = (HFONT)GetCurrentObject(dc, OBJ_FONT);
	LOGFONT logfont;
	HFONT font;
	COLORREF oldColor;

	//创建字体。
	if (!GetObject(oldFont, sizeof(logfont), &logfont))
		return false;
	logfont.lfWidth = rect.right - rect.left;
	logfont.lfHeight = rect.bottom - rect.top;
	logfont.lfEscapement = angle * 10;
	strcpy(logfont.lfFaceName, fontName);
	font = CreateFontIndirect(&logfont);
	oldFont = (HFONT)SelectObject(dc, font);

	//改变颜色。
	oldColor = SetTextColor(dc, color);

	//画。
	TextOut(dc, rect.left, rect.top, &c, 1);

	//释放
	SetTextColor(dc, oldColor);
	SelectObject(dc, oldFont);
	DeleteObject(font);
	font = NULL;

	return true;
}

COLORREF CVerityCodeDrawer::GetRandColor(BYTE minR, BYTE maxR, BYTE minG, BYTE maxG, BYTE minB, BYTE maxB)
{
	return RGB(minR + rand() % (maxR - minR), minG + rand() % (maxG - minG), minB + rand() % (maxB - minB));
}
