#include "stdafx.h"
#include "CollisionMgr.h"
#include "CBubble.h"
#include "CElectricalBubble.h"
#include "CBPlayer.h"
#include "CBItem.h"
#include "CFireBubble.h"
#include "CBMonster.h"
#include "CBoss.h"
#include "CFireBubble.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect_P_TO_M(list<CObj*> Dst, list<CObj*> Src)
{
	RECT	rc{};
	for (auto& Dest : Dst)
	{
		PLAYERSTATE ePState = dynamic_cast<CBPlayer*>(Dest)->Get_State();
		if (ePState != PLAYERSTATE::MOVE_ING && ePState != PLAYERSTATE::MOVE_POP)
		{
			for (auto& Sour : Src)
			{
				if (IntersectRect(&rc, &(Dest->Get_Rect()), &(Sour->Get_Rect())))
				{
					MONSTERSTATE monsterState = dynamic_cast<CBMonster*>(Sour)->Get_PreState();

					if (monsterState == MONSTERSTATE::MONSTER_BUBBLE || monsterState == MONSTERSTATE::MONSTER_BUBBLE_HURRY || monsterState == MONSTERSTATE::MONSTER_DIE)
						Sour->Set_Dead();
					else
						Dest->Set_Dead();
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Rect_P_TO_I(list<CObj*> Dst, list<CObj*> Src)
{
	RECT	rc{};

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (IntersectRect(&rc, &(Dest->Get_Rect()), &(Sour->Get_Rect())))
			{
				Dest->Set_Add_Score(Sour->Get_Score());
				Sour->Set_Dead();

				if (dynamic_cast<CBItem*>(Sour)->Get_CurItem() == ITEMLIST::ELECTRIC)
					dynamic_cast<CBPlayer*>(Dest)->Set_ElectBubble(true);
			}
		}
	}
}

void CCollisionMgr::Collision_Rect_P_TO_FB(list<CObj*> Dst, list<CObj*> Src)
{
	RECT	rc{};

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (IntersectRect(&rc, &(Dest->Get_Rect()), &(Sour->Get_Rect())))
			{
				//Sour->Set_Dead();

				if (dynamic_cast<CFireBubble*>(Sour)->Get_Cur_State() == FIRESTATE::CREATE)
					dynamic_cast<CFireBubble*>(Sour)->Set_Cur_State(FIRESTATE::POP);
			}
		}
	}
}

void CCollisionMgr::Collision_Rect_FB_TO_M(list<CObj*> Dst, list<CObj*> Src)
{
	RECT	rc{};

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (IntersectRect(&rc, &(Dest->Get_Rect()), &(Sour->Get_Rect())))
			{
				if (dynamic_cast<CFireBubble*>(Dest)->Get_Cur_State() == FIRESTATE::FIRE)
					Sour->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere(list<CObj*> Dst, list<CObj*> Src)
{
	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (Check_Sphere(Dest, Sour))
			{
				Dest->Set_Dead();
				Sour->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CObj * pDst, CObj * pSrc)
{
	float	fWidth  = abs(pDst->Get_Info().fX - pSrc->Get_Info().fX);
	float	fHeight = abs(pDst->Get_Info().fY - pSrc->Get_Info().fY);

	float	fDiagonal = sqrt(fWidth * fWidth + fHeight * fHeight);

	float	fRadius = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;
	
	return	fRadius >= fDiagonal;
}


void CCollisionMgr::Collision_Rect_Dead(list<CObj*> Dst, list<CObj*> Src, OBJID _id)
{
	float	fX(0.f), fY(0.f);

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if(_id == BOSS/* 보스가 버블상태라면*/)
			{

				if (dynamic_cast<CBMonster*>(Sour)->Get_State() == MONSTERSTATE::BOSS_BUBBLE_LEFT
					|| dynamic_cast<CBMonster*>(Sour)->Get_State() == MONSTERSTATE::BOSS_BUBBLE_RIGHT)
				{
					if (Check_Rect(Dest, Sour, &fX, &fY))
					{
						Sour->Set_Dead();
					}
				}
			}
			else if (dynamic_cast<CBPlayer*>(Dest)->Get_State() != PLAYERSTATE::MOVE_ING && dynamic_cast<CBPlayer*>(Dest)->Get_State() != PLAYERSTATE::MOVE_POP)
			{
				if (Check_Rect(Dest, Sour, &fX, &fY))
				{
					Dest->Set_Dead();
				}
			}
		}
	}
}

void CCollisionMgr::Collision_RectEx(list<CObj*> Dst, list<CObj*> Src)
{
	float	fX(0.f), fY(0.f);

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (Check_Rect(Dest, Sour, &fX, &fY))
			{
				// 상하 충돌
				if (fX > fY)
				{
					// 상 충돌
					if (Dest->Get_Info().fY < Sour->Get_Info().fY)
					{
						Dest->Set_PosY(-fY);
					}

					// 하 충돌
					else
					{
						Dest->Set_PosY(fY);
					}
				}

				// 좌우 충돌
				else
				{
					// 좌 충돌
					if (Dest->Get_Info().fX < Sour->Get_Info().fX)
					{
						Dest->Set_PosX(-fX);
					}

					// 우 충돌
					else
					{
						Dest->Set_PosX(fX);
					}
				}
			}
		}
	}
}

