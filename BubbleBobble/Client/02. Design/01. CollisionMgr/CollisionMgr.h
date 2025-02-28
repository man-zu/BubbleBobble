#pragma once

#include "Obj.h"

class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void		Collision_Rect_P_TO_M(list<CObj*> Dst, list<CObj*> Src);
	static void		Collision_Rect_P_TO_I(list<CObj*> Dst, list<CObj*> Src);
	static void		Collision_Rect_P_TO_FB(list<CObj*> Dst, list<CObj*> Src);
	static void		Collision_Rect_FB_TO_M(list<CObj*> Dst, list<CObj*> Src);
	static void		Collision_Sphere(list<CObj*> Dst, list<CObj*> Src);
	static bool		Check_Sphere(CObj* pDst, CObj* pSrc);

	static void		Collision_Rect_Dead(list<CObj*> Dst, list<CObj*> Src, OBJID _id);
	static void		Collision_RectEx(list<CObj*> Dst, list<CObj*> Src);
	static void		Collision_RectObj(CObj* Dst, CObj* Src);
	static bool		Check_Rect(CObj* pDst, CObj* pSrc, float* pX, float *pY);
	static void		Collision_Rect_Bubble_Player(list<CObj*> Dst, list<CObj*> Src);
	static void		Collision_Rect_ElctBubble_Monster(list<CObj*> Dst, list<CObj*> Src);
	static void		Collision_Rect_FireBubble_Monster(list<CObj*> Dst, list<CObj*> Src);
	static void		Collision_Rect_Elect_Bubble_Player(list<CObj*> Dst, list<CObj*> Src);
	static void		Collision_Rect_Bubble_Monster(list<CObj*> Dst, list<CObj*> Src);
};

