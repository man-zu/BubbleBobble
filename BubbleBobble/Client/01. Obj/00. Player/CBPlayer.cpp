#include "stdafx.h"
#include "CBPlayer.h"
#include "CollisionMgr.h"
#include "KeyMgr.h"
#include "LineMgr.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "CBubble.h"
#include "CElectricalBubble.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "SoundMgr.h"

CBPlayer::CBPlayer()
    : m_bJump(false), m_fTime(0.f), m_ePreState(PLAYERSTATE::IDLE_RIGHT), m_eCurState(PLAYERSTATE::IDLE_RIGHT)
	, m_fJumpPower(0), m_bubbleDelyTime(0), m_bfall(false), m_bOnAir(false), m_deadDelyTime(0), m_bDeadDely(true), m_DownDelayTime(0)
	, m_bCreateNewDelayTime(0), m_bElectBubble(false), m_bMoveDelyTime(0), m_iSkinSceneRender(1), m_ePlayerColor(PLAYERCOLOR::GREEN)
{
}

CBPlayer::~CBPlayer()
{
	Release();
}

void CBPlayer::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_Right_Red.bmp", L"Player_Right_Red");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_Left_Red.bmp", L"Player_Left_Red");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_StageMove_new_Red.bmp", L"Player_StageMove_new_Red");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_StageMove_new_Red.bmp", L"Player_StageMove_Red");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_Right.bmp", L"Player_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_Left.bmp", L"Player_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_StageMove_new.bmp", L"Player_StageMove_new");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_StageMove_new.bmp", L"Player_StageMove");

	m_ePlayerColor = CSceneMgr::Get_Instance()->Get_Player_Color();

	m_pFrameKey = L"Player_Right";
	if (m_ePlayerColor == PLAYERCOLOR::RED)
		m_pFrameKey = L"Player_Right_Red";

	m_tInfo = {100, 200, 32, 32};
	m_iScore = 0;
	m_iLife = 5;
	m_fSpeed = 4.f;
	m_fGravity = 4.6f;
	m_fJumpPower = 10.f;

	// PLAYER RENDER 
	m_eDir = DIR_RIGHT;
	m_eCurState = PLAYERSTATE::IDLE_RIGHT;

	m_bCreateNewDelayTime = GetTickCount64();
}

int CBPlayer::Update()
{
	if (0 >= m_iLife)
		return OBJ_DEAD;


	// 맵 이탈 충돌문제 생각하기


	// 진입 안돼ㅐㅐㅐㅐㅐ
	if (m_ePreState == PLAYERSTATE::MOVE_POP && m_bMoveDelyTime + 500 < GetTickCount64())
	{
		m_bMoveDelyTime = 0;
	}

	// 새로 태어나면 n 초 무적
	/*if (m_bDead && m_bCreateNewDelayTime + 10000 > GetTickCount64())
	{
		m_bDead = false;
		m_bCreateNewDelayTime = 0;
	}
	else */if (m_bDead)
	{

		if (!m_deadDelyTime)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Player_Death.wav", SOUND_EFFECT_PLAYER_DEAD, SOUND_VOLUME, false);
			m_deadDelyTime = GetTickCount64();
		}

		if (m_eDir == DIR_RIGHT)
			m_eCurState = PLAYERSTATE::DIE_RIGHT;
		else if (m_eDir == DIR_LEFT)
			m_eCurState = PLAYERSTATE::DIE_LEFT;

		if (m_deadDelyTime + 500 < GetTickCount64())
		{
			if (m_bDeadDely)
			{
				--m_iLife;
				m_bDeadDely = false;
				m_bSoundDelay = false;
				m_bCreateNewDelayTime = GetTickCount64();
			}
		}

		if (m_deadDelyTime + 2000 < GetTickCount64())
		{
			this->Set_Pos(100,200);
			m_bDeadDely = true;
			m_bDead = false;
			m_deadDelyTime = 0;
			m_bMoveDelyTime = 0;
		}
	}
	else if (m_bubbleDelyTime + 1000 < GetTickCount64())
	{
		//if (m_bMoveDelyTime && m_bMoveDelyTime + 500 < GetTickCount64())
		if (m_bMoveDelyTime)
		{
			m_eCurState = PLAYERSTATE::MOVE_ING;

			if (CKeyMgr::Get_Instance()->Key_Down('Z'))
			{
				if (m_eDir == DIR_RIGHT)
					m_eCurState = PLAYERSTATE::IDLE_RIGHT;
				else if (m_eDir == DIR_LEFT)
					m_eCurState = PLAYERSTATE::IDLE_LEFT;


				m_eCurState = PLAYERSTATE::MOVE_POP;
				m_bMoveDelyTime = GetTickCount64();
			}
		}
		else
		{
			if (m_eDir == DIR_RIGHT)
				m_eCurState = PLAYERSTATE::IDLE_RIGHT;
			else if (m_eDir == DIR_LEFT)
				m_eCurState = PLAYERSTATE::IDLE_LEFT;
		}
	}

	if (m_bDeadDely)
	{
		Key_Input();
		Jump();
	}

	__super::Update_InfoY();
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBPlayer::Late_Update()
{
	Move_Frame();
	Motion_Change();
}

