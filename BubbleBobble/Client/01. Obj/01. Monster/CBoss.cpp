#include "stdafx.h"
#include "CBoss.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "CBossBullet.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "CBItem.h"
#include "SoundMgr.h"

CBoss::CBoss()
	:m_angryDelyTime(0), m_patternDelyTime(0), m_iPattern1Cnt(0),
	m_BeforeDirection({0,0})
{
	ZeroMemory(&m_lt, sizeof(INFO));
	ZeroMemory(&m_rb, sizeof(INFO));
	ZeroMemory(&m_playerCpyInfo, sizeof(INFO));
	m_ePreState = MONSTERSTATE::END;
	m_eCurState = MONSTERSTATE::BOSS_IDLE_LEFT;
}

CBoss::~CBoss()
{
	Release();
}

void CBoss::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss_Right.bmp",L"Boss_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss_Left.bmp",L"Boss_Left");
	m_pFrameKey = L"Boss_Left";
	//m_tInfo = {0, 0, 176, 176 };
	m_tInfo = {0, 0, 120, 176 };

	m_fSpeed = 3.f;
	m_direction = {1, -1};

	m_fHp = BOSSHP;

	m_eCurState = MONSTERSTATE::BOSS_IDLE_LEFT;

	m_eDir = DIR_LEFT;

	m_patternDelyTime = GetTickCount64() - 2000;
	m_damageMotionDelyTime = 0;
}

int CBoss::Update()
{
	// player 좌표 따라 바라보도록
	if (m_playerCpyInfo.fX <= m_tInfo.fX)
		m_eDir = DIR_LEFT;
	else
		m_eDir = DIR_RIGHT;

	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSoundMgr::Get_Instance()->PlaySound(L"Success.wav", ENDING, SOUND_VOLUME, false);
		//CSoundMgr::Get_Instance()->PlayBGM(L"Success.wav", SOUND_VOLUME);
		
		for (int i = 0; i < 3; ++i)
		{
			CObj* pItem = new CBItem();
			pItem->Initialize();
			dynamic_cast<CBItem*>(pItem)->item_Choose(5);
			pItem->Set_Pos((float)160 + 220*i, (float)100);
			CObjMgr::Get_Instance()->Add_Object(ITEM, pItem);
		}
		return OBJ_DEAD;
	}
	// eDir 방향으로 IDLE 애니메이션 처리
	else if (m_damageMotionDelyTime  && m_damageMotionDelyTime + 800 < GetTickCount64())
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Extra_Life.wav", SOUND_EFFECT, SOUND_VOLUME, false);		// 왜넣어줬지?

		if (m_eDir == DIR_LEFT)
			m_eCurState = MONSTERSTATE::BOSS_IDLE_LEFT;
		else if (m_eDir == DIR_RIGHT)
			m_eCurState = MONSTERSTATE::BOSS_IDLE_RIGHT;
		
		/*if (m_angryDelyTime)
		{
			if (m_eDir == DIR_LEFT)
				m_eCurState = MONSTERSTATE::BOSS_ANGRY_LEFT;
			else if (m_eDir == DIR_RIGHT)
				m_eCurState = MONSTERSTATE::BOSS_ANGRY_RIGHT;
		}*/
		m_damageMotionDelyTime = 0;

		m_direction = { 1, 1 };
	}
	else if(!m_damageMotionDelyTime)
	{
		if (m_eDir == DIR_LEFT)
			m_eCurState = MONSTERSTATE::BOSS_IDLE_LEFT;
		else if (m_eDir == DIR_RIGHT)
			m_eCurState = MONSTERSTATE::BOSS_IDLE_RIGHT;

		if (m_patternDelyTime + 4000 < GetTickCount64())
			m_patternDelyTime = 0;
	}

	// HP < 0 == 보스 버블상태
	if (0 >= m_fHp)
	{
		if (m_eDir == DIR_LEFT)
			m_eCurState = MONSTERSTATE::BOSS_BUBBLE_LEFT;
		if (m_eDir == DIR_RIGHT)
			m_eCurState = MONSTERSTATE::BOSS_BUBBLE_RIGHT;

		m_direction = {0, 0};
	}
	else if ( BOSSHP * 0.5 <= m_fHp )
	{
		if (!m_patternDelyTime)
		{
			srand(unsigned int(time(NULL)));
			switch (rand() % 2)
			{
			case 0:
				pattern2();
				break;
			case 1:
				pattern3();
				break;
			}
		}
	}
	else if (BOSSHP * 0.5 > m_fHp)
	{
		if (m_eDir == DIR_LEFT)
			m_eCurState = MONSTERSTATE::BOSS_ANGRY_LEFT;
		if (m_eDir == DIR_RIGHT)
			m_eCurState = MONSTERSTATE::BOSS_ANGRY_RIGHT;

		if (m_angryDelyTime == 0 || m_patternDelyTime + 4000 < GetTickCount64())
		{
			m_angryDelyTime = 0;
			m_patternDelyTime = 0;
		}

		if (!m_patternDelyTime)
			pattern1();
	}

	/*
	*	m_lt		tileLeftTop
	*	m_rb		tileRightBottom
	*/
	if (m_lt.fX >= m_tInfo.fX - (m_tInfo.fCX*0.5f) || m_rb.fX <= m_tInfo.fX + (m_tInfo.fCX * 0.5f))
		m_direction.x *= -1;
	if (m_lt.fY >= m_tInfo.fY - (m_tInfo.fCY * 0.5f) || m_rb.fY <= m_tInfo.fY + (m_tInfo.fCY * 0.5f))
		m_direction.y *= -1;

	m_tInfo.fX += (m_direction.x * m_fSpeed);
	m_tInfo.fY += (m_direction.y * m_fSpeed);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CBoss::Late_Update()
{
	Move_Frame();
	motion_Change();
}

