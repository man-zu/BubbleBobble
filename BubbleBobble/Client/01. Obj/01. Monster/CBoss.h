#pragma once
#include "CBMonster.h"

class CBoss :
    public CBMonster
{
public:
    CBoss();
    virtual ~CBoss();

    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

    void    motion_Change() override;
    void    Set_LT(INFO _param) { m_lt = _param; }
    void    Set_RB(INFO _param) { m_rb = _param; }
    void    Set_Player_Cpy(INFO _param) { m_playerCpyInfo = _param; }
    void    Set_Hp(float _param)
    {
        __super::Set_Hp(_param); 
    }
    

private:
    void    pattern1();
    void    pattern2();
    void    pattern3();


private:
    //MONSTERSTATE m_ePreState;
    //MONSTERSTATE m_eCurState;

    INFO    m_lt;
    INFO    m_rb;

    POINT   m_BeforeDirection;

    ULONGLONG   m_angryDelyTime;
    ULONGLONG   m_patternDelyTime;
    INFO        m_playerCpyInfo;
    int         m_iPattern1Cnt;

};

