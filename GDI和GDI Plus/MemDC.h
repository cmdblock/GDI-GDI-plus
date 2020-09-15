#pragma once
class CMemDC
{
public:
	CMemDC(HDC hdc, const RECT& rect);
	virtual ~CMemDC();

	operator HDC()
	{
		return m_hMemDc;
	}
private:
	HDC m_hdc, m_hMemDc;
	RECT m_rect;
	HBITMAP m_hBitmap, m_hOldBitmap;
};