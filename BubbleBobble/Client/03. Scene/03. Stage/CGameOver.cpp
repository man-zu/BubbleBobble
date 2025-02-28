#include "stdafx.h"
#include "CGameOver.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "Obj.h"
#include "AbstractFactory.h"
#include "CBText.h"
#include "ObjMgr.h"
#include "CScoreMgr.h"
#include "CScoreMgr.h"

CGameOver::CGameOver()
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CGameOver::~CGameOver()
{
	Release();
}

void CGameOver::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Gameover_.bmp",L"Gameover_");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_Left.bmp",L"Player_Left_Death");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/HighScore.bmp", L"HighScore");

	m_tFrame = { 0, 3, 13, 200, GetTickCount64() };

	CObj* m_pHighScore = CAbstractFactory<CBText>::Create();
	dynamic_cast<CBText*>(m_pHighScore)->Set_Type(TEXTTYPE::NUM);
	m_pHighScore->Set_Init_Score(CScoreMgr::Get_Instance()->Get_Scene_Score());
	m_pHighScore->Set_Pos(450, 300);
	CObjMgr::Get_Instance()->Add_Object(TEXT, m_pHighScore);

	// ScoreMgr 에도 점수 저장
	CScoreMgr::Get_Instance()->SaveScoreData();
}

void CGameOver::Update()
{
	CObjMgr::Get_Instance()->Update();
	if (!m_bSceneChangeDelay)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSoundMgr::Get_Instance()->PlaySound(L"Ghost_Appear.wav", SOUND_EFFECT, SOUND_VOLUME, false);
		m_bSceneChangeDelay = true;
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
		return;
	}
}

void CGameOver::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwTime = GetTickCount64();
	}
}

void CGameOver::Render(HDC hDC)
{
	HDC hImg = CBmpMgr::Get_Instance()->Find_Img(L"Gameover_");
	BitBlt(hDC, 0,0,800, 600, hImg, 0, 0, SRCCOPY);

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"Player_Left_Death");


	GdiTransparentBlt(hDC,
		350,
		70,
		120,
		120,
		hMemDC,
		m_tFrame.iFrameStart * 44,
		m_tFrame.iMotion * 44,
		44, // 복사할 비트맵의 가로, 세로 길이
		44,
		RGB(95, 88, 88));	// 제거하고자 하는 색상을 전달

	if(CScoreMgr::Get_Instance()->Get_Scene_Score() >= CScoreMgr::Get_Instance()->Get_High_Score())
		GdiTransparentBlt(hDC, 350, 280, 110, 11, CBmpMgr::Get_Instance()->Find_Img(L"HighScore"), 0, 0, 220, 22, SRCCOPY);

	CObjMgr::Get_Instance()->Render(hDC);
}

void CGameOver::Release()
{
	CSceneMgr::Get_Instance()->Set_Ending(false);
	CSceneMgr::Get_Instance()->Delete_Extend_List();
	CObjMgr::Get_Instance()->Release();
}
