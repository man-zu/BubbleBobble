#pragma once
#include "Obj.h"
enum class ITEMLIST
{
    BANANA,
    ORANGE,
    PEACH,
    WATERMELON,
    GRAPE,
    ELECTRIC,
    SHOE,
    BIGCAKE,
    BEER,
    EXTEND
};

class CBItem :
    public CObj
{
public:
    CBItem();
    virtual ~CBItem();

    void Initialize() override;
    int  Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void item_Choose(int _param);
    ITEMLIST Get_CurItem() { return m_eCurItem; }
    void    Set_CurlItem(ITEMLIST _param) { m_eCurItem = _param; }

protected:
    ITEMLIST    m_eCurItem;
    bool        m_bInTile;
};

