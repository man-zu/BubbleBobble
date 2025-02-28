#include "stdafx.h"
#include "CBMonsterS2.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "CBItem.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

CBMonsterS2::CBMonsterS2() :m_fTime(0.f)
{
	m_tInfo = { 300, 100, 32, 32 };
}

CBMonsterS2::CBMonsterS2(float _x, float _y) :m_fTime(0.f)
{
	m_tInfo = {_x, _y, 32, 32};
}

CBMonsterS2::~CBMonsterS2()
{
	Release();
}

void CBMonsterS2::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BaneBou_Right.bmp", L"BaneBou_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BaneBou_Left.bmp", L"BaneBou_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BaneBou_Death.bmp", L"BaneBou_Death");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BaneBou_Bubble.bmp", L"BaneBou_Bubble");
	m_pFrameKey = L"BaneBou_Left";

	m_fSpeed = 2.f;
	m_fGravity = 2.f;
	m_direction = { 1,1 };

	m_dieDelayTime = 0;
	m_bubbleDelayTime = 0;
	m_bHurryUp = false;

	if(m_tInfo.fX <= WINCX * 0.5)
		m_eDir = DIR_LEFT;
	else
		m_eDir = DIR_RIGHT;
}

int CBMonsterS2::Update()
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
			m_tInfo.fY += m_direction.y * m_fSpeed;

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
			jump();
		}
	}
	// render 부분 고민해야됌
	//return __super::Update();

	__super::Update_InfoY();
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CBMonsterS2::Late_Update()
{
	Move_Frame();
	motion_Change();
}

void CBMonsterS2::Render(HDC hDC)
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

void CBMonsterS2::Release()
{
}

void CBMonsterS2::motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case MONSTERSTATE::WALK_LEFT:
			m_tFrame = { 0, 3, 0, 200, GetTickCount64() };
			m_pFrameKey = L"BaneBou_Left";
			break;

		case MONSTERSTATE::ANGRY_LEFT:
			m_tFrame = { 0, 3, 1, 200, GetTickCount64() };
			m_pFrameKey = L"BaneBou_Left";
			break;

		case MONSTERSTATE::WALK_RIGHT:
			m_tFrame = { 0, 3, 0, 200, GetTickCount64() };
			m_pFrameKey = L"BaneBou_Right";
			break;

		case MONSTERSTATE::ANGRY_RIGHT:
			m_tFrame = { 0, 3, 1, 200, GetTickCount64() };
			m_pFrameKey = L"BaneBou_Right";
			break;

		case MONSTERSTATE::MONSTER_BUBBLE:
			m_tFrame = { 0, 2, 0, 200, GetTickCount64() };
			m_pFrameKey = L"BaneBou_Bubble";
			break;

		case MONSTERSTATE::MONSTER_BUBBLE_HURRY:
			m_tFrame = { 0, 5, 3, 200, GetTickCount64() };
			m_pFrameKey = L"BaneBou_Bubble";
			break;

		case MONSTERSTATE::MONSTER_DIE:
			m_tFrame = { 0, 3, 0, 100, GetTickCount64() };
			m_pFrameKey = L"BaneBou_Death";
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CBMonsterS2::jump()
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

	m_tInfo.fX += m_direction.x * m_fSpeed;


	// 공중에 있을때 중력 작용
	// m_bColTile : tile위에 있는거 말곤 모두 중력작용(true)
	if (Get_ColTile() != COLLSTATE::TOP)
	{
		m_tInfo.fY += m_fGravity;
	}

	if (Get_ColTile() == COLLSTATE::TOP)
	{
		float m_fJumpPower = 15.f;
		bool m_bfall = false;

		m_tInfo.fY -= (m_fJumpPower * m_fTime) - (m_fGravity * m_fTime * m_fTime * 0.5f);

		if ((m_fJumpPower * m_fTime) >= (m_fGravity * m_fTime * m_fTime * 0.5f))
			m_bfall = false;
		else
			m_bfall = true;		// 낙하중

		m_fTime += 0.1f;

		// 낙하하면서 타일에 닿았을때
		if (m_bfall && CTileMgr::Get_Instance()->Collision_Tile(this))
		{
			m_fTime = 0.f;
			//m_bJump = false;
		}
		// 낙하하면서 타일에 닿았을때
		if (Get_ColTile() == COLLSTATE::BOTTOM)
		{
			m_fTime = 0.f;
			//m_bJump = false;
			Set_ColTile(COLLSTATE::NONE);
		}
	}
}

