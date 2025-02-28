#include "stdafx.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "CBPlayer.h"
#include "SoundMgr.h"

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	//m_tInfo.fCX = 150.f;
	//m_tInfo.fCY = 150.f;

	//CSoundMgr::Get_Instance()->PlaySound(L"Menu.wav", SOUND_EFFECT, 10);
}

int CMyButton::Update()
{
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			if (!lstrcmp(L"Start", m_pFrameKey))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Menu.wav", ITEM_SOUND_EFFECT, SOUND_VOLUME, false);
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_SKIN);
			}

			else if (!lstrcmp(L"Score", m_pFrameKey))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Menu.wav", ITEM_SOUND_EFFECT, SOUND_VOLUME, false);
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_SCORE);
			}

			else if (!lstrcmp(L"Edit", m_pFrameKey))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Menu.wav", ITEM_SOUND_EFFECT, SOUND_VOLUME, false);
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_EDIT);
			}

			else if (!lstrcmp(L"Exit", m_pFrameKey))
				DestroyWindow(g_hWnd);


			else if (!lstrcmp(L"SkinGreen", m_pFrameKey))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Menu.wav", ITEM_SOUND_EFFECT, SOUND_VOLUME, false);
				CSceneMgr::Get_Instance()->Set_Player_Color(PLAYERCOLOR::GREEN);
			}
			else if (!lstrcmp(L"SkinRed", m_pFrameKey))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Menu.wav", ITEM_SOUND_EFFECT, SOUND_VOLUME, false);
				CSceneMgr::Get_Instance()->Set_Player_Color(PLAYERCOLOR::RED);
			}

			return;
		}

		m_iDrawID = 1;
	}
	else
	{
		m_iDrawID = 0;
	}


	if (CKeyMgr::Get_Instance()->Key_Pressing('1'))
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Menu.wav", ITEM_SOUND_EFFECT, SOUND_VOLUME, false);
		CSceneMgr::Get_Instance()->Set_Player_Color(PLAYERCOLOR::GREEN);
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing('2'))
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Menu.wav", ITEM_SOUND_EFFECT, SOUND_VOLUME, false);
		CSceneMgr::Get_Instance()->Set_Player_Color(PLAYERCOLOR::RED);
	}

}

void CMyButton::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	if (m_pFrameKey == L"SkinGreen" || m_pFrameKey == L"SkinRed")
	{
		m_tInfo.fCX = 100.f;
		m_tInfo.fCY = 100.f;
		BitBlt(hDC, m_tRect.left, m_tRect.top, 100, 100, hMemDC, 0, 0, SRCCOPY);

		//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	}
	else 
	{
		m_tInfo.fCX = 150.f;
		m_tInfo.fCY = 22.f;

		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_iDrawID * 300,
			0,
			300, 
			40,
			NULL);
	}
}

void CMyButton::Release()
{
}
