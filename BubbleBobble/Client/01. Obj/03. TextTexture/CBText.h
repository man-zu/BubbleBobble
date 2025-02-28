#pragma once
#include "Obj.h"

enum class TEXTTYPE
{
    UP,
    HIGHSCORE,
    NUM,
    HURRYUP,
    BOSSUP,
    NONE
};

class CBText :
    public CObj
{
public:
    CBText();
    virtual ~CBText();

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void Set_Type(TEXTTYPE _param);

private:
    TEXTTYPE    m_eTextType;
};

