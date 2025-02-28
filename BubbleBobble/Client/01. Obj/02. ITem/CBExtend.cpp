#include "stdafx.h"
#include "CBExtend.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"

CBExtend::CBExtend() : m_eExtend(EXTENDSTATE::EXTENDEND), m_iFrame_Start(0), m_bPlayerGet(false), m_bGetExtendAll(false)
{
}

CBExtend::CBExtend(EXTENDSTATE _param) : m_iFrame_Start(0), m_bPlayerGet(false), m_bGetExtendAll(false)
{
	m_eExtend = _param;
	m_fSpeed = 0.5f;
	//m_fSpeed = 5.f;
}

CBExtend::~CBExtend()
{
	Release();
}

void CBExtend::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bubble/EXTEND.bmp", L"EXTEND");

    m_tInfo = {0, 0, 30, 30};
	m_direction = {1, -1};
	m_eCurItem = ITEMLIST::EXTEND;
}

int CBExtend::Update()
{
	if (m_bDead)
	{
		if (m_bPlayerGet)
		{
			m_direction = { 0, 0 };
		}
		else
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Collect_Alphabet.wav", ITEM_SOUND_EFFECT, SOUND_VOLUME, true);

			CObj* pObj = new CBExtend(Get_Extend_ID());
			pObj->Initialize();
			pObj->Set_Dead();
			pObj->Set_Direction_Initize();
			dynamic_cast<CBExtend*>(pObj)->Set_bPlayer_Get(true);

			switch (m_eExtend)
			{
			case EXTENDSTATE::E0:
				Set_Pos(33, 300);;
				pObj->Set_Pos(33, 300); break;
			case EXTENDSTATE::X1:
				Set_Pos(33, 330);
				pObj->Set_Pos(33, 330); break;
			case EXTENDSTATE::T2:
				Set_Pos(33, 360);
				pObj->Set_Pos(33, 360); break;
			case EXTENDSTATE::E3:
				Set_Pos(33, 390);
				pObj->Set_Pos(33, 390); break;
			case EXTENDSTATE::N4:
				Set_Pos(33, 420);
				pObj->Set_Pos(33, 420); break;
			case EXTENDSTATE::D5:
				Set_Pos(33, 450);
				pObj->Set_Pos(33, 450); break;
			}

			CSceneMgr::Get_Instance()->Set_Extend_List(pObj);
			m_bPlayerGet = true;
		}
		__super::Update_Rect();
		return OBJ_NOEVENT;
	}

	bool bCol = CTileMgr::Get_Instance()->Collision_Tile_Bubble(this);

	if (m_tInfo.fY >= 500)
	{
		m_tInfo.fY += m_direction.y * m_fSpeed;
	}
	else if (m_tInfo.fY <= CTileMgr::Get_Instance()->Get_First_Tile().fY + TILECY)
	{
		m_direction.y *= -1;
		m_direction.x = 0;
	}
	else if (bCol && (Get_ColTile() == COLLSTATE::LEFT || Get_ColTile() == COLLSTATE::LEFT))
	{
		srand(unsigned int (time(NULL)));
		if (rand() % 2 == 1)
			m_direction.x = 1;
		else
			m_direction.x = -1;

		m_tInfo.fX += m_direction.x * m_fSpeed;
	}
	else
	{
		m_tInfo.fY += m_direction.y * m_fSpeed;
	}

	__super::Update_Rect();
    return OBJ_NOEVENT;
}

void CBExtend::Late_Update()
{
}

void CBExtend::Render(HDC hDC)
{

	if (!m_bGetExtendAll)
	{
		switch (m_eExtend)
		{
		case EXTENDSTATE::E0:
			m_iFrame_Start = 0;
			break;
		case EXTENDSTATE::X1:
			m_iFrame_Start = 1;
			break;
		case EXTENDSTATE::T2:
			m_iFrame_Start = 2;
			break;
		case EXTENDSTATE::E3:
			m_iFrame_Start = 3;
			break;
		case EXTENDSTATE::N4:
			m_iFrame_Start = 4;
			break;
		case EXTENDSTATE::D5:
			m_iFrame_Start = 5;
			break;
		}
	}
	else
	{
		++m_iFrame_Start;
		if (m_iFrame_Start > 6)
			m_iFrame_Start = 0;
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"EXTEND");

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		hMemDC,
		200 * m_iFrame_Start,
		0,
		200, // 복사할 비트맵의 가로, 세로 길이
		224,
		RGB(15, 79, 174));	// 제거하고자 하는 색상을 전달

}

void CBExtend::Release()
{
}
