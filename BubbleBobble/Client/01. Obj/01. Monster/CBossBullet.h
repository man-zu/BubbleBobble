#pragma once
#include "Obj.h"
enum class BBTYPE
{
    BOTTOM,
    MOVE,
    END
};

class CBossBullet :
    public CObj
{
public:
    CBossBullet();
    virtual ~CBossBullet();

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void    Set_Boss_Type(BBTYPE _param) { m_eBossType = _param; }

private:
    BBTYPE  m_eBossType;
};

