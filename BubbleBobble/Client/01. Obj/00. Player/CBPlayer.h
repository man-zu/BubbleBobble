#pragma once
#include "Obj.h"
#include "SceneMgr.h"

enum class PLAYERSTATE
{
    IDLE_LEFT,
    WALK_LEFT,
    JUMP_LEFT,
    JUMP_DOWN_LEFT,
    BUBBLE_LEFT,
    DIE_LEFT,
    IDLE_RIGHT,
    WALK_RIGHT,
    JUMP_RIGHT,
    JUMP_DOWN_RIGHT,
    BUBBLE_RIGHT,
    DIE_RIGHT,
    MOVE_MAKE,
    MOVE_ING,
    MOVE_POP,
    END
};

class CBPlayer :
    public CObj
{

public:
    CBPlayer();
    virtual ~CBPlayer();

    void		Initialize() override;
    int		Update() override;
    void		Late_Update() override;
    void		Render(HDC hDC) override;
    void		Release() override;

public:
	void			Key_Input();
	void			Jump();
	void			Motion_Change();
    void        Set_Jump_True() { m_bJump = true; }
    void        Set_ElectBubble(bool _param) { m_bElectBubble = _param; }
    PLAYERSTATE Get_State() { return m_ePreState; }
    void        Set_SkinRender() { m_iSkinSceneRender = 5; }
    void        Set_ChooseSkin() { m_bChooseSkin = true; }
    

private:
	bool		m_bJump;		// 점프 상태 확인	
	float		m_fJumpPower;	// 힘의 크기
	float		m_fTime;		// 점프 중 진행 시간 

    PLAYERSTATE m_ePreState;
    PLAYERSTATE m_eCurState;
    PLAYERCOLOR m_ePlayerColor;

    ULONGLONG   m_bubbleDelyTime;
    ULONGLONG   m_deadDelyTime;
    bool        m_bDeadDely;
    ULONGLONG   m_bCreateNewDelayTime;
    ULONGLONG   m_bMoveDelyTime;

    // 포물선운동 낙하상태인지 계산용도 ///
    bool        m_bfall;
    bool        m_bOnAir;
    bool        m_bElectBubble;
    bool        m_bBeforeJump = false;
    bool        m_bChooseSkin = false;
    ULONGLONG   m_DownDelayTime;

    int         m_iSkinSceneRender;
};

