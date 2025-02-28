#pragma once
#include "CBItem.h"

enum class EXTENDSTATE
{
    E0,
    X1,
    T2,
    E3,
    N4,
    D5,
    EXTENDEND
};

class CBExtend :
    public CBItem
{
public:
    CBExtend();
    CBExtend(EXTENDSTATE _param);
    virtual ~CBExtend();

    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    EXTENDSTATE     Get_Extend_ID() { return m_eExtend; }
    void            Set_bPlayer_Get(bool _param) { m_bPlayerGet = _param; }
public:
    void            Set_bExtend_Get_All(bool _param) { m_bGetExtendAll = _param; }

private:
    EXTENDSTATE     m_eExtend;
    int             m_iFrame_Start;
    bool            m_bPlayerGet;
    bool            m_bGetExtendAll;
};

