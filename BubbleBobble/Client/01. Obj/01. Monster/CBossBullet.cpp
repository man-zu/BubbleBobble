#include "stdafx.h"
#include "CBossBullet.h"
#include "BmpMgr.h"

CBossBullet::CBossBullet()
	:m_eBossType(BBTYPE::BOTTOM)
{
}

CBossBullet::~CBossBullet()
{
    Release();
}

void CBossBullet::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossBullet.bmp",L"BossBullet");
    m_tInfo = {100, 100, 33, 32};
	m_fSpeed = 5.f;

	m_tFrame = { 0, 3, 0, 100, GetTickCount64() };
}

int CBossBullet::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

	if(m_eBossType == BBTYPE::BOTTOM)
		m_tInfo.fY += m_fSpeed;

	else if (m_eBossType == BBTYPE::MOVE)
	{
		/*m_tInfo.fY += m_fSpeed;
		m_tInfo.fX += m_fSpeed;*/

		m_tInfo.fX += 3 * cos(m_fAngle * PI / 180);
		m_tInfo.fY -= 3 * sin(m_fAngle * PI / 180);
	}

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CBossBullet::Late_Update()
{
	Move_Frame();
}

void CBossBullet::Render(HDC hDC)
{
	//HPEN myPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(254, 6, 49));
	//HPEN oldPen = (HPEN)SelectObject(hDC, myPen);
	//HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(254, 6, 49));
	//HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.top);

	//SelectObject(hDC, oldBrush);
	//SelectObject(hDC, oldPen);
	//DeleteObject(myBrush);
	//DeleteObject(myPen);


	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"BossBullet");

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_tFrame.iFrameStart * int(m_tInfo.fCX),
		m_tFrame.iMotion * int(m_tInfo.fCY),
		int(m_tInfo.fCX), // 복사할 비트맵의 가로, 세로 길이
		int(m_tInfo.fCY),
		RGB(15, 79, 174));	// 제거하고자 하는 색상을 전달
}

void CBossBullet::Release()
{
}
