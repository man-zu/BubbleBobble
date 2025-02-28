#pragma once
#include "Obj.h"
#include "CBItem.h"

class CBElectricItem :
    public CBItem
{
public:
    CBElectricItem();
    virtual ~CBElectricItem();

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    float       Get_Attack() { return m_fAtt; }

private:
    float       m_fAtt;
};

