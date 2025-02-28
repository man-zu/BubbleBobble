#include "stdafx.h"
#include "SceneMgr.h"
#include "CStageBoss.h"
#include "SkinChg.h"
#include "CStageExtend.h"
#include "CScore.h"
#include "CBeforeScene.h"

CSceneMgr*	CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_eCurScene(SC_END), m_ePreScene(SC_END), m_iPlayerLife(5), m_ePlayerColor(PLAYERCOLOR::GREEN), m_bEndingScore(false)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID eScene)
{
	m_eCurScene = eScene;

	if (m_ePreScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SC_LOGO:
			m_pScene = new CLogo;
			break;

		case SC_MENU:
			m_pScene = new CMyMenu;
			break;

		case SC_SCORE:
			m_pScene = new CScore;
			break;

		case SC_EDIT:
			m_pScene = new CMyEdit;
			break;

		case SC_SKIN:
			m_pScene = new SkinChg;
			break;

		case SC_STAGE1:
			m_pScene = new CStage1;
			break;

		case SC_STAGE2:
			m_pScene = new CStage2;
			break;

		case SC_STAGE3:
			m_pScene = new CStage3;
			break;

		case SC_BOSSBEFORE:
			m_pScene = new CBeforeScene;
			break;

		case SC_BOSS:
			m_pScene = new CStageBoss;
			break;

		case SC_EXTEND:
			m_pScene = new CStageExtend;
			break;

		case SC_GAMEOVER:
			m_pScene = new CGameOver;
			break;
		}

		m_pScene->Initialize();

		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}

void CSceneMgr::Extend_Render(HDC hDC)
{
	for (auto pExtend : m_ExtendList)
	{
		pExtend->Update();
		pExtend->Render(hDC);
	}
}
