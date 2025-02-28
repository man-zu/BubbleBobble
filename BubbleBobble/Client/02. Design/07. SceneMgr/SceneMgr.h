#pragma once

#include "Logo.h"
#include "MyMenu.h"
#include "MyEdit.h"
#include "CGameOver.h"
#include "CStage1.h"
#include "CStage2.h"
#include "CStage3.h"

enum class PLAYERCOLOR { GREEN, RED };

class CSceneMgr
{
public:
	enum SCENEID { SC_LOGO, SC_MENU, SC_SCORE, SC_EDIT, SC_SKIN, SC_STAGE1, SC_STAGE2, SC_STAGE3, SC_BOSSBEFORE, SC_BOSS, SC_EXTEND, SC_GAMEOVER, SC_END };

private:
	CSceneMgr();
	~CSceneMgr();

public:
	void		Scene_Change(SCENEID eScene);
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();
	SCENEID	Get_CurScene() { return m_eCurScene; }
	SCENEID	Get_PreScene() { return m_ePreScene; }

public:
	static CSceneMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;

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
	void			Set_Player_Life(int _param) { m_iPlayerLife = _param; }
	void			Set_Player_Color(PLAYERCOLOR _param) { m_ePlayerColor = _param; }
	int				Get_Player_Life() { return m_iPlayerLife; }
	PLAYERCOLOR		Get_Player_Color() { return m_ePlayerColor; }
	list<CObj*>		Get_Extend_List() { return m_ExtendList; }
	void			Set_Extend_List(CObj* _obj) { m_ExtendList.push_back(_obj); }
	void			Delete_Extend_List() { 
		for(auto pExtend : m_ExtendList)
			Safe_Delete(pExtend);
		m_ExtendList.clear();
		m_ExtendList.resize(0);
	}

	void			Extend_Render(HDC hDC);
	void			Set_Ending(bool _param) { m_bEndingScore = _param; }
	bool			Get_Ending() { return m_bEndingScore; }

private:
	static CSceneMgr*	m_pInstance;
	CScene*				m_pScene;

	SCENEID				m_ePreScene;
	SCENEID				m_eCurScene;

	int					m_iPlayerLife;

	PLAYERCOLOR			m_ePlayerColor;

	list<CObj*>			m_ExtendList;

	bool				m_bEndingScore;
};

