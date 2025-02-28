#include "stdafx.h"
#include "CStageExtend.h"
#include "AbstractFactory.h"
#include "CBPlayer.h"
#include "ObjMgr.h"
#include "CBItem.h"
#include "BmpMgr.h"
#include "SoundMgr.h"
#include "CBText.h"
#include "CScoreMgr.h"

CStageExtend::CStageExtend()
	: m_player(nullptr), m_DelayTime(0), m_DelayBool(false), m_EndTime(0), m_SceneDelayBool(false), m_ScreneDelayTime(0)
{
}

CStageExtend::~CStageExtend()
{
	Release();
}

void CStageExtend::Initialize()
{
	m_player = CAbstractFactory<CBPlayer>::Create();
	m_player->Set_Init_Life(CSceneMgr::Get_Instance()->Get_Player_Life());
	m_player->Set_Init_Score(CScoreMgr::Get_Instance()->Get_Scene_Score());
	CObjMgr::Get_Instance()->Add_Object(PLAYER, m_player);
	
	// 신발 아이템
	CObj* pItem = new CBItem();
	pItem->Initialize();
	dynamic_cast<CBItem*>(pItem)->item_Choose(6);
	pItem->Set_Pos(400, 450);
	CObjMgr::Get_Instance()->Add_Object(ITEM, pItem);

	// 점수판
	CObj* pText = CAbstractFactory<CBText>::Create();
	dynamic_cast<CBText*>(pText)->Set_Type(TEXTTYPE::UP);
	CObjMgr::Get_Instance()->Add_Object(TEXT, pText);

	pText = CAbstractFactory<CBText>::Create();
	dynamic_cast<CBText*>(pText)->Set_Type(TEXTTYPE::HIGHSCORE);
	CObjMgr::Get_Instance()->Add_Object(TEXT, pText);

	m_pCurScore = CAbstractFactory<CBText>::Create();
	dynamic_cast<CBText*>(m_pCurScore)->Set_Type(TEXTTYPE::NUM);
	m_pCurScore->Set_Pos(300, 33);
	CObjMgr::Get_Instance()->Add_Object(TEXT, m_pCurScore);

	m_pHighScore = CAbstractFactory<CBText>::Create();
	dynamic_cast<CBText*>(m_pHighScore)->Set_Type(TEXTTYPE::NUM);
	m_pHighScore->Set_Pos(650, 33);
	CObjMgr::Get_Instance()->Add_Object(TEXT, m_pHighScore);

	CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Extend.dat"); 

	m_EndTime = GetTickCount64();
}

void CStageExtend::Update()
{
	// 맥주 떨어지는 DelayTime
	if (!m_DelayTime)
		m_DelayTime = GetTickCount64();

	if (m_DelayBool && m_DelayTime + 600 < GetTickCount64())
	{
		m_DelayTime = 0;
		m_DelayBool = false;
	}

	if (!m_DelayBool)
	{
		int tmp = rand() % 10;

		CObj* pItem = new CBItem();
		pItem->Initialize();
		dynamic_cast<CBItem*>(pItem)->item_Choose(7);
		pItem->Set_Pos(100.f + 70.f * tmp, 240.f);
		CObjMgr::Get_Instance()->Add_Object(ITEM, pItem);

		m_DelayBool = true;
	}
	else
	{
		// 매 화면마다 Score Uodate 처리
		CScoreMgr::Get_Instance()->Set_High_Score(m_player->Get_Score());
		CScoreMgr::Get_Instance()->Set_Scene_Score(m_player->Get_Score());
		m_pCurScore->Set_Init_Score(CScoreMgr::Get_Instance()->Get_Scene_Score());
		m_pHighScore->Set_Init_Score(CScoreMgr::Get_Instance()->Get_High_Score());
	}

	CObjMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();
}

void CStageExtend::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	CTileMgr::Get_Instance()->Late_Update();

	if (m_EndTime + 25000 < GetTickCount64())
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_BOSSBEFORE);
	}
	else
	{
		//m_pCurScore->Set_Init_Score(m_player->Get_Score());
		//m_pHighScore->Set_Init_Score(m_player->Get_Score());
	}
}

void CStageExtend::Render(HDC hDC)
{
	CTileMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);

	// 목숨
	HDC hLife = CBmpMgr::Get_Instance()->Find_Img(L"Life");
	for (int i = 0; i < m_player->Get_Life(); ++i)
		GdiTransparentBlt(hDC, 0, 556 - (22 * i), 22, 22, hLife, 0, 0, 22, 22, RGB(15, 79, 174));	// 578 : 600 - 22(life 이미지 사이즈)


	if (m_EndTime + 22000 < GetTickCount64())
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSoundMgr::Get_Instance()->PlaySound(L"Jusin.wav", SOUND_EFFECT, SOUND_VOLUME, false);

		if (!m_ScreneDelayTime)
			m_ScreneDelayTime = GetTickCount64();

		if (m_ScreneDelayTime && m_ScreneDelayTime + 200 < GetTickCount64())
		{
			if (m_SceneDelayBool)
				m_SceneDelayBool = false;
			else
				m_SceneDelayBool = true;

			m_ScreneDelayTime = 0;
		}

		if (m_SceneDelayBool)
		{
			HDC hBlckBg = CBmpMgr::Get_Instance()->Find_Img(L"Ground_2");
			BitBlt(hDC, 0, 0, 800, 600, hBlckBg, 0, 0, SRCCOPY);
		}
	}
}

void CStageExtend::Release()
{
	// [ Next Scene 공유 ] life, score Set
	CSceneMgr::Get_Instance()->Set_Player_Life(m_player->Get_Life());
	CScoreMgr::Get_Instance()->Set_Scene_Score(m_player->Get_Score());
	CScoreMgr::Get_Instance()->Set_High_Score(m_player->Get_Score());

	CTileMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release();
}
