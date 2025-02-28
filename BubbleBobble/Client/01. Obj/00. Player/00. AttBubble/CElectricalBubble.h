#pragma once
#include "Obj.h"

enum class ELECTBUBBLESTATE { BUBBLE, BUBBLEPOP, END };

class CElectricalBubble :
    public CObj
{
public:
    CElectricalBubble();
    virtual ~CElectricalBubble();

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void    Set_CreateMoveX(float& _x) { m_fMoveX = _x + (150 * m_direction.x); }
    void    Motion_Change();
    void    Set_BubblePop() { m_bPop = true; m_eCurState = ELECTBUBBLESTATE::BUBBLEPOP; }
    ELECTBUBBLESTATE Get_BubbleState()  { return m_eCurState; }
    float   Get_Att() const             { return m_fAtt; }

private:
    ELECTBUBBLESTATE m_ePreState;
    ELECTBUBBLESTATE m_eCurState;

    float       m_fMoveX;
    bool        m_bPop;
    float       m_fAtt;
};