void CCollisionMgr::Collision_RectObj(CObj* Dst, CObj* Src)
{
	float	fX(0.f), fY(0.f);

	if (Check_Rect(Dst, Src, &fX, &fY))
	{
		// 상하 충돌
		if (fX > fY)
		{
			// 상 충돌
			if (Dst->Get_Info().fY < Src->Get_Info().fY)
			{
				Dst->Set_PosY(-fY);
		}

		// 하 충돌
		else
		{
			Dst->Set_PosY(fY);
		}
	}

	// 좌우 충돌
	else
	{
		// 좌 충돌
		if (Dst->Get_Info().fX < Src->Get_Info().fX)
		{
			Dst->Set_PosX(-fX);
		}

		// 우 충돌
		else
		{
			Dst->Set_PosX(fX);
		}
	}
}
}

bool CCollisionMgr::Check_Rect(CObj * pDst, CObj * pSrc, float* pX, float *pY)
{
	float	fWidth = abs(pDst->Get_Info().fX - pSrc->Get_Info().fX);
	float	fHeight = abs(pDst->Get_Info().fY - pSrc->Get_Info().fY);
	
	float fRadiusX = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;
	float fRadiusY = (pDst->Get_Info().fCY + pSrc->Get_Info().fCY) * 0.5f;

	if ((fRadiusX >= fWidth) && (fRadiusY >= fHeight))
	{
		*pX = fRadiusX - fWidth;
		*pY = fRadiusY - fHeight;

		return true;
	}
	
	return false;
}

void CCollisionMgr::Collision_Rect_Bubble_Player(list<CObj*> Dst, list<CObj*> Src)
{
	float	fX(0.f), fY(0.f);
	bool fCol(false);

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (Check_Rect(Dest, Sour, &fX, &fY))
			{
				if (dynamic_cast<CBubble*>(Sour)->Get_BubbleState() != BUBBLESTATE::POP)
				{
					// 상하 충돌
					if (fX > fY)
					{
						// 상 충돌
						if (Dest->Get_Info().fY < Sour->Get_Info().fY)
						{
							Dest->Set_PosY(-fY);
							dynamic_cast<CBPlayer*>(Dest)->Set_Jump_True();
							fCol = true;
						}

						// 하 충돌
						else
						{
							dynamic_cast<CBubble*>(Sour)->Set_BubblePop();
							fCol = true;
							Dest->Set_Add_Score(50);
						}
					}

					// 좌우 충돌
					else
					{
						/*if (fY > 10)
						{
							dynamic_cast<CBPlayer*>(Dest)->Set_Jump_True();
							fCol = true;
						}
						else*/
						// 좌 충돌
						if (Dest->Get_Info().fX < Sour->Get_Info().fX)
						{
							dynamic_cast<CBubble*>(Sour)->Set_BubblePop();
							fCol = true;
							Dest->Set_Add_Score(50);
						}

						// 우 충돌
						else
						{
							dynamic_cast<CBubble*>(Sour)->Set_BubblePop();
							fCol = true;
							Dest->Set_Add_Score(50);
						}
					}
				}
			}
		}
		//if (fCol)
		//	Dest->Set_Add_Score(50);
	}
}

