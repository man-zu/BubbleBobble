#pragma once
#include "Scene.h"
class CBeforeScene :
    public CScene
{
public:
    CBeforeScene();
    virtual ~CBeforeScene();

    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

};

