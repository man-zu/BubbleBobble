#include "stdafx.h"
#include "CElectricalBubble.h"
#include "BmpMgr.h"
#include "TileMgr.h"

CElectricalBubble::CElectricalBubble()
	: m_eCurState(ELECTBUBBLESTATE::END), m_ePreState(ELECTBUBBLESTATE::END), m_bPop(false), m_fMoveX(0)
	, m_fAtt(30.f)
{
}

CElectricalBubble::~CElectricalBubble()
{
}

void CElectricalBubble::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bubble/Lightning_Skill.bmp", L"Lightning_Skill");

	m_tInfo = {0, 0, 44, 44};
	m_fSpeed = 5.f;

	m_pFrameKey = L"Lightning_Bubble";
	m_eCurState = ELECTBUBBLESTATE::BUBBLE;

	m_direction = {1, -1};
}

int CElectricalBubble::Update()
{
	if (m_tInfo.fY <= (CTileMgr::Get_Instance()->Get_First_Tile().fY + TILECY))
		m_bDead = true;

    if (m_bDead)
        return OBJ_DEAD;

	if (m_bPop)
	{
		m_eCurState = ELECTBUBBLESTATE::BUBBLEPOP;
	}

	CTileMgr::Get_Instance()->Collision_Tile_Bubble(this);

	if (m_eCurState == ELECTBUBBLESTATE::BUBBLEPOP)
	{
		if (m_eDir == DIR_RIGHT)
			m_direction.x = -1;
		else if (m_eDir == DIR_LEFT)
			m_direction.x = 1;
		m_tInfo.fX += (m_direction.x) * m_fSpeed;
		m_tInfo.fY += m_direction.y * m_fSpeed;
	}
	else if (Get_ColTile() == COLLSTATE::LEFT || Get_ColTile() == COLLSTATE::RIGHT)
	{
		// y 축으로 올라감
		m_direction = { 0,0 };
		m_tInfo.fY += -0.2f * m_fSpeed;
	}
	else if (m_bColTile && m_eDir == DIR_RIGHT /*&& m_tInfo.fX <= m_fMoveX*/)
	{
		m_direction = { 1, -1 };
		m_tInfo.fX += m_direction.x * m_fSpeed;
	}
	else if (m_bColTile && m_eDir == DIR_LEFT /*&& m_tInfo.fX >= m_fMoveX*/)
	{
		m_direction = { -1, -1 };
		m_tInfo.fX += m_direction.x * m_fSpeed;
	}
	else if (/*CTileMgr::Get_Instance()->Collision_Tile(this) &&*/ (Get_ColTile() == COLLSTATE::BOTTOM))
	{
		m_direction.y = 0;
		srand(unsigned int(time(NULL)));

		if (!m_direction.x)
		{
			if (rand() % 2 == 1)
				m_direction.x = -1;
			else
				m_direction.x = 1;
		}

		m_tInfo.fX += m_direction.x * m_fSpeed * 0.2f;
	}
	

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CElectricalBubble::Late_Update()
{
	Move_Frame();
	Motion_Change();
}

void CElectricalBubble::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_tFrame.iFrameStart * 44,
		m_tFrame.iMotion * 44,
		44, // 복사할 비트맵의 가로, 세로 길이
		44,
		RGB(95, 88, 88));	// 제거하고자 하는 색상을 전달
}

void CElectricalBubble::Release()
{
}


void CElectricalBubble::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case ELECTBUBBLESTATE::BUBBLE:
			m_tFrame = { 0, 2, 0, 100, GetTickCount64() };
			m_pFrameKey = L"Lightning_Bubble";
			break;

		case ELECTBUBBLESTATE::BUBBLEPOP:
			m_tFrame = { 0, 2, 0, 100, GetTickCount64() };
			m_pFrameKey = L"Lightning_Skill";
			break;

		}
	}
}