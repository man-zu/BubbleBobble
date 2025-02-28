#pragma once

#include "Define.h"

class CMyBmp
{
public:
	CMyBmp();
	~CMyBmp();

public:
	HDC			Get_MemDC() { return m_hMemDC; }
	void		Load_Bmp(const TCHAR* pFilePath);
	void		Release();

private:
	HDC			m_hMemDC;

	HBITMAP		m_hBitmap;

	HBITMAP		m_hOldBmp;
};

// DDB(Device Dependent Bitmap) : ��ġ�� �������� ��Ʈ�� ���� ����, �̹����� ũ��, ���� �� �⺻���� ������ �̹��� �����ͷ� �����Ǿ� �־� �پ��� �ػ��� ��ġ�� �������ϰ� ������ ����, ���� ��� ��� ��Ʈ���� ��� ��ġ�θ� �÷� ��Ʈ���� �÷� ��ġ�θ� ��� �� �� �ִ�.
// DIB(Device Independent Bitmap) : ��ġ�� �������� ��Ʈ�� ���� ����, ddb�� ���� ���� ���̺�, �ػ� ���� ���� �߰� ������ �����Ƿ� ��ġ�� ���ӵ��� �ʾ� Ȱ�뵵�� �� �������ϰ� ȣȯ���� �پ