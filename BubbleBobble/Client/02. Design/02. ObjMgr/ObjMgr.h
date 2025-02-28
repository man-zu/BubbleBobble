#pragma once

#include "AbstractFactory.h"
#include "Mouse.h"
#include "Box.h"
#include "CollisionMgr.h"
#include "TileMgr.h"

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	void		Add_Object(OBJID eID, CObj* pObj);
	int			Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

	void		Delete_ID(OBJID eID);
	void		Set_All_Object_Stop();

public:
	CObj*		Get_Target(OBJID eID, CObj* pObj);
	list<CObj*> Get_Obj(OBJID eID) { return m_ObjList[eID]; }

public:
	static CObjMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;

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


private:
	list<CObj*>		m_ObjList[END];
	static CObjMgr*	m_pInstance;

};

