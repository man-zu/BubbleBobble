#pragma once

#include "Line.h"

class CLineMgr
{
private:
	CLineMgr();

	~CLineMgr();

public:
	void		Initialize();
	void		Render(HDC hDC);
	void		Release();

public:
	bool		Collision_Line(float _fX, float *_pY);
	void		Load_Data();
	void		InputLine(LINEPOINT leftLine, LINEPOINT rightLine);

public:
	static CLineMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CLineMgr;

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
	static CLineMgr*		m_pInstance;

	list<CLine*>			m_LineList;

};


