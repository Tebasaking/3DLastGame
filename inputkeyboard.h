//==================================================
// input_keyboard.h
// Author: Buriya Kota
//==================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

//**************************************************
// インクルード
//**************************************************
#include "input.h"

//**************************************************
// クラス
//**************************************************
class CInputKeyboard : public CInput
{
private:
	static const int NUM_KEY_MAX = 256;	//キーの最大数（キーボード）

public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(const HINSTANCE hInstance, const HWND hWnd) override;		// 入力処理全部の初期化
	void Uninit(void) override;									// 入力処理全部の終了処理
	void Update(void) override;									// 入力処理全部の更新処理

	bool GetPress(const int nKey);				// キーボードプレス処理
	bool GetTrigger(const int nKey);			// キーボードトリガー処理

private:
	BYTE m_KeyState[NUM_KEY_MAX];				// キーボードのプレス情報
	BYTE m_KeyStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報

};

#endif // _INPUT_KEYBOARD_H_