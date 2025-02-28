#include "stdafx.h"
#include "CScoreMgr.h"
#include "SceneMgr.h"

CScoreMgr* CScoreMgr::m_pInstance = nullptr;


CScoreMgr::CScoreMgr()
	:m_iSceneScore(0), m_iHighScore(0)
{
}

CScoreMgr::~CScoreMgr()
{
	Release();
}

void CScoreMgr::Release()
{
}

void CScoreMgr::SaveScoreData()
{
	//m_iScoreList.push_back(10);
	//m_iScoreList.push_back(20);
	//m_iScoreList.push_back(30);

	/*if (m_iScoreList[0] <= CSceneMgr::Get_Instance()->Get_Scene_Score())
	{
		vector<int>::iterator iter= m_iScoreList.begin();
		iter = m_iScoreList.insert(iter, CSceneMgr::Get_Instance()->Get_Scene_Score());
	}*/

	for (auto score : m_iScoreList)
	{
		vector<int>::iterator iter = m_iScoreList.begin();
		if (score < m_iSceneScore)
		{
			iter = m_iScoreList.insert(iter, m_iSceneScore);
			break;
		}
	}

	sort(m_iScoreList.begin(), m_iScoreList.end(), greater<int>());

	HANDLE		hFile = CreateFile(L"../Data/Score/BubbleBobbleScore.dat", // 파일 경로와 이름을 명시
		GENERIC_WRITE, // 파일 접근 모드(GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL, // 공유 방식, 파일이 개방된 상태에서 다른 프로세스가 파일 개방을 하는 것을 허가할 것인가(NULL 공유하지 않음)
		NULL, // 보안 속성, NULL 인 경우 기본 보안 상태
		CREATE_ALWAYS,	// 생성 방식, (CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 열기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성(읽기 전용, 숨김 파일 등등),FILE_ATTRIBUTE_NORMAL : 특수 속성이 없는 일반 파일 형태 
		NULL); // 생성될 파일의 속성을 제공할 템플릿 파일 ( 사용하지 않기 때문에 NULL)

	/*if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OKCANCEL);
		return;
	}*/

	int		iDrawID(0), iOption(0);
	DWORD	dwByte(0);

	int tmp = size(m_iScoreList);
	int tmp2 = sizeof(m_iScoreList);
	
	int iMax(m_iScoreList.size());
	iMax = iMax > 10 ? 10 : iMax;

	for (int i=0;i< iMax;++i)
	{
		WriteFile(hFile, &m_iScoreList[i], sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);
}

void CScoreMgr::LoadScoreData()
{
	HANDLE		hFile = CreateFile(L"../Data/Score/BubbleBobbleScore.dat", // 파일 경로와 이름을 명시
		GENERIC_READ, // 파일 접근 모드(GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL, // 공유 방식, 파일이 개방된 상태에서 다른 프로세스가 파일 개방을 하는 것을 허가할 것인가(NULL 공유하지 않음)
		NULL, // 보안 속성, NULL 인 경우 기본 보안 상태
		OPEN_EXISTING,	// 생성 방식, (CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 열기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성(읽기 전용, 숨김 파일 등등),FILE_ATTRIBUTE_NORMAL : 특수 속성이 없는 일반 파일 형태 
		NULL); // 생성될 파일의 속성을 제공할 템플릿 파일 ( 사용하지 않기 때문에 NULL)

	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OKCANCEL);
	//	return;
	//}

	int score;
	DWORD	dwByte(0);

	while (true)
	{
		bool bGetData = ReadFile(hFile, &score, sizeof(int), &dwByte, NULL);

		if (0 == dwByte)
			break;

		if(bGetData)
			m_iScoreList.push_back(score);
	}

	CloseHandle(hFile);

	m_iHighScore = m_iScoreList[0];
}
