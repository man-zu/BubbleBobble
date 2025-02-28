#pragma once
#include "Scene.h"
#include "Obj.h"

class SkinChg :
    public CScene
{
public:
    SkinChg();
    virtual ~SkinChg();

    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    CObj* m_player;
};

