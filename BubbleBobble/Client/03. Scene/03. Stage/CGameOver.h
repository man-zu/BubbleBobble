#pragma once
#include "Scene.h"
class CGameOver :
    public CScene
{
public:
    CGameOver();
    virtual ~CGameOver();

    // CScene을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    FRAME m_tFrame;
};

