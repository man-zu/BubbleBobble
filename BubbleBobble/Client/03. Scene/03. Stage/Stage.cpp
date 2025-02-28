#include "stdafx.h"
#include "Stage.h"

CStage::CStage()
	: m_iScore(0), m_bPlayerShoe(false)
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	/*CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Tile.bmp", L"Tile");

	CObjMgr::Get_Instance()->Add_Object(PLAYER, CAbstractFactory<CPlayer>::Create());*/

}

void CStage::Update()
{
	//CObjMgr::Get_Instance()->Update();
	//CTileMgr::Get_Instance()->Update();
}

void CStage::Late_Update()
{
	//CObjMgr::Get_Instance()->Late_Update();
	//CTileMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC hDC)
{
	//int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//CTileMgr::Get_Instance()->Render(hDC);

	//CObjMgr::Get_Instance()->Render(hDC);
}

void CStage::Release()
{
}
