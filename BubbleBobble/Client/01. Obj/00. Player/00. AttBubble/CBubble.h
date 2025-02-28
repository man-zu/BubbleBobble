#pragma once
#include "Obj.h"

enum class BUBBLESTATE { CREATE, IDLE, RED, POP, END };

class CBubble :
    public CObj
{
public:
    CBubble();
    virtual ~CBubble();

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void    Set_CreateMoveX(float& _x) { m_fMoveX = _x + (150 * m_direction.x); }
    void    Set_BubblePop() { m_bDead = true;  m_uDeadDelayTime = GetTickCount64(); m_eCurState = BUBBLESTATE::POP; }
    /*void    Set_ColTile(bool _bool) {m_bColTile = _bool; }*/
    void    Set_ColMonster(bool _bool) { m_bColMonster = _bool; }
    float   Set_BubbleSpeedY() { return (float)(m_direction.y * m_fSpeed * 0.1); }
    void    Motion_Change();
    BUBBLESTATE Get_BubbleState() { return m_eCurState; }

private:
    float       m_fMoveX;
    BUBBLESTATE m_ePreState;
    BUBBLESTATE m_eCurState;
    ULONGLONG   m_uDeadDelayTime;
    bool        m_bColMonster;
    bool        m_bTileCol;
    bool        m_bTileCol2;
};

