//=========================================
//
//	debug.h
//	Author:冨所知生
//
//=========================================
#ifndef _DEBUG_H_		//このマクロ定義がされてなかったら
#define _DEBUG_H_		//２重インクルード防止のマクロ定義

#include "main.h"
//=========================================
// クラス
//=========================================
class CDebugProc
{
public:
	CDebugProc(){ }
	~CDebugProc() {}

	void Init(void);
	void Uninit(void);
	static void print(const char *pFormat,...);
	static void Draw(void);

private:
	static LPD3DXFONT m_pFont;
	static char m_aStr[1024];
};

#endif	// _DEBUG_H_