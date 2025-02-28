#include "stdafx.h"
#include "ObjMgr.h"
#include "CBItem.h"
#include "CBMonster.h"

CObjMgr*	CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Add_Object(OBJID eID, CObj* pObj)
{
	if ((END <= eID) || (nullptr == pObj))
		return;

	m_ObjList[eID].push_back(pObj);
}

int CObjMgr::Update()
{
	for (size_t i = 0; i < END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			if (i == ITEM)
			{
				if ((*iter)->Get_Score() == 99)
				{

				}
			}

			// 보스 화염병은 y축 WINCY-50 넘어가면 삭제
			if (i == BOSSBULLET)
			{
				if ((*iter)->Get_Info().fY >= WINCY-50)
				{
					Safe_Delete(*iter);
					iter = m_ObjList[i].erase(iter);
					break;
				}
			}

			// 화면 X,Y 밖 넘어가면 삭제
			if ((*iter)->Get_Info().fX <= 10 || (*iter)->Get_Info().fX >= WINCX
				|| (*iter)->Get_Info().fY <= 10 || (*iter)->Get_Info().fY >= WINCY)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
				break;
			}

			int iResult = (*iter)->Update();

			// 죽었을 때 삭제
			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return 0;
}

void CObjMgr::Late_Update()
{
	for (size_t i = 0; i < END; ++i)
	{
		for (auto& iter : m_ObjList[i])
		{
			iter->Late_Update();

			if (m_ObjList[i].empty())
				break;
		}
	}

	CCollisionMgr::Collision_Rect_Bubble_Monster(m_ObjList[MONSTER], m_ObjList[BUBBLE]);
	CCollisionMgr::Collision_Rect_ElctBubble_Monster(m_ObjList[ELECTBUBBLE], m_ObjList[BOSS]);
	CCollisionMgr::Collision_Rect_ElctBubble_Monster(m_ObjList[ELECTBUBBLE], m_ObjList[MONSTER]);
	CCollisionMgr::Collision_Rect_FireBubble_Monster(m_ObjList[FIREBUBBLE], m_ObjList[MONSTER]);
	CCollisionMgr::Collision_Rect_Bubble_Player(m_ObjList[PLAYER], m_ObjList[BUBBLE]);
	CCollisionMgr::Collision_Rect_Elect_Bubble_Player(m_ObjList[PLAYER], m_ObjList[ELECTBUBBLE]);
	CCollisionMgr::Collision_Rect_P_TO_I(m_ObjList[PLAYER], m_ObjList[ITEM]);
	CCollisionMgr::Collision_Rect_P_TO_I(m_ObjList[PLAYER], m_ObjList[EXTEND]);
	CCollisionMgr::Collision_Rect_P_TO_M(m_ObjList[PLAYER], m_ObjList[MONSTER]);
	CCollisionMgr::Collision_Rect_Dead(m_ObjList[PLAYER], m_ObjList[BOSS], BOSS);
	CCollisionMgr::Collision_Rect_Dead(m_ObjList[PLAYER], m_ObjList[BOSSBULLET], BOSSBULLET);
	
	CCollisionMgr::Collision_Rect_P_TO_FB(m_ObjList[PLAYER], m_ObjList[FIREBUBBLE]);
}

void CObjMgr::Render(HDC hDC)
{
	for (size_t i = 0; i < END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Render(hDC);
	}
}

void CObjMgr::Release()
{
	for (size_t i = 0; i < END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObjMgr::Delete_ID(OBJID eID)
{
	for (auto& iter : m_ObjList[eID])
		Safe_Delete(iter);

	m_ObjList[eID].clear();
}

void CObjMgr::Set_All_Object_Stop()
{
	for (size_t i = 0; i < END; ++i)
	{
		for (auto iter : m_ObjList[i])
		{
			//(iter)->Set_Direction_Initize();
			//(iter)->Set_Speed_Zero();
		}
	}
}

CObj * CObjMgr::Get_Target(OBJID eID, CObj * pObj)
{
	if (m_ObjList[eID].empty())
		return nullptr;

	CObj*		pTarget = nullptr;

	float	fDistance(0.f);

	for (auto& iter : m_ObjList[eID])
	{
		if(iter->Get_Dead())
			continue;

		float	fWidth = abs(iter->Get_Info().fX - pObj->Get_Info().fX);
		float	fHeight = abs(iter->Get_Info().fY - pObj->Get_Info().fY);

		float	fDiagonal = (float)sqrt(fWidth * fWidth + fHeight * fHeight);

		if ((!pTarget) || fDistance > fDiagonal)
		{
			pTarget = iter;
			fDistance = fDiagonal;
		}

	}
	
	return pTarget;
}

