//=========================================
//
//	application.h
//	Author:冨所知生
//
//
//=========================================
#ifndef _MANAGER_H_		//このマクロ定義がされてなかったら
#define _MANAGER_H_		//２重インクルード防止のマクロ定義

#include "main.h"
#include "render.h"

class CDebugProc;
class CTexture;
class CPlayer;
class CEnemy;
class CCamera;
class CMesh;
class CInputKeyboard;
class CTarget;
class CMouse;
class CGame;
class CMode;
class CFade;
class CTexture3D;
class CApplication
{
public:
	enum MODE
	{
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	};

	CApplication();			//コンストラクタ
	~CApplication();		//デストラクタ

	HRESULT Init(HINSTANCE hInstance,HWND hWnd);
	void Uninit();
	void Update();
	void Draw();

	//クリエイト処理
	static void SetNextMode(MODE mode);
	static MODE GetMode();
	void CApplication::ChangeMode();
	static MODE m_NextMode;

	static D3DXVECTOR3 WorldToScreen(D3DXVECTOR3 pos);

	static CRender *GetRender();
	static CEnemy *GetEnemy();
	static CPlayer *GetPlayer();
	static CTexture* GetTexture();
	static CCamera* GetCamera();
	static CInputKeyboard * GetInputKeyboard();
	static CMouse *GetMouse() { return m_pMouse; }
	static CGame *GetGame() { return m_pGame; }
	static CMode *GetModeObject() { return m_pMode; }
	static CTexture3D *GetTexture3D() { return m_pTexture3D; }

private:
	static CDebugProc		*m_pDebug;
	static CRender			*m_pRender;
	static CInputKeyboard	*m_pInputKeyboard;
	static CTexture			*m_pTexture;
	static CCamera			*m_pCamera;
	static CPlayer			*m_pPlayer;
	static CEnemy			*m_pEnemy;
	static CMouse			*m_pMouse;
	static CGame			*m_pGame;
	static CMode			*m_pMode;
	static CTexture3D		*m_pTexture3D;
};

#endif