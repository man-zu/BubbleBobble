#pragma once
#include "Scene.h"
class CScore :
    public CScene
{
public:
    CScore();
    virtual ~CScore();

    // CScene��(��) ���� ��ӵ�
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

