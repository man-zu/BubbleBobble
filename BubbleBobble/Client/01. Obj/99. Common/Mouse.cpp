#include "stdafx.h"
#include "Mouse.h"
#include "BmpMgr.h"

CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/mouse_pointer.bmp", L"mouse_pointer");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/mouse_cuser.bmp", L"mouse_cuser");
	
	m_tFrame = { 0, 1, 0, 200, GetTickCount64() };

	m_tInfo.fCX = 44;
	m_tInfo.fCY = 44;
}

int CMouse::Update()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);

	// 전체 스크린 영역에서 클라이언트 좌표로 변환
	ScreenToClient(g_hWnd, &ptMouse);
	
	m_tInfo.fX = (float)ptMouse.x;
	m_tInfo.fY = (float)ptMouse.y;

	__super::Update_Rect();

	ShowCursor(false);	// 마우스 커서의 출력 유무를 결정

	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{
	Move_Frame();
}

void CMouse::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"mouse_cuser");

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0, //m_tFrame.iFrameStart * 44,
		0, //m_tFrame.iMotion * 44,
		19, // 복사할 비트맵의 가로, 세로 길이
		19,
		RGB(255, 255, 255));	// 제거하고자 하는 색상을 전달
		

	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"mouse_pointer");

	//GdiTransparentBlt(hDC,
	//	m_tRect.left,
	//	m_tRect.top,
	//	44,
	//	44,
	//	hMemDC,
	//	0, //m_tFrame.iFrameStart * 44,
	//	0, //m_tFrame.iMotion * 44,
	//	44, // 복사할 비트맵의 가로, 세로 길이
	//	44,
	//	RGB(96, 96, 64));	// 제거하고자 하는 색상을 전달
}

void CMouse::Release()
{
}
