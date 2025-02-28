#pragma once
#include "Scene.h"
#include "Obj.h"
class CStage :	public CScene
{
public:
	CStage();
	virtual ~CStage();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void	Set_Score(int _param) { m_iScore = _param; }
	void	Set_Shoe_Bool() { m_bPlayerShoe = true; }

protected:
	int				m_iScore;
	bool			m_bPlayerShoe;
};

