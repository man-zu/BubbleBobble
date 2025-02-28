#include "stdafx.h"
#include "CStageBoss.h"
#include "LineMgr.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "CollisionMgr.h"
#include "CBPlayer.h"
#include "SceneMgr.h"
#include "CBoss.h"
#include "CBossBullet.h"
#include "CBMonsterS2.h"
#include "CBElectricItem.h"
#include "CBItem.h"
#include "SoundMgr.h"
#include "KeyMgr.h"
#include "CBText.h"
#include "CScoreMgr.h"

CStageBoss::CStageBoss() : m_player(nullptr), pBoss(nullptr), m_delayTime(0)
{
}

CStageBoss::~CStageBoss()
{
	Release();
}

void CStageBoss::Initialize()
{

	m_player = CAbstractFactory<CBPlayer>::Create();
	m_player->Set_Init_Life(CSceneMgr::Get_Instance()->Get_Player_Life());
	m_player->Set_Init_Score(CScoreMgr::Get_Instance()->Get_Scene_Score());
	CObjMgr::Get_Instance()->Add_Object(PLAYER, m_player);

	CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Stageboss.dat");

	pBoss = CAbstractFactory<CBoss>::Create(300, 300);
	CObjMgr::Get_Instance()->Add_Object(BOSS, pBoss);
	dynamic_cast<CBoss*>(pBoss)->Set_LT(CTileMgr::Get_Instance()->Get_Left_Top_Tile());
	dynamic_cast<CBoss*>(pBoss)->Set_RB(CTileMgr::Get_Instance()->Get_Right_Bottom_Tile());

	CObjMgr::Get_Instance()->Add_Object(ITEM, CAbstractFactory< CBElectricItem>::Create());

	for (int i = 0; i < 2; ++i)
	{
		CObj* pMonster = new CBMonsterS2((float)300 + (100 * i), (float)450);
		pMonster->Initialize();
		CObjMgr::Get_Instance()->Add_Object(MONSTER, pMonster);
	}

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


}

void CStageBoss::Update()
{
	if(0 < CObjMgr::Get_Instance()->Get_Obj(BOSS).size())		// 죽으면 size 0 되는거 아님 ?
		dynamic_cast<CBoss*>(pBoss)->Set_Player_Cpy(m_player->Get_Info());
	else
	{
		// 원래 속도로
		m_player->Set_SpeedDown();

		// 승리 화면 띄우기
		/*CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSoundMgr::Get_Instance()->PlayBGM(L"Success.wav", SOUND_VOLUME + 10);*/

		if(!m_delayTime)
			m_delayTime = GetTickCount64();

		if (m_delayTime + 12000 < GetTickCount64())
		{
			CSceneMgr::Get_Instance()->Set_Ending(true);
			CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_SCORE);
			return;
		}
	}

	CObjMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();

	Key_Input();
}

void CStageBoss::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	CTileMgr::Get_Instance()->Late_Update();

	if (!m_player->Get_Life())
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_GAMEOVER);
		return;
	}
	else
	{
		CScoreMgr::Get_Instance()->Set_High_Score(m_player->Get_Score());
		CScoreMgr::Get_Instance()->Set_Scene_Score(m_player->Get_Score());
		m_pCurScore->Set_Init_Score(CScoreMgr::Get_Instance()->Get_Scene_Score());
		m_pHighScore->Set_Init_Score(CScoreMgr::Get_Instance()->Get_High_Score());
	}
}

void CStageBoss::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down('F1'))
	{
		CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_BGM, 0.2f);
	}

	if (CKeyMgr::Get_Instance()->Key_Down('F2'))
	{
		CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_EFFECT, 0.2f);
		CSoundMgr::Get_Instance()->SetChannelVolume(ITEM_SOUND_EFFECT, 0.2f);
	}
}

void CStageBoss::Render(HDC hDC)
{
	CTileMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);

	// 목숨
	HDC hLife = CBmpMgr::Get_Instance()->Find_Img(L"Life");
	for (int i = 0; i < m_player->Get_Life(); ++i)
		GdiTransparentBlt(hDC, 0, 556 - (22 * i), 22, 22, hLife, 0, 0, 22, 22, RGB(15, 79, 174));	// 578 : 600 - 22(life 이미지 사이즈)
}

void CStageBoss::Release()
{
	CSceneMgr::Get_Instance()->Set_Ending(true);

	// [ Next Scene 공유 ] life, score Set
	CSceneMgr::Get_Instance()->Set_Player_Life(m_player->Get_Life());
	CScoreMgr::Get_Instance()->Set_Scene_Score(m_player->Get_Score());
	CScoreMgr::Get_Instance()->Set_High_Score(m_player->Get_Score());

	CObjMgr::Get_Instance()->Release();
	CTileMgr::Get_Instance()->Release();
}
