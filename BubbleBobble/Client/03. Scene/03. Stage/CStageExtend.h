#pragma once
#include "Scene.h"

class CStageExtend :
    public CScene
{
public:
    CStageExtend();
    virtual ~CStageExtend();

    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    CObj* m_player;
    ULONGLONG   m_DelayTime;
    ULONGLONG   m_EndTime;
    ULONGLONG   m_ScreneDelayTime;
    bool        m_DelayBool;
    bool        m_SceneDelayBool;
};