void CBPlayer::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	if(m_eCurState != PLAYERSTATE::MOVE_MAKE && m_eCurState != PLAYERSTATE::MOVE_ING && m_eCurState != PLAYERSTATE::MOVE_POP)
	//if (!m_bMoveDelyTime)
	{
		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top,
			(int)m_tInfo.fCX * m_iSkinSceneRender,
			(int)m_tInfo.fCY * m_iSkinSceneRender,
			hMemDC,
			m_tFrame.iFrameStart * 44,
			m_tFrame.iMotion * 44,
			44, // 복사할 비트맵의 가로, 세로 길이
			44,
			RGB(95, 88, 88));	// 제거하고자 하는 색상을 전달
	}
	else
	{
		GdiTransparentBlt(hDC,
			m_tRect.left-23,
			m_tRect.top-25,
			80,
			80,
			hMemDC,
			m_tFrame.iFrameStart * 98,
			m_tFrame.iMotion * 98,
			98, // 복사할 비트맵의 가로, 세로 길이
			98,
			RGB(95, 88, 88));	// 제거하고자 하는 색상을 전달
	}
}

void CBPlayer::Release()
{
}

void CBPlayer::Key_Input()
{
	float	fY(0.f);


	// 낙하하면서 타일에 닿았을때
	if (m_bOnAir && m_bfall && m_DownDelayTime + 200 < GetTickCount64())
	{
		this->Set_Egnore_col(false);
		m_fTime = 0.f;
		m_bJump = false;
		m_bOnAir = false;
		m_DownDelayTime = 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_tInfo.fX -= m_direction.x * m_fSpeed;
		m_eCurState = PLAYERSTATE::WALK_LEFT;
		m_eDir = DIR_LEFT;
		m_bMoveDelyTime = 0;
		
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
		{
			m_bJump = true;
		}
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_tInfo.fX += m_direction.x * m_fSpeed;
		m_eCurState = PLAYERSTATE::WALK_RIGHT;
		m_eDir = DIR_RIGHT;
		m_bMoveDelyTime = 0;

		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
		{
			m_bJump = true;
		}
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		if(m_eDir == DIR_RIGHT)
			m_eCurState = PLAYERSTATE::JUMP_DOWN_RIGHT;
		else
			m_eCurState = PLAYERSTATE::JUMP_DOWN_LEFT;
		
		this->Set_Egnore_col(true);

		m_bOnAir = true;
		m_bfall = true;

		m_DownDelayTime = GetTickCount64();
	}

	else if (CKeyMgr::Get_Instance()->Key_Down('S') )
	{

		CSoundMgr::Get_Instance()->PlaySound(L"Bullet.wav", SOUND_EFFECT, SOUND_VOLUME, true);

		if (m_bElectBubble)
		{
			if (m_eDir == DIR_RIGHT)
				m_eCurState = PLAYERSTATE::BUBBLE_RIGHT;
			else if (m_eDir == DIR_LEFT)
				m_eCurState = PLAYERSTATE::BUBBLE_LEFT;
			
				CObj* pEBubble = CAbstractFactory<CElectricalBubble>::Create(m_tInfo.fX, m_tInfo.fY, m_eDir);
				dynamic_cast<CElectricalBubble*>(pEBubble)->Set_CreateMoveX(m_tInfo.fX);
				CObjMgr::Get_Instance()->Add_Object(ELECTBUBBLE, pEBubble);

				m_bubbleDelyTime = GetTickCount64();
		}
		else
		{
			if (m_eDir == DIR_RIGHT)
				m_eCurState = PLAYERSTATE::BUBBLE_RIGHT;
			else if (m_eDir == DIR_LEFT)
				m_eCurState = PLAYERSTATE::BUBBLE_LEFT;

			if (1 == m_iSkinSceneRender)
			{
				CObj* bubble = CAbstractFactory<CBubble>::Create(m_tInfo.fX, m_tInfo.fY, m_eDir);
				dynamic_cast<CBubble*>(bubble)->Set_CreateMoveX(m_tInfo.fX);
				CObjMgr::Get_Instance()->Add_Object(BUBBLE, bubble);
			}
			m_bubbleDelyTime = GetTickCount64();
		}

		return;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down('X'))
	{
		m_eCurState = PLAYERSTATE::MOVE_MAKE;
		m_bMoveDelyTime = GetTickCount64();
	}

	else if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Jump.wav", SOUND_EFFECT, SOUND_VOLUME, true);
		
		m_bJump = true;
		return;
	}
}

