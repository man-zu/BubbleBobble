#pragma once

#define WINCX		800
#define WINCY		600

#define PURE		= 0

#define	PI			3.141592f

#define TILECX		24	//32
#define TILECY		24	//32

#define TILEX		34
#define TILEY		26

#define		OBJ_NOEVENT	0
#define		OBJ_DEAD		1

#define		VK_MAX		0xff
#define		BOSSHP		500

#define		SOUND_VOLUME	5



template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

typedef struct tagInfo
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;

}INFO;

enum DIRECTION
{
	DIR_LEFT, 
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
	DIR_END
};

enum OBJID
{
	MOUSE,
	PLAYER,
	MONSTER,
	BUBBLE,
	ELECTBUBBLE,
	FIREBUBBLE,
	ITEM,
	ITEMSCORE,
	BOSS,
	BOSSBULLET,
	TEXT,
	MENUBUTTON,
	SKINBUTTON,
	EXTEND,
	END
};

enum CHANNELID
{
	SOUND_EFFECT,
	SOUND_EFFECT_PLAYER_DEAD,
	ITEM_SOUND_EFFECT,
	SOUND_BGM,
	ENDING,
	MAXCHANNEL
};

typedef struct tagLingPoint
{
	float	fX;
	float	fY;

	tagLingPoint() { ZeroMemory(this, sizeof(tagLingPoint)); }
	tagLingPoint(float _fX, float _fY) : fX(_fX), fY(_fY) { }

}LINEPOINT;

typedef struct tagLine
{
	LINEPOINT	tLeft;
	LINEPOINT	tRight;

	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(LINEPOINT& _tLeft, LINEPOINT& _tRight)
		: tLeft(_tLeft), tRight(_tRight) {}

}LINE;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iMotion;
	DWORD	dwSpeed;
	ULONGLONG	dwTime;

}FRAME;


struct tagFinder
{
public:
	tagFinder(const TCHAR* pString) : m_pString(pString) {}

public:
	template<typename T>
	bool operator()(T& Pair)
	{
		return !lstrcmp(m_pString, Pair.first);
	}

private:
	const TCHAR*		m_pString;
};




extern HWND g_hWnd;