void CBoss::pattern1()
{
	if (!m_patternDelyTime)
		m_patternDelyTime = GetTickCount64();

	if (!m_angryDelyTime)
		m_angryDelyTime = GetTickCount64();

	ULONGLONG TMP = GetTickCount64();


	if (m_angryDelyTime == 999999999999999)
	{
		// 보스 얼굴 기본으로 바꿔주고
		/*if (m_eDir == DIR_LEFT)
			m_eCurState = MONSTERSTATE::BOSS_IDLE_LEFT;
		else if (m_eDir == DIR_RIGHT)
			m_eCurState = MONSTERSTATE::BOSS_IDLE_RIGHT;*/

	}
	else
	{
		// player 가 wincx 왼쪽에 있으면 왼쪽에 불
		// wincx 오른쪽에 있으면 오른쪽에 불
		int iStartX = 0;

		if (m_playerCpyInfo.fX <= WINCX * 0.5)
			iStartX = 90;
		else
			iStartX = int(WINCX * 0.5) + 30;	// 30 : 보정치

		for (int i = 0; i < 14; ++i)
		{
			CObj* pBossBullet = CAbstractFactory<CBossBullet>::Create(float(iStartX + (23 * i)), float(100));
			CObjMgr::Get_Instance()->Add_Object(BOSSBULLET, pBossBullet);
		}

		++m_iPattern1Cnt;
		m_angryDelyTime = 999999999999999;
	}
}

void CBoss::pattern2()
{
	if (!m_patternDelyTime)
		m_patternDelyTime = GetTickCount64();

	// boss 중점 기준으로 좌 절반 총알
	m_fAngle = 90.f;

	for (int i = 0; i < 7; ++i)
	{
		POINT tmp;
		tmp.x = LONG(m_tInfo.fX + 100 * cos(m_fAngle * PI / 180));
		tmp.y = LONG(m_tInfo.fY - 100 * sin(m_fAngle * PI / 180));

		CObj* pBossBullet = CAbstractFactory<CBossBullet>::Create(float(tmp.x), float(tmp.y), m_fAngle);
		dynamic_cast<CBossBullet*>(pBossBullet)->Set_Boss_Type(BBTYPE::MOVE);
		CObjMgr::Get_Instance()->Add_Object(BOSSBULLET, pBossBullet);
		m_fAngle += 30.f;
	}
}

