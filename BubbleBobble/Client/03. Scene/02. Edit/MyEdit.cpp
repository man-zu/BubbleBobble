#include "stdafx.h"
#include "MyEdit.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"

CMyEdit::CMyEdit()
{
}

CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
	CTileMgr::Get_Instance()->Initialize();
}

void CMyEdit::Update()
{
	CTileMgr::Get_Instance()->Update();
}

void CMyEdit::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	Key_Input();



}

void CMyEdit::Render(HDC hDC)
{
	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"Ground");
	BitBlt(hDC, 0+ ScrollX, 0+ ScrollY, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	
	CTileMgr::Get_Instance()->Render(hDC);
}

void CMyEdit::Release()
{

}



void CMyEdit::Key_Input()
{

	float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F1))
		ChangeTileType(1, 1);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F2))
		ChangeTileType(1, 2);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F3))
		ChangeTileType(1, 3);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F4))
		ChangeTileType(1, 4);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F5))
		ChangeTileType(1, 5);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F6))
		ChangeTileType(1, 6);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DELETE))
		ChangeTileType(0, 0);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F11))
		CTileMgr::Get_Instance()->Save_Tile();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F12))
		CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile.dat");

	if (CKeyMgr::Get_Instance()->Key_Down(VK_BACK))
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
}


void CMyEdit::ChangeTileType(int _option, int _drawID)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	CTileMgr::Get_Instance()->Picking_Tile(pt, _option, _drawID);
}