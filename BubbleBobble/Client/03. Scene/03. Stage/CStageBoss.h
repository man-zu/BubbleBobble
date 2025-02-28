#pragma once
#include "Scene.h"
#include "Obj.h"

class CStageBoss :
    public CScene
{
public:
    CStageBoss();
    virtual ~CStageBoss();

    // CScene을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    void    Key_Input();

private:
    CObj* m_player;
    CObj* pBoss;

    ULONGLONG   m_delayTime;
};
