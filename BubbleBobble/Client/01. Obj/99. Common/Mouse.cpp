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

	// ��ü ��ũ�� �������� Ŭ���̾�Ʈ ��ǥ�� ��ȯ
	ScreenToClient(g_hWnd, &ptMouse);
	
	m_tInfo.fX = (float)ptMouse.x;
	m_tInfo.fY = (float)ptMouse.y;

	__super::Update_Rect();

	ShowCursor(false);	// ���콺 Ŀ���� ��� ������ ����

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
		19, // ������ ��Ʈ���� ����, ���� ����
		19,
		RGB(255, 255, 255));	// �����ϰ��� �ϴ� ������ ����
		

	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"mouse_pointer");

	//GdiTransparentBlt(hDC,
	//	m_tRect.left,
	//	m_tRect.top,
	//	44,
	//	44,
	//	hMemDC,
	//	0, //m_tFrame.iFrameStart * 44,
	//	0, //m_tFrame.iMotion * 44,
	//	44, // ������ ��Ʈ���� ����, ���� ����
	//	44,
	//	RGB(96, 96, 64));	// �����ϰ��� �ϴ� ������ ����
}

void CMouse::Release()
{
}
