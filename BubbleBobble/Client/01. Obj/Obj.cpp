#include "stdafx.h"
#include "Obj.h"
#include "TileMgr.h"
#include "SoundMgr.h"

CObj::CObj()
	: m_fSpeed(0.f), m_eDir(DIR_END), m_bDead(false), m_fAngle(0.f), m_fDistance(0.f), m_pTarget(nullptr), m_bEgnoreCol(false), m_bSoundDelay(false)
	 ,m_pFrameKey(nullptr), m_fGravity(0.f), m_direction({1,1}), m_eColTile(COLLSTATE::NONE), m_eCol2Tile(COLLSTATE::NONE), m_iLife(5), m_iScore(0), m_bColTile(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CObj::~CObj()
{
}

void CObj::Set_Hurry()
{
}

void CObj::Update_Rect()
{
	m_tRect.left	= LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	= LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Update_InfoY()
{
	if (m_tInfo.fY > 600)
	{
		m_tInfo.fY = CTileMgr::Get_Instance()->Get_First_Tile().fY+TILECY;
	}
	else if (m_tInfo.fY < CTileMgr::Get_Instance()->Get_First_Tile().fY + TILECY)
	{
		//m_tInfo.fY += (WINCY - m_tInfo.fY);
	}
	
}

void CObj::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
	{
		++m_tFrame.iFrameStart;
		
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwTime = GetTickCount64();
	}

}
