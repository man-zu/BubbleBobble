#pragma once
#include "CBMonster.h"
class CBMonsterS4 :
    public CBMonster
{
public:
    CBMonsterS4();
    virtual ~CBMonsterS4();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

