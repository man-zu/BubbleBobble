#pragma once

#include "Tile.h"

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void		Picking_Tile(POINT pt, int _iOption, int _iDrawID);
	void		Save_Tile();
	void		Load_Tile(LPCWSTR fileName);
	bool		Collision_Tile(CObj* Dst);
	bool		Collision_Tile_Monster(CObj* Dst);
	bool		Collision_Tile_Bubble(CObj* Dst);
	INFO		Get_First_Tile() {
		for (auto& tile : m_vecTile)
		{
			if (dynamic_cast<CTile*>(tile)->Get_Option() == 1)
				return tile->Get_Info();
		}
		return {};
	}

	INFO 	Get_Left_Top_Tile() {
		float fLeftX(800.f);		// 작은 숫자 들어가야 됌
		float fLeftY(600.f);		// 큰 숫자 들어가야 됌 

		for (auto& tile : m_vecTile)
		{
			if (dynamic_cast<CTile*>(tile)->Get_Option() == 1)
			{
				fLeftX = fLeftX < tile->Get_Info().fX ? fLeftX : tile->Get_Info().fX;
				fLeftY = fLeftY < tile->Get_Info().fY ? fLeftY : tile->Get_Info().fY;
			}
		}
		return { fLeftX , fLeftY , 0, 0 };
	}
	INFO 	Get_Right_Bottom_Tile() {
		float fLeftX(0.f);		// 작은 숫자 들어가야 됌
		float fLeftY(0.f);		// 큰 숫자 들어가야 됌 

		for (auto& tile : m_vecTile)
		{
			if (dynamic_cast<CTile*>(tile)->Get_Option() == 1)
			{
				fLeftX = fLeftX > tile->Get_Info().fX ? fLeftX : tile->Get_Info().fX;
				fLeftY = fLeftY > tile->Get_Info().fY ? fLeftY : tile->Get_Info().fY;
			}
		}
		return { fLeftX , fLeftY , 0, 0 };
	}

private:
	static CTileMgr*		m_pInstance;
	vector<CObj*>			m_vecTile;
};

