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

	HANDLE		hFile = CreateFile(L"../Data/Score/BubbleBobbleScore.dat", // ���� ��ο� �̸��� ���
		GENERIC_WRITE, // ���� ���� ���(GENERIC_WRITE : ���� ����, GENERIC_READ : �б� ����)
		NULL, // ���� ���, ������ ����� ���¿��� �ٸ� ���μ����� ���� ������ �ϴ� ���� �㰡�� ���ΰ�(NULL �������� ����)
		NULL, // ���� �Ӽ�, NULL �� ��� �⺻ ���� ����
		CREATE_ALWAYS,	// ���� ���, (CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ�(�б� ����, ���� ���� ���),FILE_ATTRIBUTE_NORMAL : Ư�� �Ӽ��� ���� �Ϲ� ���� ���� 
		NULL); // ������ ������ �Ӽ��� ������ ���ø� ���� ( ������� �ʱ� ������ NULL)

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
	HANDLE		hFile = CreateFile(L"../Data/Score/BubbleBobbleScore.dat", // ���� ��ο� �̸��� ���
		GENERIC_READ, // ���� ���� ���(GENERIC_WRITE : ���� ����, GENERIC_READ : �б� ����)
		NULL, // ���� ���, ������ ����� ���¿��� �ٸ� ���μ����� ���� ������ �ϴ� ���� �㰡�� ���ΰ�(NULL �������� ����)
		NULL, // ���� �Ӽ�, NULL �� ��� �⺻ ���� ����
		OPEN_EXISTING,	// ���� ���, (CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ�(�б� ����, ���� ���� ���),FILE_ATTRIBUTE_NORMAL : Ư�� �Ӽ��� ���� �Ϲ� ���� ���� 
		NULL); // ������ ������ �Ӽ��� ������ ���ø� ���� ( ������� �ʱ� ������ NULL)

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
