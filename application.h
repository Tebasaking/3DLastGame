//=========================================
//
//	application.h
//	Author:冨所知生
//
//
//=========================================
#ifndef _MANAGER_H_		//このマクロ定義がされてなかったら
#define _MANAGER_H_		//２重インクルード防止のマクロ定義

#include "render.h"

class CDebugProc;
class CTexture;
class CPlayer;
class CEnemy;
class CCamera;
class CCameraPlayer;
class CCameraRadar;
class CMesh;
class CInputKeyboard;
class CTarget;
class CMouse;
class CGame;
class CMode;
class CFade;
class CTexture3D;
class CJoypad;
class CSound;
class CCameraTitle;
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
	void SetViewPort();

	//クリエイト処理
	static void SetNextMode(MODE mode);
	static MODE GetMode() { return m_mode; }
	void CApplication::ChangeMode();
	static MODE m_mode;
	static MODE m_NextMode;

	static D3DXVECTOR3 WorldToScreen(D3DXVECTOR3 pos);

	static CRender *GetRender();
	static CEnemy *GetEnemy();
	static CPlayer *GetPlayer();
	static CTexture* GetTexture();
	static CCameraPlayer* GetCamera() { return m_pCamera; };
	static CCameraTitle *GetTitleCamera() { return m_pTitle; }
	static CCameraRadar* GetRader() { return m_pRader; }
	static CInputKeyboard * GetInputKeyboard();
	static CMouse *GetMouse() { return m_pMouse; }
	static CGame *GetGame() { return m_pGame; }
	static CMode *GetModeObject() { return m_pMode; }
	static CTexture3D *GetTexture3D() { return m_pTexture3D; }
	static CJoypad *GetJoy() { return m_pJoy; }
	static CSound *GetSound() { return m_pSound; }
	static LPD3DXEFFECT GetShader() { return m_pEffect; }

private:
	static CDebugProc		*m_pDebug;
	static CRender			*m_pRender;
	static CInputKeyboard	*m_pInputKeyboard;
	static CTexture			*m_pTexture;
	static CCameraPlayer	*m_pCamera;
	static CCameraRadar		*m_pRader;
	static CPlayer			*m_pPlayer;
	static CEnemy			*m_pEnemy;
	static CMouse			*m_pMouse;
	static CGame			*m_pGame;
	static CMode			*m_pMode;
	static CTexture3D		*m_pTexture3D;
	static CJoypad			*m_pJoy;
	static CSound			*m_pSound;
	static CCameraTitle		*m_pTitle;
	static LPD3DXEFFECT		m_pEffect;			// シェーダー
	bool					m_bCheckViewport;
};

#endif