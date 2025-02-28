#include "stdafx.h"
#include "CScore.h"
#include "BmpMgr.h"
#include "CScoreMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "CBText.h"
#include "SceneMgr.h"
#include "KeyMgr.h"

CScore::CScore()
{
}

CScore::~CScore()
{
	Release();
}

void CScore::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/HighScoreWhite.bmp", L"HighScoreWhite");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/HighScore.bmp", L"HighScore");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ending_Score.bmp", L"Ending_Score");

	int i = 0;
	for (auto iScore : CScoreMgr::Get_Instance()->Get_Score_Data())
	{
		CObj* pScore = CAbstractFactory<CBText>::Create(); //300, 200 + (30 * i)
		dynamic_cast<CBText*>(pScore)->Set_Type(TEXTTYPE::NUM);
		pScore->Set_Init_Score(iScore);
		pScore->Set_Pos(500.f, 150.f + (30.f * i));
		CObjMgr::Get_Instance()->Add_Object(TEXT, pScore);

		//CObj* m_pHighScore = CAbstractFactory<CBText>::Create();
		//dynamic_cast<CBText*>(m_pHighScore)->Set_Type(TEXTTYPE::NUM);
		//m_pHighScore->Set_Init_Score(CSceneMgr::Get_Instance()->Get_Scene_Score());
		//m_pHighScore->Set_Pos(450, 300);
		//CObjMgr::Get_Instance()->Add_Object(TEXT, m_pHighScore);
		++i;
	}
	// ScoreMgr 에도 점수 저장
	//CScoreMgr::Get_Instance()->SaveScoreData();
}

void CScore::Update()
{
	CObjMgr::Get_Instance()->Update();

	if(CKeyMgr::Get_Instance()->Key_Down(VK_BACK))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
		return;
	}

	else if (CSceneMgr::Get_Instance()->Get_Ending()&&CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
		return;
	}
}

void CScore::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CScore::Render(HDC hDC)
{
	if (CSceneMgr::Get_Instance()->Get_Ending())
	{
		// 점수 저장
		if (!m_bSceneChangeDelay)
		{
			CScoreMgr::Get_Instance()->SaveScoreData();
			m_bSceneChangeDelay = true;
		}
		// 이전 extendList 삭제
		CSceneMgr::Get_Instance()->Delete_Extend_List();

		HDC hBlackBG = CBmpMgr::Get_Instance()->Find_Img(L"Ending_Score");
		BitBlt(hDC, 0, 0, 800, 600, hBlackBG, 0, 0, SRCCOPY);
		
		for (auto p : CObjMgr::Get_Instance()->Get_Obj(TEXT))
		{
			p->Set_Pos(600, 150);
			p->Set_Init_Score(CScoreMgr::Get_Instance()->Get_Scene_Score());
		}
		CObjMgr::Get_Instance()->Render(hDC);
		if (CScoreMgr::Get_Instance()->Get_Scene_Score() >= CScoreMgr::Get_Instance()->Get_High_Score())
			GdiTransparentBlt(hDC, 480, 125, 140, 14, CBmpMgr::Get_Instance()->Find_Img(L"HighScore"), 0, 0, 220, 22, SRCCOPY);
	}
	else
	{
		HDC hBlackBG = CBmpMgr::Get_Instance()->Find_Img(L"Ground_2");
		BitBlt(hDC, 0, 0, 800, 600, hBlackBG, 0, 0, SRCCOPY);

		GdiTransparentBlt(hDC, 290, 100, 220, 22, CBmpMgr::Get_Instance()->Find_Img(L"HighScoreWhite"), 0, 0, 220, 22, SRCCOPY);
		

		CObjMgr::Get_Instance()->Render(hDC);
	}
}

void CScore::Release()
{
	CSceneMgr::Get_Instance()->Set_Ending(false);
	CObjMgr::Get_Instance()->Release();
}
