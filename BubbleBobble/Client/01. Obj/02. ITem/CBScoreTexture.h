#pragma once
#include "Obj.h"
class CBScoreTexture :
    public CObj
{
public:
    CBScoreTexture();
    CBScoreTexture(int _score);
    virtual ~CBScoreTexture();

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    int             m_iScore; 
    ULONGLONG       m_lDeadDelyTime;
    bool            m_bNoScore;
};

