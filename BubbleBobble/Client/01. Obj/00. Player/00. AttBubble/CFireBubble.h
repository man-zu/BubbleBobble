#pragma once
#include "Obj.h"

enum class FIRESTATE
{
    CREATE,
    POP,
    FIRE
};

class CFireBubble :
    public CObj
{
public:
    CFireBubble();
    virtual ~CFireBubble();

    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    FIRESTATE Get_Cur_State() { return m_eCurState; }
    void Set_Cur_State(FIRESTATE _param) { m_eCurState = _param; }
    float   Get_Att() const { return m_fAtt; }

private:
    FIRESTATE m_eCurState;
    FIRESTATE m_epPreState;
    float m_fAtt;
};

