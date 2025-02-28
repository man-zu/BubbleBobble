#pragma once
class CScoreMgr

{
private:
	CScoreMgr();
	~CScoreMgr();

public:
	static CScoreMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CScoreMgr;

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

public:
	//void Initialize();
	//void Update();
	void Release();
	void SaveScoreData();
	void LoadScoreData();
	vector<int> Get_Score_Data() {
		vector<int> vecTop10;
		int iMax(m_iScoreList.size());
		iMax = iMax > 10 ? 10 : iMax;
		iMax = 10 > m_iScoreList.size() ? m_iScoreList.size() : 10;

		for (int i = 0; i < iMax; ++i)
			vecTop10.push_back(m_iScoreList[i]);
		return vecTop10;
	}

	int				Get_High_Score() { return m_iHighScore; }
	void			Set_High_Score(int _param) { m_iHighScore = m_iHighScore > _param ? m_iHighScore : _param; }
	void			Set_Scene_Score(int _param) { m_iSceneScore = _param; }
	int				Get_Scene_Score() { return m_iSceneScore; }

private:
	static	CScoreMgr*	m_pInstance;
	vector<int>			m_iScoreList;
	int					m_iSceneScore;
	int					m_iHighScore;
};

