#include "stdafx.h"
#include "Line.h"
#include "ScrollMgr.h"

CLine::CLine()
{
}

CLine::CLine(LINEPOINT & tLeft, LINEPOINT & tRight)
	: m_tInfo(tLeft, tRight)
{
}

CLine::CLine(LINE& tInfo)
	: m_tInfo(tInfo)
{
}


CLine::~CLine()
{
}

void CLine::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	MoveToEx(hDC, (int)m_tInfo.tLeft.fX + iScrollX, (int)m_tInfo.tLeft.fY, nullptr);
	LineTo(hDC, (int)m_tInfo.tRight.fX + iScrollX, (int)m_tInfo.tRight.fY);
}