void CBPlayer::Jump()
{

	// 공중에 있을때 중력 작용
	// m_bColTile : tile위에 있는거 말곤 모두 중력작용(true)
	if (!CTileMgr::Get_Instance()->Collision_Tile(this) || Get_ColTile() != COLLSTATE::TOP)
	{
		m_tInfo.fY += m_fGravity;
		m_bOnAir = true;
	}

	if (m_bJump)
	{
		m_tInfo.fY -= (m_fJumpPower * m_fTime) - (m_fGravity * m_fTime * m_fTime * 0.5f);

		if(!m_bBeforeJump)
			m_tInfo.fY -= 4;

		if ((m_fJumpPower * m_fTime) >= (m_fGravity * m_fTime * m_fTime * 0.5f))
		{
			m_bBeforeJump = true;

			if(!m_bChooseSkin)
				this->Set_Egnore_col(true);

			if (m_eDir == DIR_RIGHT)
				m_eCurState = PLAYERSTATE::JUMP_RIGHT;
			else if (m_eDir == DIR_LEFT)
				m_eCurState = PLAYERSTATE::JUMP_LEFT;

			m_bfall = false;
		}
		else
		{
			this->Set_Egnore_col(false);
			if (m_eDir == DIR_RIGHT)
				m_eCurState = PLAYERSTATE::JUMP_DOWN_RIGHT;
			else if (m_eDir == DIR_LEFT)
				m_eCurState = PLAYERSTATE::JUMP_DOWN_LEFT;

			m_bfall = true;		// 낙하중
			m_bBeforeJump = false;
		}

		m_bOnAir = true;
		m_fTime += 0.1f;

		// 낙하하면서 타일에 닿았을때
		if (m_bOnAir && m_bfall || m_bOnAir && Get_ColTile() == COLLSTATE::BOTTOM)
		{
			m_fTime = 0.f;
			m_bJump = false;
			m_bOnAir = false;
			m_bBeforeJump = false;
		}
		// 낙하하면서 타일에 닿았을때
		/*if ()
		{
			m_fTime = 0.f;
			m_bJump = false;
			m_bOnAir = false;
			m_bBeforeJump = false;
			Set_ColTile(COLLSTATE::NONE);
		}*/
	}
}

