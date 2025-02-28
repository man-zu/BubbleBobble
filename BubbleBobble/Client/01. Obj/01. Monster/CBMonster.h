#pragma once
#include "Obj.h"

enum class MONSTERSTATE
{
    WALK_LEFT,
    ANGRY_LEFT,
    WALK_RIGHT,
    ANGRY_RIGHT,
    MONSTER_BUBBLE,
    MONSTER_BUBBLE_HURRY,
    MONSTER_DIE,
    BOSS_IDLE_LEFT,
    BOSS_ANGRY_LEFT,
    BOSS_BUBBLE_LEFT,
    BOSS_DIE_LEFT,
    BOSS_DAMAGE_LEFT,
    BOSS_IDLE_RIGHT,
    BOSS_ANGRY_RIGHT,
    BOSS_BUBBLE_RIGHT,
    BOSS_DIE_RIGHT,
    BOSS_DAMAGE_RIGHT,
    END
};

class CBMonster :
    public CObj
{
public:
    CBMonster();
    virtual ~CBMonster();

    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

    virtual void motion_Change() = 0;
    
    virtual bool Set_Bubble();

    void Set_Hurry();

public:
    MONSTERSTATE    Get_PreState() const { return m_ePreState; }
    MONSTERSTATE    Get_State() const { return m_eCurState; }
    void            Set_PreState(MONSTERSTATE _param) { m_ePreState = _param; }
    void            Set_Hp(float _param) {
        if (m_eDir == DIR_LEFT)
            m_eCurState = MONSTERSTATE::BOSS_DAMAGE_LEFT;
        else if (m_eDir == DIR_RIGHT)
            m_eCurState = MONSTERSTATE::BOSS_DAMAGE_RIGHT;
        m_damageMotionDelyTime = GetTickCount64();
        m_direction = {0, 0};
        m_fHp -= _param; 
    }

protected:
    MONSTERSTATE m_ePreState;
    MONSTERSTATE m_eCurState;

    ULONGLONG   m_bubbleDelayTime;
    ULONGLONG   m_dieDelayTime;

    ULONGLONG   m_HurryDelayTime;
    bool        m_bHurryUp;
    int         m_iFrameChange;

    float       m_fHp;
    ULONGLONG   m_damageMotionDelyTime;
};

