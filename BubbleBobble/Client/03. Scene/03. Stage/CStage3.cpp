#include "stdafx.h"
#include "CStage3.h"
#include "LineMgr.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "CollisionMgr.h"
#include "CBPlayer.h"
#include "CBMonsterS2.h"
#include "CBMonsterS3.h"
#include "SceneMgr.h"
#include "CBText.h"
#include "SoundMgr.h"
#include "CBExtend.h"
#include "CFireBubble.h"
#include "CScoreMgr.h"

CStage3::CStage3() : m_player(nullptr), m_fireDelayTime(0), m_bFireCreate(false)
{
}

CStage3::~CStage3()
{
	Release();
}

void CStage3::Initialize()
{
	m_HurryUpTime = GetTickCount64();

	// CObj 생성
	m_player = CAbstractFactory<CBPlayer>::Create();
	m_player->Set_Init_Life(CSceneMgr::Get_Instance()->Get_Player_Life());
	m_player->Set_Init_Score(CScoreMgr::Get_Instance()->Get_Scene_Score());
	CObjMgr::Get_Instance()->Add_Object(PLAYER, m_player);

	for (int i = 0; i < 5; ++i)
		//CObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<CBMonsterS2>::Create((float)200 + (50 * i), (float)400));
		CObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<CBMonsterS2>::Create((float)200 + (50 * i), (float)500));

	// EXTEND
	CObj* pObj = new CBExtend(EXTENDSTATE::E3);
	pObj->Initialize();
	pObj->Set_Pos(200, 550);
	CObjMgr::Get_Instance()->Add_Object(EXTEND, pObj);

	pObj = new CBExtend(EXTENDSTATE::D5);
	pObj->Initialize();
	pObj->Set_Pos(300, 550);
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
	CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Stage3.dat");

	// Sound
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"Main_Theme.wav", SOUND_VOLUME);

	m_ExtendStageDelayTime = 0;
}

void CStage3::Update()
{
	CObjMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();
}

void CStage3::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	CTileMgr::Get_Instance()->Late_Update();

	// EXTEND
	if (6 <= CSceneMgr::Get_Instance()->Get_Extend_List().size())
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSoundMgr::Get_Instance()->PlaySound(L"Alphabet_Theme.wav", SOUND_EFFECT, SOUND_VOLUME, false);
		CObjMgr::Get_Instance()->Set_All_Object_Stop();
		
		if(!m_ExtendStageDelayTime)
			m_ExtendStageDelayTime = GetTickCount64();

		for (auto pExtend : CSceneMgr::Get_Instance()->Get_Extend_List())
		{
			dynamic_cast<CBExtend*>(pExtend)->Set_bExtend_Get_All(true);
			//CObjMgr::Get_Instance()->Add_Object(EXTEND, pExtend);
		}
	}

	if (m_ExtendStageDelayTime && m_ExtendStageDelayTime + 4000 < GetTickCount64())
	{
		// [ Next Scene 공유 ] life, score Set
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_EXTEND);
		return;
	}

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
	if (m_sceneChDelayTime && m_sceneChDelayTime + 4000 < GetTickCount64())
		m_bSceneChangeDelay = true;
	else if (!m_sceneChDelayTime && !CObjMgr::Get_Instance()->Get_Obj(MONSTER).size())
		m_sceneChDelayTime = GetTickCount64();

	if (!m_player->Get_Life())
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_GAMEOVER);
		return;
	}
	else if (m_bSceneChangeDelay)
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_BOSS);
	}
	else
	{
		// 매 화면마다 Score Uodate 처리
		CScoreMgr::Get_Instance()->Set_High_Score(m_player->Get_Score());
		CScoreMgr::Get_Instance()->Set_Scene_Score(m_player->Get_Score());
		m_pCurScore->Set_Init_Score(CScoreMgr::Get_Instance()->Get_Scene_Score());
		m_pHighScore->Set_Init_Score(CScoreMgr::Get_Instance()->Get_High_Score());
	}
	
	// fire
	if (!m_fireDelayTime)
		m_fireDelayTime = GetTickCount64();

	if (m_fireDelayTime && m_fireDelayTime + 2000 < GetTickCount64())
	{
		m_bFireCreate = false;
		m_fireDelayTime = 0;
	}

	if (!m_bFireCreate)
	{
		srand(unsigned int(time(NULL)));
		int x = rand() % 10;

		float tmp = CTileMgr::Get_Instance()->Get_First_Tile().fY + TILECY;
		CObjMgr::Get_Instance()->Add_Object(FIREBUBBLE, CAbstractFactory<CFireBubble>::Create((float)200 + (40 * x), (float)80));

		m_bFireCreate = true;
	}
}

void CStage3::Render(HDC hDC)
{
	CTileMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);
	CSceneMgr::Get_Instance()->Extend_Render(hDC);

	// 목숨
	HDC hLife = CBmpMgr::Get_Instance()->Find_Img(L"Life");
	for (int i = 0; i < m_player->Get_Life(); ++i)
		GdiTransparentBlt(hDC, 0, 556 - (22 * i), 22, 22, hLife, 0, 0, 22, 22, RGB(15, 79, 174));	// 578 : 600 - 22(life 이미지 사이즈)
}

void CStage3::Release()
{
	// [ Next Scene 공유 ] life, score Set
	CSceneMgr::Get_Instance()->Set_Player_Life(m_player->Get_Life());
	CScoreMgr::Get_Instance()->Set_Scene_Score(m_player->Get_Score());
	CScoreMgr::Get_Instance()->Set_High_Score(m_player->Get_Score());

	CObjMgr::Get_Instance()->Release();
	CTileMgr::Get_Instance()->Release();
}
