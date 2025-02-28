#pragma once
#include "CBMonster.h"
class CBMonsterS3 :
    public CBMonster
{
public:
    CBMonsterS3();
    virtual ~CBMonsterS3();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void motion_Change() override;

private:
    ULONGLONG m_fYDelayTime;
};

