#pragma once
#include "CBMonster.h"
class CBMonsterS2 :
    public CBMonster
{
public:
    CBMonsterS2();
    CBMonsterS2(float x, float y);
    virtual ~CBMonsterS2();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void motion_Change() override;
    void jump();

private:
    float m_fTime;
};

