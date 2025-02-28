#include "stdafx.h"
#include "SkinChg.h"
#include "CBPlayer.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "MyButton.h"
#include "SoundMgr.h"

SkinChg::SkinChg() : m_player(nullptr)
{
}

SkinChg::~SkinChg()
{
	Release();
}

void SkinChg::Initialize()
{
	m_player = CAbstractFactory<CBPlayer>::Create(102, 200);
	dynamic_cast<CBPlayer*>(m_player)->Set_SkinRender();
	dynamic_cast<CBPlayer*>(m_player)->Set_ChooseSkin();
	CObjMgr::Get_Instance()->Add_Object(PLAYER, m_player);
	CTileMgr::Get_Instance()->Load_Tile(L"../Data/Tile_Skin.dat");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/skin_color_green.bmp", L"SkinGreen");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/skin_color_red.bmp", L"SkinRed");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/skin_slelect.bmp", L"skin_slelect");


	CObj* pButton = CAbstractFactory<CMyButton>::Create(580.f, 300.f);
	pButton->Set_FrameKey(L"SkinGreen");
	CObjMgr::Get_Instance()->Add_Object(SKINBUTTON, pButton);

	pButton = CAbstractFactory<CMyButton>::Create(580.f, 410.f);
	pButton->Set_FrameKey(L"SkinRed");
	CObjMgr::Get_Instance()->Add_Object(SKINBUTTON, pButton);

	CSoundMgr::Get_Instance()->PlaySound(L"Intro_Theme.wav", SOUND_BGM, SOUND_VOLUME, false);
}

void SkinChg::Update()
{
	CObjMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();
}

void SkinChg::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_BACK))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
		return;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		//CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE1);
		return;
	}
	CObjMgr::Get_Instance()->Late_Update();
}

void SkinChg::Render(HDC hDC)
{
	//HDC hBg = CBmpMgr::Get_Instance()->Find_Img(L"Ground_2");
	HDC hBg = CBmpMgr::Get_Instance()->Find_Img(L"skin_slelect");
	BitBlt(hDC, 0, 0, 800, 600, hBg, 0, 0, SRCCOPY);

	//CTileMgr::Get_Instance()->Render(hDC);

	// button  삭제때문에 블랙 이미지로 덮음
	/*hBg = CBmpMgr::Get_Instance()->Find_Img(L"Ground_2");
	BitBlt(hDC, 0, 0, 500, 600, hBg, 0, 0, SRCCOPY);*/


	//wchar_t tmp[50] = L"";
	//swprintf_s(tmp, L" <- ");
	//TextOut(hDC, 150, 180, tmp, wcslen(tmp));
	//swprintf_s(tmp, L" -> ");
	//TextOut(hDC, 290, 180, tmp, wcslen(tmp));
	//swprintf_s(tmp, L" SPACEBAR ");
	//TextOut(hDC, 190, 180, tmp, wcslen(tmp));

	/*wchar_t tmp2[50] = L"";
	swprintf_s(tmp2, L"SELECT COMPLETE ?");
	TextOut(hDC, 510, 180, tmp2, wcslen(tmp2));
	swprintf_s(tmp2, L"PRESS 'ENTER' ");
	TextOut(hDC, 520, 200, tmp2, wcslen(tmp2));*/

	CObjMgr::Get_Instance()->Render(hDC);
	/*list<CObj*> pPlayer =  CObjMgr::Get_Instance()->Get_Obj(PLAYER);

	for (auto player : pPlayer)
	{
		player->Render(hDC);
	}*/
}

void SkinChg::Release()
{
	/*
	*   누수 잡기
		Dumping objects ->
		{89002} normal block at 0x008F96F0, 8 bytes long.
		 Data: <`L      > 60 4C 8D 00 00 00 00 00 
		C:\jusin\bubble\BubbleBobble\TileMgr.h(22) : {89001} normal block at 0x008D4C60, 16 bytes long.
		 Data: <                > F0 96 8F 00 00 00 00 00 00 00 00 00 00 00 00 00 
		{89000} normal block at 0x008F81A0, 24 bytes long.
		 Data: <                > A0 81 8F 00 A0 81 8F 00 A0 81 8F 00 01 01 CD CD 
		{88999} normal block at 0x008F9990, 8 bytes long.
		 Data: <p       > 70 84 8F 00 00 00 00 00 
		C:\jusin\bubble\BubbleBobble\SoundMgr.h(11) : {88998} normal block at 0x008F8470, 28 bytes long.
		 Data: <                > 90 99 8F 00 A0 81 8F 00 00 00 00 00 CD CD CD CD 
	*/ 
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CObjMgr::Get_Instance()->Release();
	CTileMgr::Get_Instance()->Release();
}
