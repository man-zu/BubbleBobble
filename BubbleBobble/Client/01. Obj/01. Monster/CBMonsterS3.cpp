#include "stdafx.h"
#include "CBMonsterS3.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "CBItem.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

CBMonsterS3::CBMonsterS3():m_fYDelayTime(0)
{
}

CBMonsterS3::~CBMonsterS3()
{
	Release();
}

void CBMonsterS3::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Monsta_Right.bmp", L"Monsta_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Monsta_Left.bmp", L"Monsta_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Monsta_Death.bmp", L"Monsta_Death");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Monsta_Bubble.bmp", L"Monsta_Bubble");
	m_pFrameKey = L"Monsta_Left";

	m_tInfo = { 300, 100, 32, 32 };
	m_fSpeed = 2.f;
	//m_fGravity = 4.6f;
	m_direction = { 1,1 };

	m_dieDelayTime = 0;
	m_bubbleDelayTime = 0;
	m_bHurryUp = false;

	m_eDir = DIR_LEFT;
}

int CBMonsterS3::Update()
{
	if (0 >= m_fHp)
		m_bDead = true;

	if (m_bDead)
	{
		if (m_eCurState == MONSTERSTATE::MONSTER_BUBBLE || m_eCurState == MONSTERSTATE::MONSTER_BUBBLE_HURRY ||
			(0 >= m_fHp && m_eCurState != MONSTERSTATE::MONSTER_DIE))	// 번개 닿았을때
		{
			m_eCurState = MONSTERSTATE::MONSTER_DIE;
			if (!m_dieDelayTime)
				m_dieDelayTime = GetTickCount64();

		}
		else if (m_eCurState == MONSTERSTATE::MONSTER_DIE)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"OnePop.wav", SOUND_EFFECT, SOUND_VOLUME, false);
			// 죽었을때 벽에 부딫히기
			//bool colTile = CTileMgr::Get_Instance()->Collision_Tile_Monster(this);

			m_tInfo.fX += m_direction.x * m_fSpeed;
			m_tInfo.fY += (-1) * m_fSpeed;
			//m_tInfo.fY += m_direction.y * m_fSpeed;

			if (m_dieDelayTime + 1000 < GetTickCount64())
			{
				CObjMgr::Get_Instance()->Add_Object(ITEM, CAbstractFactory<CBItem>::Create(m_tInfo.fX, m_tInfo.fY));
				return OBJ_DEAD;
			}
		}
	}
	else
	{
		if (m_eCurState == MONSTERSTATE::MONSTER_BUBBLE)
		{
			if (m_tInfo.fY >= (CTileMgr::Get_Instance()->Get_First_Tile().fY + TILECY))
				m_tInfo.fY -= (float)(m_direction.y * 4.f * 0.1);

			if (m_bubbleDelayTime + 7000 <= GetTickCount64())
			{
				m_eCurState = MONSTERSTATE::MONSTER_BUBBLE_HURRY;
				m_bubbleDelayTime = GetTickCount64();
			}
		}
		else if (m_eCurState == MONSTERSTATE::MONSTER_BUBBLE_HURRY)
		{
			if (m_bubbleDelayTime + 7000 <= GetTickCount64())
			{
				if (m_eDir == DIR_LEFT)
				{
					if (m_bHurryUp)
						m_eCurState = MONSTERSTATE::ANGRY_LEFT;
					else
						m_eCurState = MONSTERSTATE::WALK_LEFT;
					m_direction.x = -1;
				}
				if (m_eDir == DIR_RIGHT)
				{
					if (m_bHurryUp)
						m_eCurState = MONSTERSTATE::ANGRY_RIGHT;
					else
						m_eCurState = MONSTERSTATE::WALK_RIGHT;
					m_direction.x = 1;
				}
				//if (m_eDir == DIR_LEFT)
				//{
				//	m_eCurState = MONSTERSTATE::WALK_LEFT;
				//	m_direction.x = -1;
				//}
				//if (m_eDir == DIR_RIGHT)
				//{
				//	m_eCurState = MONSTERSTATE::WALK_RIGHT;
				//	m_direction.x = 1;
				//}
			}
		}
		else
		{
			bool colTile = CTileMgr::Get_Instance()->Collision_Tile_Monster(this);

			if (m_eDir == DIR_LEFT)
			{
				if (m_bHurryUp)
					m_eCurState = MONSTERSTATE::ANGRY_LEFT;
				else
					m_eCurState = MONSTERSTATE::WALK_LEFT;
				m_direction.x = -1;
			}
			if (m_eDir == DIR_RIGHT)
			{
				if (m_bHurryUp)
					m_eCurState = MONSTERSTATE::ANGRY_RIGHT;
				else
					m_eCurState = MONSTERSTATE::WALK_RIGHT;
				m_direction.x = 1;
			}
			//if (m_eDir == DIR_LEFT)
			//{
			//	m_eCurState = MONSTERSTATE::WALK_LEFT;
			//	m_direction.x = -1;
			//}
			//if (m_eDir == DIR_RIGHT)
			//{
			//	m_eCurState = MONSTERSTATE::WALK_RIGHT;
			//	m_direction.x = 1;
			//}

			if (colTile && m_fYDelayTime + 300 < GetTickCount64())
			{
				m_direction.y *= -1;
				m_fYDelayTime = GetTickCount64();
			}

			m_tInfo.fX += m_direction.x * m_fSpeed;
			m_tInfo.fY += m_direction.y * m_fSpeed;
		}
	}
	// render 부분 고민해야됌
	//return __super::Update();

	__super::Update_InfoY();
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CBMonsterS3::Late_Update()
{
	Move_Frame();
	motion_Change();
}

void CBMonsterS3::Render(HDC hDC)
{
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

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

	__super::Render(hDC);
}

void CBMonsterS3::Release()
{
}

void CBMonsterS3::motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case MONSTERSTATE::WALK_LEFT:
			m_tFrame = { 0, 1, 0, 200, GetTickCount64() };
			m_pFrameKey = L"Monsta_Left";
			break;

		case MONSTERSTATE::ANGRY_LEFT:
			m_tFrame = { 0, 1, 1, 200, GetTickCount64() };
			m_pFrameKey = L"Monsta_Left";
			break;

		case MONSTERSTATE::WALK_RIGHT:
			m_tFrame = { 0, 1, 0, 200, GetTickCount64() };
			m_pFrameKey = L"Monsta_Right";
			break;

		case MONSTERSTATE::ANGRY_RIGHT:
			m_tFrame = { 0, 1, 1, 200, GetTickCount64() };
			m_pFrameKey = L"Monsta_Right";
			break;

		case MONSTERSTATE::MONSTER_BUBBLE:
			m_tFrame = { 0, 2, 0, 200, GetTickCount64() };
			m_pFrameKey = L"Monsta_Bubble";
			break;

		case MONSTERSTATE::MONSTER_BUBBLE_HURRY:
			m_tFrame = { 0, 5, 3, 200, GetTickCount64() };
			m_pFrameKey = L"Monsta_Bubble";
			break;

		case MONSTERSTATE::MONSTER_DIE:
			m_tFrame = { 0, 3, 0, 100, GetTickCount64() };
			m_pFrameKey = L"Monsta_Death";
			break;
		}
		m_ePreState = m_eCurState;
	}
}
