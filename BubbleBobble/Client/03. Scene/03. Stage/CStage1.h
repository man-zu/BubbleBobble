#pragma once
#include "Scene.h"
#include "Obj.h"

class CStage1 :
    public CScene
{
public:
    CStage1();
    virtual ~CStage1();
public:
    // CScene을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    CObj* m_player;
};

