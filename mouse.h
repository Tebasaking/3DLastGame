//=============================================================================
//
// マウス入力処理(mouse.h)
// Author : 唐�ｱ結斗
// 概要 : マウス生成を行う
//
//=============================================================================
#ifndef _MOUSE_H_		// このマクロ定義がされてなかったら
#define _MOUSE_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "input.h"

//=============================================================================
// マウス入力処理
// Author : 唐�ｱ結斗
// 概要 : マウス生成を行う
//=============================================================================
class CInput;

class CMouse : public CInput
{
public:
	//--------------------------------------------------------------------
	// マウスのキー種別
	// Author : 唐�ｱ結斗
	// 概要 : マウスのキー種別
	//--------------------------------------------------------------------
	enum MOUSE_KEY
	{
		MOUSE_KEY_LEFT = 0,				// マウス左ボタン
		MOUSE_KEY_RIGHT,				// マウス右ボタン
		MOUSE_KEY_MIDDLE,				// マウス中央ボタン
		MOUSE_KEY_SIDE_0,				// マウスサイドボタン0
		MOUSE_KEY_SIDE_1,				// マウスサイドボタン1
		MOUSE_KEY_SIDE_2,				// マウスサイドボタン2
		MOUSE_KEY_SIDE_3,				// マウスサイドボタン3
		MOUSE_KEY_SIDE_4,				// マウスサイドボタン4
		MAX_MOUSE_KEY,					// マウスのボタンの最大数
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMouse();
	~CMouse() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(const HINSTANCE hInstance, const HWND hWnd) override;			// 初期化
	void Uninit(void) override;										// 終了
	void Update(void) override;										// 更新
	bool GetPress(int nKey);										// プレス情報の取得
	bool GetTrigger(int nKey);										// トリガー情報の取得
	bool GetRelease(int nKey);										// リリース情報の取得
	D3DXVECTOR3 GetMouseCursor(void);								// マウスカーソルのスクリーン座標の取得
	int GetMouseWheel(void);										// マウスのホイールの動き処理
	D3DXVECTOR3 GetMouseMove(void);									// マウスの移動量を出力処理
	void UseSetPosLock(bool use) { m_isPosLock = use; }
	void SetCursorPosLock(void);

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	DIMOUSESTATE2 m_aKeyState;					// プレス情報
	DIMOUSESTATE2 m_aKeyStateTrigger;			// トリガー情報
	DIMOUSESTATE2 m_aKeyStateRelease;			// リリース情報
	POINT m_mouseCursor;						// マウスカーソルの位置
	HWND m_hWnd;								// ウィンドウハンドを格納する変数
	bool m_isPosLock;
};

#endif
