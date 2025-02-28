#include "stdafx.h"
#include "CBScoreTexture.h"
#include "BmpMgr.h"

CBScoreTexture::CBScoreTexture()
    :m_iScore(0), m_lDeadDelyTime(0), m_bNoScore(true)
{
}

CBScoreTexture::CBScoreTexture(int _score) 
    : m_bNoScore(true)
{
    m_iScore = _score;
    m_lDeadDelyTime = 0;
}

CBScoreTexture::~CBScoreTexture()
{
    Release();
}

void CBScoreTexture::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/ScoreEffect.bmp",L"ScoreEffect");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/ScoreBigEffect.bmp",L"ScoreBigEffect");

    m_tFrame = { 0, 0, 0, 0, GetTickCount64() };
    m_pFrameKey = L"ScoreEffect";
    m_tInfo = {0, 0, 54, 32};
    m_fSpeed = 1.f;
    m_lDeadDelyTime = GetTickCount64();
}

int CBScoreTexture::Update()
{
    if (m_lDeadDelyTime + 600 < GetTickCount64())
    {
        return OBJ_DEAD;
    }


    switch (m_iScore)
    {
    case 100:
        m_tFrame.iFrameStart = 0; break;
    case 300:
        m_tFrame.iFrameStart = 1; break;
    case 500:
        m_tFrame.iFrameStart = 2; break;
    case 700:
        m_tFrame.iFrameStart = 3; break;
    case 1000:
        m_tFrame.iFrameStart = 4; break;
    case 2000:
        m_tFrame.iFrameStart = 5; break;
    case 3000:
        m_tFrame.iFrameStart = 6; break;
    case 4000:
        m_tFrame.iFrameStart = 7; break;
    case 5000:
        m_tFrame.iFrameStart = 8; break;
    case 7000:
        m_tFrame.iFrameStart = 9; break;
    default:
        m_bNoScore = false;  break;
    }

    m_tInfo.fY -= m_fSpeed;

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CBScoreTexture::Late_Update()
{
}

void CBScoreTexture::Render(HDC hDC)
{
    if (m_bNoScore)
    {
        HDC hImg = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

        int iSize(1);
        if (m_iScore >= 5000)
        {
            iSize = 2;
        }

        GdiTransparentBlt(hDC,
            m_tRect.left,
            m_tRect.top,
            (int)m_tInfo.fCX * iSize,
            (int)m_tInfo.fCY * iSize,
            hImg,
            m_tFrame.iFrameStart * m_tInfo.fCX,
            0,
            (int)m_tInfo.fCX, // 복사할 비트맵의 가로, 세로 길이
            (int)m_tInfo.fCY,
            RGB(15, 79, 174));	// 제거하고자 하는 색상을 전달
    }
}

void CBScoreTexture::Release()
{
}
