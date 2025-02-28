#include "stdafx.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "ObjMgr.h"
#include "Mouse.h"
#include "CScoreMgr.h"

CLogo::CLogo()
	: m_bSoundTrue(false)
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/intro.bmp", L"intro");
	CScoreMgr::Get_Instance()->LoadScoreData();
}

void CLogo::Update()
{
	if (!m_bSoundTrue)
	{
		CSoundMgr::Get_Instance()->PlayBGM(L"Intro_Theme.wav", SOUND_VOLUME);
		m_bSoundTrue = true;
	}
}

void CLogo::Late_Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
		//CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_SCORE);
		//CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_BOSSBEFORE);
		//CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_EXTEND);
		//CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE2);
		return;
	}
}

void CLogo::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"intro");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
}