void CCollisionMgr::Collision_Rect_Elect_Bubble_Player(list<CObj*> Dst, list<CObj*> Src)
{
	float	fX(0.f), fY(0.f);
	bool fCol(false);

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (Check_Rect(Dest, Sour, &fX, &fY))
			{
				//
				if (dynamic_cast<CElectricalBubble*>(Sour)->Get_BubbleState() == ELECTBUBBLESTATE::BUBBLE)
				{
					// 상하 충돌
					if (fX > fY)
					{
						// 상 충돌
						if (Dest->Get_Info().fY < Sour->Get_Info().fY)
						{
							Dest->Set_PosY(-fY);
							dynamic_cast<CBPlayer*>(Dest)->Set_Jump_True();
							fCol = true;
						}

						// 하 충돌
						else
						{
							dynamic_cast<CElectricalBubble*>(Sour)->Set_BubblePop();
							fCol = true;
							Dest->Set_Add_Score(50);
							//Sour->Set_Dead();
						}
					}

					// 좌우 충돌
					else
					{
						/*if (fY > 10)
						{
							dynamic_cast<CBPlayer*>(Dest)->Set_Jump_True();
							fCol = true;
						}
						else*/
						// 좌 충돌
						if (Dest->Get_Info().fX < Sour->Get_Info().fX)
						{
							dynamic_cast<CElectricalBubble*>(Sour)->Set_BubblePop();
							fCol = true;
							//Sour->Set_Dead();
							Dest->Set_Add_Score(50);
						}

						// 우 충돌
						else
						{
							dynamic_cast<CElectricalBubble*>(Sour)->Set_BubblePop();
							fCol = true;
							//Sour->Set_Dead();
							Dest->Set_Add_Score(50);
						}
					}
				}
			}
		}
		if (fCol)
		{
		}
	}
}

void CCollisionMgr::Collision_Rect_Bubble_Monster(list<CObj*> Dst, list<CObj*> Src)
{
	float	fX(0.f), fY(0.f);

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (Check_Rect(Dest, Sour, &fX, &fY))
			{
				if (dynamic_cast<CBubble*>(Sour)->Get_BubbleState() == BUBBLESTATE::CREATE)
				{
					// 좌우 충돌
					// 좌 충돌
					if (Dest->Get_Info().fX < Sour->Get_Info().fX)
					{
						if (dynamic_cast<CBMonster*>(Dest)->Set_Bubble())
							dynamic_cast<CBubble*>(Sour)->Set_ColMonster(true);
					}

					// 우 충돌
					else
					{
						if (dynamic_cast<CBMonster*>(Dest)->Set_Bubble())
							dynamic_cast<CBubble*>(Sour)->Set_ColMonster(true);
					}
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Rect_ElctBubble_Monster(list<CObj*> Dst, list<CObj*> Src)
{
	float	fX(0.f), fY(0.f);

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (Check_Rect(Dest, Sour, &fX, &fY))
			{
				if (ELECTBUBBLESTATE::BUBBLEPOP == dynamic_cast<CElectricalBubble*>(Dest)->Get_BubbleState())
				{
					// 번개 사라지고 몬스터 타격
					Dest->Set_Dead();
					dynamic_cast<CBMonster*>(Sour)->Set_Hp(dynamic_cast<CElectricalBubble*>(Dest)->Get_Att());
				}
			}
		}
	}
}


void CCollisionMgr::Collision_Rect_FireBubble_Monster(list<CObj*> Dst, list<CObj*> Src)
{
	float	fX(0.f), fY(0.f);

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (Check_Rect(Dest, Sour, &fX, &fY))
			{
				if (FIRESTATE::POP == dynamic_cast<CFireBubble*>(Dest)->Get_Cur_State())
				{
					// 번개 사라지고 몬스터 타격
					Dest->Set_Dead();
					dynamic_cast<CBMonster*>(Sour)->Set_Hp(dynamic_cast<CFireBubble*>(Dest)->Get_Att());
				}
			}
		}
	}
}