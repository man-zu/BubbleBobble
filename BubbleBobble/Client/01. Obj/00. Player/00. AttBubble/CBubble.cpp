#include "stdafx.h"
#include "CBubble.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"

CBubble::CBubble()
    :m_fMoveX(0), m_eCurState(BUBBLESTATE::END), m_ePreState(BUBBLESTATE::END), m_uDeadDelayTime(0)
	,m_bColMonster(false), m_bTileCol(false), m_bTileCol2(false)
{
}

CBubble::~CBubble()
{
    Release();
}

void CBubble::Initialize()
{

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bubble/Bullet.bmp", L"Bullet");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bubble/PlayerBubble.bmp", L"PlayerBubble");
    
    m_tInfo = {30, 30, 30, 30};
    m_fSpeed = 4.f;
    m_direction = { 1, -1 };

	m_eCurState = BUBBLESTATE::CREATE;
	m_uDeadDelayTime = GetTickCount64();

	m_bColTile = false;

}

int CBubble::Update()
{
	if (m_bDead)
	{
		if (m_uDeadDelayTime + 800 < GetTickCount64())
			return OBJ_DEAD;
		return OBJ_NOEVENT;
	}

	if (m_bColMonster)
		return OBJ_DEAD;

	CTileMgr::Get_Instance()->Collision_Tile_Bubble(this);


	if (m_tInfo.fY <= (CTileMgr::Get_Instance()->Get_First_Tile().fY + TILECY))
		m_direction.y = 0;
    else if (!m_bColTile && m_eDir == DIR_RIGHT && m_tInfo.fX <= m_fMoveX)
        m_tInfo.fX += m_direction.x * m_fSpeed;
    else if (!m_bColTile && m_eDir == DIR_LEFT && m_tInfo.fX >= m_fMoveX)
        m_tInfo.fX += m_direction.x * m_fSpeed;
	else if (CTileMgr::Get_Instance()->Collision_Tile(this) && (Get_ColTile() == COLLSTATE::BOTTOM))
	{
		if (!m_bTileCol2 && (Get_Col2Tile() == COLLSTATE::RIGHT || Get_Col2Tile() == COLLSTATE::LEFT))
		{
			m_direction.x;
			m_direction.x *= -1;
			m_bTileCol2 = true;
		}
		m_direction.y = 0;
		srand(unsigned int(time(NULL)));

		if (!m_bTileCol || 0 == m_direction.x)
		{
			if (rand() % 2 == 1)
				m_direction.x = -1;
			else
				m_direction.x = 1;
		}

		/*if (0 != m_direction.x)
		{
		}*/
		m_tInfo.fX += m_direction.x * m_fSpeed * 0.2f;
		m_bTileCol = true;
	}
	else
	{
		m_direction = { 0,-1 };	// tile 충돌했으면 x,y 바뀌어 있어서 초기화

		// y 축으로 올라감
		m_tInfo.fY += (float)(m_direction.y * m_fSpeed * 0.2);
		m_eCurState = BUBBLESTATE::IDLE;
	}
	if (m_uDeadDelayTime + 10000 < GetTickCount64())
		m_eCurState = BUBBLESTATE::RED;

	if (m_uDeadDelayTime + 12000 < GetTickCount64())
		Set_BubblePop();


	__super::Update_InfoY();
    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CBubble::Late_Update()
{
	Move_Frame();
	Motion_Change();
}

void CBubble::Render(HDC hDC)
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

void CBubble::Release()
{
}

void CBubble::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case BUBBLESTATE::CREATE:
			m_tFrame = { 0, 5, 0, 100, GetTickCount64() };
			m_pFrameKey = L"Bullet";
			break;

		case BUBBLESTATE::IDLE:
			m_tFrame = { 0, 1, 0, 200, GetTickCount64() };
			m_pFrameKey = L"PlayerBubble";
			break;

		case BUBBLESTATE::RED:
			m_tFrame = { 0, 5, 3, 200, GetTickCount64() };
			m_pFrameKey = L"PlayerBubble";
			break;

		case BUBBLESTATE::POP:
			m_tFrame = { 0, 5, 4, 200, GetTickCount64() };
			m_pFrameKey = L"PlayerBubble";
			break;
		}
		m_ePreState = m_eCurState;
	}
}

