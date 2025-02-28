#pragma once
#include "Scene.h"
#include "Obj.h"

class CStage2 :
    public CScene
{
public:
    CStage2();
    virtual ~CStage2();

public:
    // CScene��(��) ���� ��ӵ�
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    CObj* m_player;
};

