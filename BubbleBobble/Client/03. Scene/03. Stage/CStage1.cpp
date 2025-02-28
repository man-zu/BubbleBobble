#include "stdafx.h"
#include "CStage1.h"
#include "LineMgr.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "CollisionMgr.h"
#include "CBPlayer.h"
#include "CBMonsterS1.h"
#include "SceneMgr.h"
#include "CBText.h"
#include "CBItem.h"
#include "CBExtend.h"
#include "SoundMgr.h"
#include "CScoreMgr.h"

CStage1::CStage1() : m_player(nullptr)
{
}

CStage1::~CStage1()
{
	Release();
}

void CStage1::Initialize()
{
	CSoundMgr::Get_Instance()->PlayBGM(L"Main_Theme.wav", SOUND_VOLUME);

	m_sceneChDelayTime = 0;
	m_HurryUpTime = GetTickCount64();

	//CObj 객체들 생성
	m_player = CAbstractFactory<CBPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(PLAYER, m_player);

	for (int i = 0; i < 5; ++i)
		CObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<CBMonsterS1>::Create(200.f + (50.f * i), 150.f));


	// 신발 아이템
	CObj* pItem = new CBItem();
	pItem->Initialize();
	dynamic_cast<CBItem*>(pItem)->item_Choose(6);
	pItem->Set_Pos(300, 300);
	CObjMgr::Get_Instance()->Add_Object(ITEM, pItem);

	// EXTEND
	CObj* pObj = new CBExtend(EXTENDSTATE::E0);
	pObj->Initialize();
	pObj->Set_Pos(300, 550);
	CObjMgr::Get_Instance()->Add_Object(EXTEND, pObj);

	pObj = new CBExtend(EXTENDSTATE::N4);
	pObj->Initialize();
	pObj->Set_Pos(500, 550);
	CObjMgr::Get_Instance()->Add_Object(EXTEND, pObj);

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

	// TILE
	CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Stage1.dat");
}

void CStage1::Update()
{
	CObjMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();
}

void CStage1::Late_Update()
{

	CObjMgr::Get_Instance()->Late_Update();
	CTileMgr::Get_Instance()->Late_Update();

	// HurryUp
	if (!m_bHurryCheck && m_HurryUpTime + 10000 < GetTickCount64())
	{
		list<CObj*> pMonsterList = CObjMgr::Get_Instance()->Get_Obj(MONSTER);
		for (auto pMonster : pMonsterList)
		{
			dynamic_cast<CBMonster*>(pMonster)->Set_Hurry();
		}
		m_bHurryCheck = true;
	}

	// 몬스터 잡고 scene변경 딜레이
	if (m_sceneChDelayTime && m_sceneChDelayTime + 6000 < GetTickCount64())
		m_bSceneChangeDelay = true;
	else if (!m_sceneChDelayTime && !CObjMgr::Get_Instance()->Get_Obj(MONSTER).size())
		m_sceneChDelayTime = GetTickCount64();

	// 플레이어 생명 == 0 ? GAMEOVER 화면이동
	if (!m_player->Get_Life())
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_GAMEOVER);
		return;
	}

	// 몬스터 다 잡고 시간 좀 흘렀다면 -> STAGE2 화면이동
	else if (m_bSceneChangeDelay)
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE2);
	}
	else
	{
		// 매 화면마다 Score Uodate 처리
		CScoreMgr::Get_Instance()->Set_High_Score(m_player->Get_Score());
		CScoreMgr::Get_Instance()->Set_Scene_Score(m_player->Get_Score());
		m_pCurScore->Set_Init_Score(CScoreMgr::Get_Instance()->Get_Scene_Score());
		m_pHighScore->Set_Init_Score(CScoreMgr::Get_Instance()->Get_High_Score());
	}

	//CTileMgr::Get_Instance()->Load_Tile(L"../Data/TileY_Stage1.dat");
}

void CStage1::Render(HDC hDC)
{
	CTileMgr::Get_Instance()->Render(hDC);

	// 위아래 Black BackGround
	HDC hBlackBG = CBmpMgr::Get_Instance()->Find_Img(L"Ground_2");
	BitBlt(hDC, 0, 0, 800, TILECY * 3, hBlackBG, 0, 0, SRCCOPY);
	BitBlt(hDC, 0, WINCY - TILECY, 800, TILECY, hBlackBG, 0, 0, SRCCOPY);

	// 목숨
	HDC hLife = CBmpMgr::Get_Instance()->Find_Img(L"Life");
	for (int i = 0; i < m_player->Get_Life(); ++i)
		GdiTransparentBlt(hDC, 0, 556 - (22 * i), 22, 22, hLife, 0, 0, 22, 22, RGB(15, 79, 174));	// 578 : 600 - 22(life 이미지 사이즈)

	// 점수
	//HDC hScore = CBmpMgr::Get_Instance()->Find_Img(L"score");
	//wchar_t tmp[50] = L"";
	//swprintf_s(tmp, L"SCORE : %d", m_player->Get_Score());
	//TextOut(hDC, 200, 50, tmp, wcslen(tmp));

	CObjMgr::Get_Instance()->Render(hDC);
}

void CStage1::Release()
{
	// [ Next Scene 공유 ] life, score Set
	CSceneMgr::Get_Instance()->Set_Player_Life(m_player->Get_Life());
	CScoreMgr::Get_Instance()->Set_Scene_Score(m_player->Get_Score());
	CScoreMgr::Get_Instance()->Set_High_Score(m_player->Get_Score());

	CObjMgr::Get_Instance()->Release();
	CTileMgr::Get_Instance()->Release();
}
