#pragma once
#include "Scene.h"
class CScore :
    public CScene
{
public:
    CScore();
    virtual ~CScore();

    // CScene을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

