#include "stdafx.h"
#include "CBElectricItem.h"
#include "BmpMgr.h"
#include "TileMgr.h"

CBElectricItem::CBElectricItem()
	:m_fAtt(50.f)
{
}

CBElectricItem::~CBElectricItem()
{
    Release();
}

void CBElectricItem::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Bubble/Lightning_Skill.bmp", L"Lightning_Skill");

    m_tInfo = { 300, 450, 44, 44 };
    m_fSpeed = 5.f;
	m_direction = {1, 1};
	m_eCurItem = ITEMLIST::ELECTRIC;
}

int CBElectricItem::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

	bool colTile = CTileMgr::Get_Instance()->Collision_Tile_Monster(this);
	m_tInfo.fY += m_direction.y * m_fSpeed;
	if (colTile && Get_ColTile() == COLLSTATE::TOP)
		m_direction.y = 0;
    
	__super::Update_Rect();
    return OBJ_NOEVENT;
}

void CBElectricItem::Late_Update()
{
}

void CBElectricItem::Render(HDC hDC)
{
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"Lightning_Skill");

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		44,
		44,
		44, // 복사할 비트맵의 가로, 세로 길이
		44,
		RGB(95, 88, 88));	// 제거하고자 하는 색상을 전달
}

void CBElectricItem::Release()
{
}
