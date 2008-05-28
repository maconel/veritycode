
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
	const float XELASTICRATE = 0.03f;		//x���������ַ���ȵĶ��ٱ���
	const float YELASTICRATE = 0.1f;		//y���������ַ���ȵĶ��ٱ���
	const BYTE MINR = 0x00;					//��ɫ��Сֵ��
	const BYTE MAXR = 0x7F;					//��ɫ���ֵ��
	const BYTE MING = 0x00;					//��ɫ��Сֵ��
	const BYTE MAXG = 0x7F;					//��ɫ���ֵ��
	const BYTE MINB = 0x00;					//��ɫ��Сֵ��
	const BYTE MAXB = 0x7F;					//��ɫ���ֵ��
	const int MAXANGLE = 10;				//�Ƕ�ƫ�ƾ���ֵ�����ֵ����λΪ�ȡ�
	const char* FONTFACENAME = "";			//���塣
	const int MINLINECOUNT = 2;				//���ټ����ߡ�
	const int MAXLINECOUNT = 4;				//��༸���ߡ�
	const int MINLINEWIDTH = 2;				//����С��ϸ��
	const int MAXLINEWIDTH = 3;				//������֡�
	const float XLINEELASTICRATE = 0.2;		//����x����������ƽ��x���򳤶ȵĶ��ٱ���
	const float YLINEELASTICRATE = 0.4;		//����x����������ƽ��y���򳤶ȵĶ��ٱ���
	int i;
	int width = 0;
	RECT charRect;
	COLORREF color;
	HBRUSH backBrush = NULL;

	srand((unsigned)time(NULL));

	color = GetRandColor(MINR, MAXR, MING, MAXG, MINB, MAXB);

	if (!m_verityCode.IsEmpty())
	{
		//��������
		HBRUSH backBrush = CreateSolidBrush(backColor);
		COLORREF oldBkColor = SetBkColor(dc, backColor);
		FillRect(dc, &rect, backBrush);

		//�õ�ÿ���ַ��Ŀ�ȡ�
		width = (rect.right - rect.left) / m_verityCode.GetLength();
		charRect.left = rect.left;
		charRect.right = charRect.left;

		//ѭ����ÿ���ַ���
		for (i=0; i<m_verityCode.GetLength(); i++)
		{
			int xElastic = (int)(width * XELASTICRATE);						//x�������������
			int yElastic = (int)((rect.bottom - rect.top) * YELASTICRATE);	//y�������������

			//�õ����ַ��Ĵ�Сλ�ã������������Ҷ�����������
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

			//�Ƕȡ�
			int angle = rand() % MAXANGLE * 2 - MAXANGLE;

			//����
			DrawChar(dc, charRect, m_verityCode[i], color, angle, FONTFACENAME);
		}

		//���ߡ�
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

			//�õ��ߵ���ʼ�㡣
			x += rand() % xLineElastic * 2 - xLineElastic;
			if (x < rect.left)
				x = rect.left;
			if (x > rect.right)
				x = rect.right;
			y += rand() % yLineElastic * 2 - yLineElastic;
			MoveToEx(dc, x, y, &point);

			//�õ��ߵĽ����㡣
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

		//�ͷš�
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

	//�������塣
	if (!GetObject(oldFont, sizeof(logfont), &logfont))
		return false;
	logfont.lfWidth = rect.right - rect.left;
	logfont.lfHeight = rect.bottom - rect.top;
	logfont.lfEscapement = angle * 10;
	strcpy(logfont.lfFaceName, fontName);
	font = CreateFontIndirect(&logfont);
	oldFont = (HFONT)SelectObject(dc, font);

	//�ı���ɫ��
	oldColor = SetTextColor(dc, color);

	//����
	TextOut(dc, rect.left, rect.top, &c, 1);

	//�ͷ�
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
