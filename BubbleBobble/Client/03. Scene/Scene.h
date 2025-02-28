#pragma once

#include "Define.h"
#include "Obj.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void	Initialize() PURE;
	virtual void	Update()PURE;
	virtual void	Late_Update()PURE;
	virtual void	Render(HDC hDC)PURE;
	virtual void	Release()PURE;

protected:
	bool			m_bSceneChangeDelay;
	//bool			m_bCallSceneChange;

	ULONGLONG		m_sceneChDelayTime;
	ULONGLONG		m_HurryUpTime;
	ULONGLONG		m_ExtendStageDelayTime;

	CObj*			m_pCurScore;
	CObj*			m_pHighScore;
	bool			m_bHurryCheck;
};

