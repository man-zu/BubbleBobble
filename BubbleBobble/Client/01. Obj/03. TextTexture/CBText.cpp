#include "stdafx.h"
#include "CBText.h"
#include "BmpMgr.h"
#include "SceneMgr.h"

CBText::CBText()
    :m_eTextType(TEXTTYPE::NONE)
{
}

CBText::~CBText()
{
    Release();
}

void CBText::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/1UP.bmp", L"1UP");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/HighScore.bmp", L"HighScore");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/Score.bmp", L"ScoreNum");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/HurryUp.bmp", L"HurryUp");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Score/BOSS_STAGE.bmp", L"BOSS_STAGE");

    m_tInfo = {0, 0, 100, 22};
    m_iScore = 0;
}

int CBText::Update()
{
    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CBText::Late_Update()
{
    if (m_eTextType == TEXTTYPE::HURRYUP)
    {
        Move_Frame();
        //Motion_Change();
    }
}

void CBText::Render(HDC hDC)
{
    HDC hImg = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

    if (m_eTextType == TEXTTYPE::UP)
    {
        m_pFrameKey = L"1UP";
        BitBlt(hDC, m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, hImg, 0, 0, SRCCOPY);
    }
    else if (m_eTextType == TEXTTYPE::HIGHSCORE)
    {
        BitBlt(hDC, m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, hImg, 0, 0, SRCCOPY);
    }
    else if (m_eTextType == TEXTTYPE::NUM)
    {
        string sScore = to_string(m_iScore);
        int iCnt = to_string(m_iScore).length();

        for (int i = 0; i < to_string(m_iScore).length(); ++i)
        {
            BitBlt(hDC, m_tInfo.fX - (m_tInfo.fCX * iCnt), m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY, hImg, m_tInfo.fCX * (sScore[i] - '0'), 0, SRCCOPY);
            --iCnt;
        }
    }
    else if (m_eTextType == TEXTTYPE::HURRYUP)
    {
        m_tFrame = { 0, 1, 0, 200, GetTickCount64() };

        GdiTransparentBlt(hDC,
            m_tRect.left,
            m_tRect.top,
            164,//(int)m_tInfo.fCX,
            22,//(int)m_tInfo.fCY,
            hImg,
            164 * (m_tFrame.iFrameStart),
            22 * m_tFrame.iMotion,
            164, // 복사할 비트맵의 가로, 세로 길이
            22,
            RGB(15, 79, 174));	// 제거하고자 하는 색상을 전달
    }
    else if (m_eTextType == TEXTTYPE::BOSSUP)
    {
        hImg = CBmpMgr::Get_Instance()->Find_Img(L"BOSS_STAGE");
        GdiTransparentBlt(hDC,
            m_tInfo.fX,
            m_tInfo.fY,
            400,//(int)m_tInfo.fCX,
            98,//(int)m_tInfo.fCY,
            hImg,
            0,
            0,
            874, // 복사할 비트맵의 가로, 세로 길이
            200,
            RGB(1, 1, 11));	// 제거하고자 하는 색상을 전달
    }
}

void CBText::Release()
{
}

void CBText::Set_Type(TEXTTYPE _param)
{ 
    m_eTextType = _param;

    if (m_eTextType == TEXTTYPE::UP)
    {
        m_pFrameKey = L"1UP";
        m_tInfo = { 120, 11 , 242 , 22 };
    }
    else if (m_eTextType == TEXTTYPE::HIGHSCORE)
    {
        m_pFrameKey = L"HighScore";
        m_tInfo = { 450, 11 , 220, 22 };
    }
    else if (m_eTextType == TEXTTYPE::NUM)
    {

        // 자리수 구해서 해당 숫자만큼 띄우면 될거같은데
        //int iScore[32];
        m_pFrameKey = L"ScoreNum";
        m_tInfo.fCX = 22;
        m_tInfo.fCY = 22;
        //m_tInfo = { 300, 33 , 22, 22 };
        /*m_tInfo = { 120, 33 , 220, 22 };*/
        //m_tFrame = {0, 9, 0, 200};
    }
    else if (m_eTextType == TEXTTYPE::HURRYUP)
    {
        m_pFrameKey = L"HurryUp";
        m_tInfo = { 50, 150 , 242 , 22 };
    }
    else if (m_eTextType == TEXTTYPE::BOSSUP)
    {
        m_pFrameKey = L"BOSS_STAGE";
    }
}
