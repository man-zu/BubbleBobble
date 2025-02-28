#include "stdafx.h"
#include "MyMenu.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "MyButton.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CMyMenu::CMyMenu()
{
}

CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/menu.bmp", L"menu");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/BStart.bmp", L"Start");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/BEdit.bmp", L"Edit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/BExit.bmp", L"Exit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/BScore.bmp", L"Score");

	CObj*	pButton = CAbstractFactory<CMyButton>::Create(380.f, 390.f);
	pButton->Set_FrameKey(L"Start");
	CObjMgr::Get_Instance()->Add_Object(MENUBUTTON, pButton);

	pButton = CAbstractFactory<CMyButton>::Create(380.f, 435.f);
	pButton->Set_FrameKey(L"Score");
	CObjMgr::Get_Instance()->Add_Object(MENUBUTTON, pButton);

	pButton = CAbstractFactory<CMyButton>::Create(365.f, 480.f);
	pButton->Set_FrameKey(L"Edit");
	CObjMgr::Get_Instance()->Add_Object(MENUBUTTON, pButton);

	pButton = CAbstractFactory<CMyButton>::Create(365.f, 520.f);
	pButton->Set_FrameKey(L"Exit");
	CObjMgr::Get_Instance()->Add_Object(MENUBUTTON, pButton);

	CSoundMgr::Get_Instance()->PlaySound(L"Intro_Theme.wav", SOUND_BGM, SOUND_VOLUME, false);
}

void CMyMenu::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CMyMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CMyMenu::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"menu");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(hDC);
}

void CMyMenu::Release()
{
	CObjMgr::Get_Instance()->Release();
	//CObjMgr::Get_Instance()->Delete_ID(MENUBUTTON);
}
