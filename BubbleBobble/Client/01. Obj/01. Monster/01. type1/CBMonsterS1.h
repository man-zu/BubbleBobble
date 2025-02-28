#pragma once
#include "CBMonster.h"
class CBMonsterS1 :
    public CBMonster
{
public:
    CBMonsterS1();
    virtual ~CBMonsterS1();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void motion_Change() override;

};

