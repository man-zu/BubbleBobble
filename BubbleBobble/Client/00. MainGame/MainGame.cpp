#include "stdafx.h"
#include "MainGame.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "Mouse.h"
#include "CScoreMgr.h"

CMainGame::CMainGame()
	: m_dwTime(GetTickCount64()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground_2.bmp", L"Ground_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/tile/tile.bmp", L"tile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Life.bmp", L"Life");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bubble/Lightning_Bubble.bmp", L"Lightning_Bubble");
	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_LOGO);
	CSoundMgr::Get_Instance()->Initialize();
	
	// 마우스
	//CObjMgr::Get_Instance()->Add_Object(MOUSE, CAbstractFactory<CMouse>::Create());

	m_mouse = CAbstractFactory<CMouse>::Create();
}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
	dynamic_cast<CMouse*>(m_mouse)->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	dynamic_cast<CMouse*>(m_mouse)->Late_Update();
}

void CMainGame::Render()
{
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount64())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		// SetWindowText : 윈도우 타일에 출력하는 함수
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount64();
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"Back");
	HDC hGroundDC = CBmpMgr::Get_Instance()->Find_Img(L"Ground");
	
	CSceneMgr::Get_Instance()->Render(hMemDC);

	dynamic_cast<CMouse*>(m_mouse)->Render(hMemDC);
	
	BitBlt(m_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);	
}

void CMainGame::Release()
{
	Safe_Delete(m_mouse);
	CScoreMgr::Destroy_Instance();
	CSceneMgr::Get_Instance()->Delete_Extend_List();
	CSoundMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CLineMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	
	ReleaseDC(g_hWnd, m_DC);
}
