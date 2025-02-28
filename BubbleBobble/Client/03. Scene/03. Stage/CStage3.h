#pragma once
#include "Scene.h"
#include "Obj.h"

class CStage3 :
    public CScene
{
public:
    CStage3();
    virtual ~CStage3();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    CObj* m_player;
    ULONGLONG       m_fireDelayTime;
    bool            m_bFireCreate;
};

