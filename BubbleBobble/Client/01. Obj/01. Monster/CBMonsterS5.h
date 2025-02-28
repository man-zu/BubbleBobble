#pragma once
#include "CBMonster.h"
class CBMonsterS5 :
    public CBMonster
{
public:
    CBMonsterS5();
    virtual ~CBMonsterS5();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