void CBPlayer::Motion_Change()
{
	m_ePlayerColor = CSceneMgr::Get_Instance()->Get_Player_Color();

	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
					// left //
		case PLAYERSTATE::IDLE_LEFT:
			m_tFrame = { 0, 1, 0, 200, GetTickCount64() };
			m_pFrameKey = L"Player_Left";
			if(m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Left_Red";
			break;

		case PLAYERSTATE::WALK_LEFT:
			m_tFrame = { 0, 3, 1, 100, GetTickCount64() };
			m_pFrameKey = L"Player_Left";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Left_Red";
			break;

		case PLAYERSTATE::JUMP_LEFT:
			m_tFrame = { 0, 1, 2, 100, GetTickCount64() };
			m_pFrameKey = L"Player_Left";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Left_Red";
			break;

		case PLAYERSTATE::JUMP_DOWN_LEFT:
			m_tFrame = { 0, 1, 3, 100, GetTickCount64() };
			m_pFrameKey = L"Player_Left";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Left_Red";
			break;

		case PLAYERSTATE::BUBBLE_LEFT:
			m_tFrame = { 0, 1, 5, 100, GetTickCount64() };
			m_pFrameKey = L"Player_Left";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Left_Red";
			break;

		case PLAYERSTATE::DIE_LEFT:
			m_tFrame = { 0, 3, 13, 100, GetTickCount64() };
			m_pFrameKey = L"Player_Left";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Left_Red";
			break;
		
					// right //
		case PLAYERSTATE::IDLE_RIGHT:
			m_tFrame = { 0, 1, 0, 200, GetTickCount64() };
			m_pFrameKey = L"Player_Right";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Right_Red";
			break;

		case PLAYERSTATE::WALK_RIGHT:
			m_tFrame = { 0, 3, 1, 100, GetTickCount64() };
			m_pFrameKey = L"Player_Right";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Right_Red";
			break;

		case PLAYERSTATE::JUMP_RIGHT:
			m_tFrame = { 0, 1, 2, 100, GetTickCount64() };
			m_pFrameKey = L"Player_Right";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Right_Red";
			break;

		case PLAYERSTATE::JUMP_DOWN_RIGHT:
			m_tFrame = { 0, 1, 3, 100, GetTickCount64() };
			m_pFrameKey = L"Player_Right";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Right_Red";
			break;

		case PLAYERSTATE::BUBBLE_RIGHT:
			m_tFrame = { 0, 1, 5, 100, GetTickCount64() };
			m_pFrameKey = L"Player_Right";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Right_Red";
			break;

		case PLAYERSTATE::DIE_RIGHT:
			m_tFrame = { 0, 3, 13, 100, GetTickCount64() };
			m_pFrameKey = L"Player_Right";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_Right_Red";
			break;

		case PLAYERSTATE::MOVE_MAKE:
			m_tFrame = { 0, 7, 0, 100, GetTickCount64() };
			m_pFrameKey = L"Player_StageMove";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_StageMove_Red";
			break;

		case PLAYERSTATE::MOVE_ING:
			m_tFrame = { 0, 1, 1, 100, GetTickCount64() };
			m_pFrameKey = L"Player_StageMove_new";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_StageMove_new_Red";
			break;

		case PLAYERSTATE::MOVE_POP:
			m_tFrame = { 0, 1, 1, 500, GetTickCount64() };
			m_pFrameKey = L"Player_StageMove";
			if (m_ePlayerColor == PLAYERCOLOR::RED)
				m_pFrameKey = L"Player_StageMove_Red";
			break;
		}

		m_ePreState = m_eCurState;
	}

}

