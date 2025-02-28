#include "stdafx.h"
#include "CBMonster.h"
#include "SoundMgr.h"
#include "BmpMgr.h"

CBMonster::CBMonster()
    :m_ePreState(MONSTERSTATE::WALK_LEFT), m_eCurState(MONSTERSTATE::WALK_LEFT), m_bubbleDelayTime(0), m_dieDelayTime(0)
	, m_fHp(1.f), m_damageMotionDelyTime(0), m_bHurryUp(false), m_HurryDelayTime(0), m_iFrameChange(0)
{
}

CBMonster::~CBMonster()
{
}

void CBMonster::Initialize()
{
}

int CBMonster::Update()
{
	if (0 >= m_fHp)
		m_bDead = true;

    if (m_bDead)
    {
        // render set
        return OBJ_DEAD;
    }

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CBMonster::Late_Update()
{
}

void CBMonster::Render(HDC hDC)
{
	if (m_bHurryUp && m_HurryDelayTime + 2000 > GetTickCount64())
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/HurryUp.bmp", L"HurryUp");

		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"HurryUp");

		GdiTransparentBlt(hDC,
			310,
			250,
			164,
			22,
			hMemDC,
			164*1,
			0,
			164, // 복사할 비트맵의 가로, 세로 길이
			22,
			RGB(15, 79, 174));	// 제거하고자 하는 색상을 전달
	}
}

void CBMonster::Release()
{
}

bool CBMonster::Set_Bubble()
{
	// 몬스터 버블에 갇혀있을 때
	if (m_eCurState != MONSTERSTATE::MONSTER_BUBBLE)
	{
		m_eCurState = MONSTERSTATE::MONSTER_BUBBLE;
		m_bubbleDelayTime = GetTickCount64();

		return true;
	}
	return false;
}

void CBMonster::Set_Hurry()
{
	m_fSpeed *= 2;
	m_bHurryUp = true;
	m_HurryDelayTime = GetTickCount64();

	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"HurryUp_Theme.wav", SOUND_VOLUME + 10);
}