void CBoss::pattern3()
{
	if(!m_patternDelyTime)
		m_patternDelyTime = GetTickCount64();

	// boss 중점 기준으로 하 절반 총알
	m_fAngle = 180.f;

	for (int i = 0; i < 7; ++i)
	{
		POINT tmp;
		tmp.x = LONG(m_tInfo.fX + 100 * cos(m_fAngle * PI / 180));
		tmp.y = LONG(m_tInfo.fY - 100 * sin(m_fAngle * PI / 180));

		CObj* pBossBullet = CAbstractFactory<CBossBullet>::Create(float(tmp.x), float(tmp.y), m_fAngle);
		dynamic_cast<CBossBullet*>(pBossBullet)->Set_Boss_Type(BBTYPE::MOVE);
		CObjMgr::Get_Instance()->Add_Object(BOSSBULLET, pBossBullet);
		m_fAngle += 30.f;
	}
}

void CBoss::Render(HDC hDC)
{
	if (0 < m_fHp)
	{
		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(254, 6, 49));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

		Rectangle(hDC, m_tRect.left - 10, m_tRect.top - 10, (int(m_tRect.left) - 10) + int((m_fHp * 0.3f)), m_tRect.top);

		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
	}
	
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left - 30,
		m_tRect.top,
		(int)m_tInfo.fCX + 56,
		(int)m_tInfo.fCY,
		hMemDC,
		m_tFrame.iFrameStart * (int(m_tInfo.fCX) + 56),
		m_tFrame.iMotion * int(m_tInfo.fCY),
		int(m_tInfo.fCX) + 56, // 복사할 비트맵의 가로, 세로 길이
		int(m_tInfo.fCY),
		RGB(15, 79, 174));	// 제거하고자 하는 색상을 전달
}

void CBoss::Release()
{
}

void CBoss::motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{

			// left //
		case MONSTERSTATE::BOSS_IDLE_LEFT:
			m_tFrame = { 0, 3, 0, 200, GetTickCount64() };
			m_pFrameKey = L"Boss_Left";
			break;

		case MONSTERSTATE::BOSS_ANGRY_LEFT:
			m_tFrame = { 0, 1, 3, 200, GetTickCount64() };
			m_pFrameKey = L"Boss_Left";
			break;

		case MONSTERSTATE::BOSS_BUBBLE_LEFT:
			m_tFrame = { 0, 1, 4, 200, GetTickCount64() };
			m_pFrameKey = L"Boss_Left";
			break;

		case MONSTERSTATE::BOSS_DIE_LEFT:
			m_tFrame = { 0, 3, 2, 100, GetTickCount64() };
			m_pFrameKey = L"Boss_Left";
			break;

		case MONSTERSTATE::BOSS_DAMAGE_LEFT:
			m_tFrame = { 0, 3, 1, 100, GetTickCount64() };
			m_pFrameKey = L"Boss_Left";
			break;

			// right //
		case MONSTERSTATE::BOSS_IDLE_RIGHT:
			m_tFrame = { 0, 3, 0, 200, GetTickCount64() };
			m_pFrameKey = L"Boss_Right";
			break;

		case MONSTERSTATE::BOSS_ANGRY_RIGHT:
			m_tFrame = { 0, 1, 3, 200, GetTickCount64() };
			m_pFrameKey = L"Boss_Right";
			break;

		case MONSTERSTATE::BOSS_BUBBLE_RIGHT:
			m_tFrame = { 0, 1, 4, 200, GetTickCount64() };
			m_pFrameKey = L"Boss_Right";
			break;

		case MONSTERSTATE::BOSS_DIE_RIGHT:
			m_tFrame = { 0, 3, 2, 100, GetTickCount64() };
			m_pFrameKey = L"Boss_Right";
			break;

		case MONSTERSTATE::BOSS_DAMAGE_RIGHT:
			m_tFrame = { 0, 3, 1, 100, GetTickCount64() };
			m_pFrameKey = L"Boss_Right";
			break;
		}
	}
}
