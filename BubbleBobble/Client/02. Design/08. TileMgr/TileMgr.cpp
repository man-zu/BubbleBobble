#include "stdafx.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"
#include "Tile.h"
#include "CBubble.h"
#include "Scene.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = float((TILECX >> 1) + (TILECX * j));
			float fY = float((TILECY >> 1) + (TILECY * i));

			CObj* pObj = CAbstractFactory<CTile>::Create(fX, fY);
			m_vecTile.push_back(pObj);
		}
	}
}

void CTileMgr::Update()
{
	for (auto& iter : m_vecTile)
		iter->Update();
}

void CTileMgr::Late_Update()
{
	for (auto& iter : m_vecTile)
		iter->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	
	int iCullX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int iCullY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int iMaxX = iCullX + WINCX / TILECX + 2;
	int iMaxY = iCullY + WINCY / TILECY + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			int iIndex = i * TILEX + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}
	/*for (int i = 0; i < 27; ++i)
	{
		for (int j = 0; j < 35; ++j)
		{
			int iIndex = i * TILEX + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}*/
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTileMgr::Picking_Tile(POINT pt, int _iOption, int _iDrawID)
{
	int		x = pt.x / TILECX;
	int		y = pt.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_TileOption(_iDrawID, _iOption);
}

void CTileMgr::Save_Tile()
{
	HANDLE		hFile = CreateFile(L"../Data/Tile.dat", // 파일 경로와 이름을 명시
		GENERIC_WRITE, // 파일 접근 모드(GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL, // 공유 방식, 파일이 개방된 상태에서 다른 프로세스가 파일 개방을 하는 것을 허가할 것인가(NULL 공유하지 않음)
		NULL, // 보안 속성, NULL 인 경우 기본 보안 상태
		CREATE_ALWAYS,	// 생성 방식, (CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 열기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성(읽기 전용, 숨김 파일 등등),FILE_ATTRIBUTE_NORMAL : 특수 속성이 없는 일반 파일 형태 
		NULL); // 생성될 파일의 속성을 제공할 템플릿 파일 ( 사용하지 않기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OKCANCEL);
		return;
	}

	int		iDrawID(0), iOption(0);
	DWORD	dwByte(0);


	for (auto& iter : m_vecTile)
	{
		iDrawID = dynamic_cast<CTile*>(iter)->Get_DrawID();
		iOption = dynamic_cast<CTile*>(iter)->Get_Option();

		WriteFile(hFile, &(iter->Get_Info()), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, _T("Save 성공"), L"성공", MB_OKCANCEL);
}

void CTileMgr::Load_Tile(LPCWSTR fileName)
{
	HANDLE		hFile = CreateFile(fileName, // 파일 경로와 이름을 명시
		GENERIC_READ, // 파일 접근 모드(GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL, // 공유 방식, 파일이 개방된 상태에서 다른 프로세스가 파일 개방을 하는 것을 허가할 것인가(NULL 공유하지 않음)
		NULL, // 보안 속성, NULL 인 경우 기본 보안 상태
		OPEN_EXISTING,	// 생성 방식, (CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 열기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성(읽기 전용, 숨김 파일 등등),FILE_ATTRIBUTE_NORMAL : 특수 속성이 없는 일반 파일 형태 
		NULL); // 생성될 파일의 속성을 제공할 템플릿 파일 ( 사용하지 않기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OKCANCEL);
		return;
	}

	INFO	tInfo{};
	int		iDrawID(0), iOption(0);
	DWORD	dwByte(0);

	Release();

	while (true)
	{
		bool t1 = ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, NULL);
		bool t2 = ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
		bool t3 = ReadFile(hFile, &iOption, sizeof(int), &dwByte, NULL);

		if (0 == dwByte)
			break;

		CObj* pTile = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		dynamic_cast<CTile*>(pTile)->Set_TileOption(iDrawID, iOption);

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, _T("Load 성공"), L"성공", MB_OKCANCEL);
}

