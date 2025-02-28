#pragma once

#include "Define.h"

enum class COLLSTATE
{
	NONE,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	END
};

class CObj
{
public:
	CObj();
	virtual ~CObj();
	
public:
	INFO	const	Get_Info() { return m_tInfo; }
	void			Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}

	void		Set_Direction(DIRECTION eDir) { 
		m_eDir = eDir;
		if (m_eDir == DIR_DOWN)
			m_direction.y = 1;
		else if (m_eDir == DIR_UP)
			m_direction.y = -1;
		else if (m_eDir == DIR_RIGHT)
			m_direction.x = 1;
		else if (m_eDir == DIR_LEFT)
			m_direction.x = -1;
	}
	POINT			Get_DirectionPoint() const		{ return m_direction; }
	RECT			Get_Rect() const				{ return m_tRect; }
	bool			Get_Dead() const				{ return m_bDead; }
	COLLSTATE		Get_ColTile()  const			{ return m_eColTile; }
	COLLSTATE		Get_Col2Tile()  const			{ return m_eCol2Tile; }
	int				Get_Score() const				{ return m_iScore; }
	int				Get_Life() const				{ return m_iLife; }
	
	void			Set_Angle(float _fAngle)		{ m_fAngle = _fAngle; }
	void			Set_Target(CObj* pTarget)		{ m_pTarget = pTarget; }
	void			Set_PosX(float _fX)				{ m_tInfo.fX += _fX; }
	void			Set_PosY(float _fY)				{ m_tInfo.fY += _fY; }
	void			Set_FrameKey(TCHAR* pFrameKey)	{ m_pFrameKey = pFrameKey; }
	void			Set_Dead()						{ m_bDead = true; }
	void			Set_ColTile(COLLSTATE _state)	{ m_eColTile = _state; }
	void			Set_Col2Tile(COLLSTATE _state)	{ m_eCol2Tile = _state; }
	void			Set_Add_Score(int _score)		{ m_iScore += _score; }
	void			Set_Add_Life(int _life)			{ m_iLife += _life; }
	void			Set_Init_Score(int _score)		{ m_iScore = _score; }
	void			Set_Init_Life(int _life)		{ m_iLife = _life; }
	void			Set_ColTileBool(bool _bool)		{ m_bColTile = _bool; }
	void			Set_SpeedUp()					{ m_fSpeed *= 2; }
	void			Set_SpeedDown()					{ m_fSpeed = 4.f; }
	void			Set_Egnore_col(bool _param)		{ m_bEgnoreCol = _param; }
	bool			Get_Egnore_col() const			{ return m_bEgnoreCol; }
	void			Set_Direction_Initize()			{ m_direction = { 0, 0 }; }
	void			Set_Speed_Zero()				{ m_fSpeed = 0; }

	void			Set_Hurry();

public:
	virtual void Initialize()		PURE;
	virtual int	 Update()			PURE;
	virtual void Late_Update()		PURE;
	virtual void Render(HDC hDC)		PURE;
	virtual void Release()			PURE;

protected:
	void		Update_Rect();
	void		Update_InfoY();
	void		Move_Frame();

protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;

	int			m_iScore;
	int         m_iLife;

	POINT		m_direction;

	float		m_fSpeed;
	float		m_fAngle;
	float		m_fDistance;
	float		m_fGravity;

	DIRECTION	m_eDir;
	bool		m_bDead;
	COLLSTATE	m_eColTile;
	COLLSTATE	m_eCol2Tile;
	bool		m_bColTile;
	bool		m_bEgnoreCol;
	bool		m_bSoundDelay;

	CObj*		m_pTarget;
	TCHAR*		m_pFrameKey;

};

