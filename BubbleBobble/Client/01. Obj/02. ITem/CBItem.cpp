#include "stdafx.h"
#include "CBItem.h"
#include "BmpMgr.h"
#include "time.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "CBScoreTexture.h"
#include "CBPlayer.h"
#include "SoundMgr.h"

CBItem::CBItem()
	: m_eCurItem(ITEMLIST::BANANA), m_bInTile(true)
{
}

CBItem::~CBItem()
{
	Release();
}

void CBItem::Initialize()
{

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Fruit.bmp",L"Fruit.bmp");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/BigFruit.bmp",L"BigFruit.bmp");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/CandyShoe.bmp",L"CandyShoe.bmp");

	m_pFrameKey = L"Fruit.bmp";
    m_tInfo = {100, 100, 32, 32};

	m_fGravity = 3.f;

	srand(unsigned(time(NULL)));
	item_Choose(rand() % 5);
}

int CBItem::Update()
{
    if (m_bDead)
	{
		if (m_bInTile)
		{
		if (m_eCurItem == ITEMLIST::BIGCAKE)
			CSoundMgr::Get_Instance()->PlaySound(L"BigFood.wav", ITEM_SOUND_EFFECT, SOUND_VOLUME, true);
		else
			CSoundMgr::Get_Instance()->PlaySound(L"Collect_Fruit.wav", ITEM_SOUND_EFFECT, SOUND_VOLUME, true);

		// shoe
		if (m_eCurItem == ITEMLIST::SHOE)
		{
			list<CObj*> pPlayer = CObjMgr::Get_Instance()->Get_Obj(PLAYER);
			for (auto player : pPlayer)
			{
				player->Set_SpeedUp();
			}
		}
			//ITEMSCORE
			CObj* pTexture = new CBScoreTexture(Get_Score());
			pTexture->Initialize();
			pTexture->Set_Pos(m_tInfo.fX, m_tInfo.fY);
			CObjMgr::Get_Instance()->Add_Object(ITEMSCORE, pTexture);
		}
		return OBJ_DEAD;
	}

	if (m_eCurItem == ITEMLIST::BIGCAKE)
	{
		float y = CTileMgr::Get_Instance()->Get_Right_Bottom_Tile().fY;
		if (m_tInfo.fY <= CTileMgr::Get_Instance()->Get_Right_Bottom_Tile().fY - 80)
			m_tInfo.fY += float(m_fGravity * 0.5);
	}
	else if (m_eCurItem == ITEMLIST::SHOE)
	{
	}
	else if (CTileMgr::Get_Instance()->Collision_Tile(this) || Get_ColTile() != COLLSTATE::TOP)
		m_tInfo.fY += m_fGravity;


	if (m_eCurItem == ITEMLIST::BEER)
	{
		// 하단 타일 넘어가면 삭제
		if (Get_Info().fY >= WINCY - 70)
		{
			m_bDead = true;
			m_bInTile = false;
		}
	}

	//__super::Update_InfoY();
	__super::Update_Rect();
    return OBJ_NOEVENT;
}

void CBItem::Late_Update()
{
}

void CBItem::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);
	
	if (m_eCurItem == ITEMLIST::SHOE)
	{
		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_tFrame.iFrameStart * 40,
			m_tFrame.iMotion * 40,
			40, // 복사할 비트맵의 가로, 세로 길이
			40,
			RGB(15, 79, 174));	// 제거하고자 하는 색상을 전달
	}
	else if (m_eCurItem != ITEMLIST::BIGCAKE)
	{
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
			RGB(15, 79, 174));	// 제거하고자 하는 색상을 전달
	}
	else
	{
		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_tFrame.iFrameStart,
			m_tFrame.iMotion,
			88, // 복사할 비트맵의 가로, 세로 길이
			88,
			RGB(15, 79, 174));	// 제거하고자 하는 색상을 전달
	}
}

void CBItem::Release()
{
}


void CBItem::item_Choose(int _param)
{
	switch (_param)
	{
	case 0:
		m_tFrame = { 0, 0, 0, 200, GetTickCount64() };
		m_eCurItem = ITEMLIST::BANANA;
		m_iScore = 100;
		break;

	case 1:
		m_tFrame = { 1, 1, 0, 200, GetTickCount64() };
		m_eCurItem = ITEMLIST::ORANGE;
		m_iScore = 300;
		break;

	case 2:
		m_tFrame = { 2, 2, 0, 200, GetTickCount64() };
		m_eCurItem = ITEMLIST::PEACH;
		m_iScore = 500;
		break;

	case 3:
		m_tFrame = { 3, 3, 0, 200, GetTickCount64() };
		m_eCurItem = ITEMLIST::WATERMELON;
		m_iScore = 500;
		break;

	case 4:
		m_tFrame = { 4, 4, 0, 200, GetTickCount64() };
		m_eCurItem = ITEMLIST::GRAPE;
		m_iScore = 700;
		break;

	case 5:
		m_tFrame = { 0, 0, 0, 200, GetTickCount64() };
		m_eCurItem = ITEMLIST::BIGCAKE;
		m_pFrameKey = L"BigFruit.bmp";
		m_iScore = 7000;
		m_tInfo.fCX = 150;
		m_tInfo.fCY = 150;
		break;
	
	case 6:
		m_tFrame = { 3, 3, 0, 200, GetTickCount64() };
		m_eCurItem = ITEMLIST::SHOE;
		m_pFrameKey = L"CandyShoe.bmp";
		m_iScore = 0;
		break;
	
	case 7:
		m_tFrame = { 2, 2, 1, 200, GetTickCount64() };
		m_eCurItem = ITEMLIST::BEER;
		m_iScore = 3000;
		m_fGravity *= 2;
		break;
	}
}