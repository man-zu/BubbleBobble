#include "stdafx.h"
#include "CFireBubble.h"
#include "BmpMgr.h"
#include "TileMgr.h"

CFireBubble::CFireBubble()
	:m_epPreState(FIRESTATE::CREATE), m_eCurState(FIRESTATE::CREATE), m_fAtt(50.f)
{
}

CFireBubble::~CFireBubble()
{
}

void CFireBubble::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bubble/fire.bmp", L"fire");

	m_tInfo.fCX = 40;
	m_tInfo.fCY = 40;
	m_fSpeed = 3.f;

	m_pFrameKey = L"fire";
	m_eCurState = FIRESTATE::CREATE;

	m_direction = { 1, -1 };
}

int CFireBubble::Update()
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	// 하단 타일 넘어가면 삭제
	if (Get_Info().fY >= WINCY - 70)  /*Get_Info().fX <= 10 || Get_Info().fX >= WINCX || Get_Info().fY <= 10 ||*/
	{
		m_bDead = true;
	}


	switch (m_eCurState)
	{
	case FIRESTATE::CREATE:
		m_fSpeed = 2.f; break;
	case FIRESTATE::POP:
		m_fSpeed = 4.f; break;
	case FIRESTATE::FIRE:
		m_fSpeed = 0.f; break;
	}

	//if (m_eCurState == FIRESTATE::POP)
	//{
	//	CTileMgr::Get_Instance()->Collision_Tile(this);

	//	COLLSTATE tmp = Get_ColTile();

	//	// 타일충돌은 되는데 땅에 닿았을때 상태바꿔주기가 안됌
	//	if(CTileMgr::Get_Instance()->Collision_Tile(this) && Get_ColTile() == COLLSTATE::TOP);
	//	{
	//		m_eCurState = FIRESTATE::FIRE;
	//	}
	//}

	m_tInfo.fY += m_fSpeed;

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CFireBubble::Late_Update()
{
}

void CFireBubble::Render(HDC hDC)
{
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

	switch (m_eCurState)
	{
	case FIRESTATE::CREATE:
		m_tFrame.iFrameStart = 0; break;
	case FIRESTATE::POP:
		m_tFrame.iFrameStart = 1; break;
	case FIRESTATE::FIRE:
		m_tFrame.iFrameStart = 2; break;
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0,
		m_tFrame.iFrameStart * 75,
		70, // 복사할 비트맵의 가로, 세로 길이
		75,
		RGB(15, 79, 174));	// 제거하고자 하는 색상을 전달

	//case FIRESTATE::FIRE:
	// 불일때 이미지 좌우 여러장띄우고 fcx 가로로 길게 해주면 될 듯
}

void CFireBubble::Release()
{
}
