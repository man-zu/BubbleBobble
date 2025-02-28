#include "stdafx.h"
#include "Box.h"

CBox::CBox()
{
}

CBox::~CBox()
{
	Release();
}

void CBox::Initialize()
{
	m_tInfo = { WINCX / 2.f, WINCY / 2.f, 200.f, 200.f };
}

int CBox::Update()
{
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBox::Late_Update()
{
}

void CBox::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CBox::Release()
{
}
