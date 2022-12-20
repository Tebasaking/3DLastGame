//==================================================
// input_keyboard.cpp
// Author: 冨所知生
//==================================================

//**************************************************
// include
//**************************************************
#include <string.h>
#include "inputkeyboard.h"

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CInputKeyboard::CInputKeyboard()
{
	// クリア
	memset(m_KeyState, 0, sizeof(m_KeyState));
	memset(m_KeyStateTrigger, 0, sizeof(m_KeyStateTrigger));
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CInputKeyboard::~CInputKeyboard()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CInputKeyboard::Init(const HINSTANCE hInstance, const HWND hWnd)
{
	return CInput::Init(hInstance, hWnd);
}

//--------------------------------------------------
// 終了処理
//--------------------------------------------------
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//--------------------------------------------------
// 更新処理
//--------------------------------------------------
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報

	int nCntKey;
	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_KeyStateTrigger[nCntKey] = (m_KeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		// キーボードのトリガー情報を保存
			m_KeyState[nCntKey] = aKeyState[nCntKey];		// キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevKeyboard->Acquire();		// キーボードへのアクセス権を獲得
	}
}

//--------------------------------------------------
// キーボードプレス処理
//--------------------------------------------------
bool CInputKeyboard::GetPress(const int nKey)
{
	return (m_KeyState[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------
// キーボードトリガー処理
//--------------------------------------------------
bool CInputKeyboard::GetTrigger(const int nKey)
{
	return (m_KeyStateTrigger[nKey] & 0x80) ? true : false;
}
