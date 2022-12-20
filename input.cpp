//==================================================
//
// input.cpp
// Author: 鰤
//
//==================================================
#include "input.h"

//=========================================
// 静的メンバ変数
//=========================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//=========================================
// コンストラクタ
//=========================================
CInput::CInput()
{
	m_pDevKeyboard = nullptr;
}

//=========================================
// デストラクタ
//=========================================
CInput::~CInput()
{
}

//=========================================
// 初期化
//=========================================
HRESULT CInput::Init(const HINSTANCE hInstance, const HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス（キーボード）の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevKeyboard->Acquire();

	return S_OK;
}

//=========================================
//終了処理
//=========================================
void CInput::Uninit()
{
	// DirectInputオブジェクトの破壊
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}
