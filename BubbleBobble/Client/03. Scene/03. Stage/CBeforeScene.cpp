#include "stdafx.h"
#include "CBeforeScene.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "ObjMgr.h"
#include "CBText.h"

CBeforeScene::CBeforeScene()
{
}

CBeforeScene::~CBeforeScene()
{
	Release();
}

void CBeforeScene::Initialize()
{
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"BossStage_Theme.wav", SOUND_VOLUME + 20);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BOSS_STAGE_BG.bmp", L"BOSS_STAGE_BG");

	//CObj* pText = new CBText();
	//dynamic_cast<CBText*>(pText)->Set_Type(TEXTTYPE::BOSSUP);
	//pText->Set_Pos(220, 250);
	//CObjMgr::Get_Instance()->Add_Object(TEXT, pText);

	m_sceneChDelayTime = GetTickCount64();
}

void CBeforeScene::Update()
{
	if (m_sceneChDelayTime + 3300 < GetTickCount64())
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_BOSS);
	}
	//CObjMgr::Get_Instance()->Update();
}

void CBeforeScene::Late_Update()
{
	//CObjMgr::Get_Instance()->Late_Update();
}

void CBeforeScene::Render(HDC hDC)
{
	HDC hBlckBg = CBmpMgr::Get_Instance()->Find_Img(L"BOSS_STAGE_BG");
	BitBlt(hDC, 0, 0, 800, 600, hBlckBg, 0, 0, SRCCOPY);

	//CObjMgr::Get_Instance()->Render(hDC);
}

void CBeforeScene::Release()
{
	//CObjMgr::Get_Instance()->Release();
}