bool CTileMgr::Collision_Tile(CObj* Dst)
{
	float	fX(0.f), fY(0.f);
	bool bCol(false);
	Dst->Set_ColTile(COLLSTATE::NONE);

	if (!Dst->Get_Egnore_col())		//true / false
	{
		for (auto& Sour : m_vecTile)
		{
			if (CCollisionMgr::Check_Rect(Dst, Sour, &fX, &fY))
			{
				if (0 != dynamic_cast<CTile*>(Sour)->Get_Option())
				{
					// 상하 충돌
					if (fX > fY)
					{
						// 상 충돌
						if (Dst->Get_Info().fY < Sour->Get_Info().fY)
						{
							Dst->Set_PosY(-fY);
							Dst->Set_ColTile(COLLSTATE::TOP);
							bCol = true;
						}

						// 하 충돌
						else
						{
							Dst->Set_PosY(fY);
							Dst->Set_ColTile(COLLSTATE::BOTTOM);
							bCol = true;
						}
					}

					// 좌우 충돌
					else
					{
						// 좌 충돌
						if (Dst->Get_Info().fX < Sour->Get_Info().fX)
						{
							Dst->Set_PosX(-fX);
							Dst->Set_Col2Tile(COLLSTATE::LEFT);
							bCol = true;
						}

						// 우 충돌
						else
						{
							Dst->Set_PosX(fX);
							Dst->Set_Col2Tile(COLLSTATE::RIGHT);
							bCol = true;
						}
					}
				}
			}
		}
	}
	return bCol;
}



bool CTileMgr::Collision_Tile_Monster(CObj* Dst)
{
	float	fX(0.f), fY(0.f);
	bool bCol(false);
	Dst->Set_ColTile(COLLSTATE::NONE);

	for (auto& Sour : m_vecTile)
	{
		if (CCollisionMgr::Check_Rect(Dst, Sour, &fX, &fY))
		{
			if (0 != dynamic_cast<CTile*>(Sour)->Get_Option())
			{
				// 상하 충돌
				if (fX >= fY)
				{
					// 상 충돌
					if (Dst->Get_Info().fY < Sour->Get_Info().fY)
					{
						Dst->Set_PosY(-fY);
						Dst->Set_ColTile(COLLSTATE::TOP);
						bCol = true;
					}

					// 하 충돌
					else
					{
						Dst->Set_PosY(fY);
						Dst->Set_ColTile(COLLSTATE::BOTTOM);
						bCol = true;
					}
				}

				// 좌우 충돌
				else
				{
					// 좌 충돌
					if (Dst->Get_Info().fX < Sour->Get_Info().fX)
					{
						Dst->Set_PosX(-fX);
						Dst->Set_Direction(DIR_LEFT);
						bCol = true;
					}

					// 우 충돌
					else
					{
						Dst->Set_PosX(fX);
						Dst->Set_Direction(DIR_RIGHT);
						bCol = true;
					}
				}
			}
		}
	}
	return bCol;
}


bool CTileMgr::Collision_Tile_Bubble(CObj* Dst)
{
	float	fX(0.f), fY(0.f);
	bool bCol(false);
	Dst->Set_ColTile(COLLSTATE::NONE);

	for (auto& Sour : m_vecTile)
	{
		if (CCollisionMgr::Check_Rect(Dst, Sour, &fX, &fY))
		{
			if (0 != dynamic_cast<CTile*>(Sour)->Get_Option())
			{
				// 상하 충돌
				//if (fX > fY)
				//{
				//	// 상 충돌
				//	if (Dst->Get_Info().fY < Sour->Get_Info().fY)
				//	{
				//		Dst->Set_PosY(-fY);
				//		Dst->Set_ColTile(COLLSTATE::TOP);
				//		bCol = true;
				//	}

				//	// 하 충돌
				//	else
				//	{
				//		Dst->Set_PosY(fY);
				//		Dst->Set_ColTile(COLLSTATE::BOTTOM);
				//		bCol = true;
				//	}
				//}

				// 좌우 충돌
				// 좌 충돌
				if (Dst->Get_Info().fX < Sour->Get_Info().fX)
				{
					//Dst->Set_PosX(-fX);
					Dst->Set_ColTileBool(true);
					Dst->Set_ColTile(COLLSTATE::LEFT);
					//dynamic_cast<CBubble*>(Dst)->Set_ColTile(true);
					bCol = true;
				}

				// 우 충돌
				else
				{
					//Dst->Set_PosX(fX);
					Dst->Set_ColTileBool(true);
					Dst->Set_ColTile(COLLSTATE::RIGHT);
					//dynamic_cast<CBubble*>(Dst)->Set_ColTile(true);
					bCol = true;
				}
			}
		}
	}
	return bCol;
}