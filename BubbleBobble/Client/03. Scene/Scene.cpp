#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
	: m_bSceneChangeDelay(false), m_sceneChDelayTime(0), m_pHighScore(nullptr), m_pCurScore(nullptr), m_HurryUpTime(0), m_bHurryCheck(false)
	, m_ExtendStageDelayTime(0)
{
}


CScene::~CScene()
{
}
