//=========================================
// 
// 	マネージャ―の処理
// 	Author:冨所知生　
// 
//=========================================
#include "application.h"
#include "render.h"
#include "object2D.h"
#include "billboard.h"
#include "player3D.h"
#include "input.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "Object3D.h"
#include "meshfield.h"
#include "sphere.h"
#include "inputkeyboard.h"
#include "mouse.h"
#include "sound.h"
#include "mode.h"
#include "score.h"
#include "game.h"
#include "title.h"
#include "debug_proc.h"
#include "result.h"
#include "fade.h"
#include "model3D.h"
#include "texture3D.h"
#include "joypad.h"

CDebugProc *CApplication::m_pDebug = nullptr;
CRender *CApplication::m_pRender = nullptr;
CInputKeyboard *CApplication::m_pInputKeyboard = nullptr;
CPlayer *CApplication::m_pPlayer = nullptr;
CEnemy *CApplication::m_pEnemy = nullptr;
CTexture* CApplication::m_pTexture = nullptr;
CMouse* CApplication::m_pMouse = nullptr;
CMode* CApplication::m_pMode = nullptr;
CJoypad *CApplication::m_pJoy = {};	
CGame* CApplication::m_pGame = nullptr;
CTexture3D* CApplication::m_pTexture3D = nullptr;
CSound*	CApplication::m_pSound = nullptr;		//サウンド

CCamera* CApplication::m_pRader = nullptr;
CCamera* CApplication::m_pCamera = nullptr;

CSound *pSound = nullptr;
CApplication::MODE CApplication::m_NextMode = MODE_MAX;
CApplication::MODE CApplication::m_mode = MODE_MAX;

LPD3DXEFFECT CApplication::m_pEffect = nullptr;

//=========================================
// コンストラクタ
//=========================================
CApplication::CApplication()
{
}

//=========================================
// デストラクタ
//=========================================
CApplication::~CApplication()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CApplication::Init(HINSTANCE hInstance,HWND hWnd)
{
	m_pRender = new CRender;
	m_pTexture = new CTexture;
	m_pCamera = new CCamera;
	m_pRader = new CCamera;
	m_pInputKeyboard = new CInputKeyboard;
	pSound = new CSound;
	m_pMouse = new CMouse;
	m_pDebug = new CDebugProc;
	m_pTexture3D = new CTexture3D;
	m_pJoy = new CJoypad;

	if (FAILED(m_pRender->Init(hWnd, true)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// TEXTURE(3D)の読み込み
	m_pTexture3D->Init();

	// ライトの初期化処理
	InitLight();

	// Model3Dの読み込み
	CModel3D::InitModel();

	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// 初期化処理
	if (FAILED(m_pJoy->Init(1)))
	{
		return E_FAIL;
	}

	//----------------------------
	// サウンドの生成と初期化
	//----------------------------
	m_pSound = new CSound;
	m_pSound->Init(hWnd);

	// カメラの初期化設定
	if (m_pCamera != nullptr)
	{
		m_pCamera->SetObjMode(CObject::NORMAL_MODE);
		m_pCamera->Init(D3DXVECTOR3(0.0f, 1000.0f, -150.0f));
		m_pCamera->SetViewSize(0, 0, SCREEN_WIDTH , SCREEN_HEIGHT);
	}

	if (m_pRader != nullptr)
	{
		// カメラの初期化設定2
		m_pRader->SetObjMode(CObject::RADAR_MODE);
		m_pRader->Init(D3DXVECTOR3(0.0f, 5000.0f, -150.0f));
		m_pRader->SetViewSize(0, 0, SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f);
	}

	// ゲームスタート時の初期設定
	// モードの設定
	SetNextMode(MODE_TITLE);

	// デバックの初期化処理
	m_pDebug->Init();

	//モードの設定
	CFade::Create(m_NextMode);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetRender()->GetDevice();

	D3DXCreateEffectFromFile(
		pDevice, "Effect.fx", NULL, NULL,
		0, NULL, &m_pEffect, nullptr);

	return S_OK;
}

//=========================================
// 更新処理
//=========================================
void CApplication::Update()
{
	// 入力処理の更新処理
	m_pInputKeyboard->Update();

	// マウス処理の更新処理
	m_pMouse->Update();

	// モードの変更
	ChangeMode();

	// ライトの更新処理
	UpdateLight();

	// カメラの更新処理
	m_pCamera->Update();

	// レーダーの更新処理
	m_pRader->Update();

	// レンダーの更新処理
	m_pRender->Update();

	//// ジョイパッドの更新処理
	//m_pJoy->Update();

	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
}

//=========================================
// 描画処理
//=========================================
void CApplication::Draw()
{
	// レンダーの描画処理
	m_pRender->Draw();
}

//=========================================
// 終了処理
//=========================================
void CApplication::Uninit()
{
	// 入力処理の終了処理
	m_pInputKeyboard->Uninit();

	// マウス処理の終了処理
	m_pMouse->Uninit();

	// レンダーの終了処理
	m_pRender->Uninit();

	// テクスチャの終了処理
	m_pTexture3D->Uninit();

	// モデルの終了処理
	CModel3D::UninitModel();

	// テクスチャの終了処理
	m_pTexture->ReleaseAll();

	// ライトの終了処理
	UninitLight();

	// カメラの終了処理
	m_pCamera->Uninit();

	// レーダーの終了処理
	m_pRader->Uninit();

	// デバックの終了処理
	m_pDebug->Uninit();

	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	if (m_pRender != nullptr)
	{
		delete m_pRender;
		m_pRender = nullptr;
	}
	if (m_pTexture != nullptr)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	if (m_pCamera != nullptr)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	if (m_pRader != nullptr)
	{
		delete m_pRader;
		m_pRader = nullptr;
	}
	if (m_pInputKeyboard != nullptr)
	{
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}
	if (m_pJoy != nullptr)
	{// 終了処理
		m_pJoy->Uninit();

		delete m_pJoy;
		m_pJoy = nullptr;
	}
}

//=========================================
// レンダーの情報の取得
//=========================================
CRender *CApplication::GetRender()
{
	return m_pRender;
}

//=========================================
// プレイヤーの情報の取得
//=========================================
CPlayer *CApplication::GetPlayer()
{
	return m_pPlayer;
}

//=========================================
// プレイヤーの情報の取得
//=========================================
CEnemy *CApplication::GetEnemy()
{
	return m_pEnemy;
}

//=========================================
// テクスチャの情報の取得
//=========================================
CTexture* CApplication::GetTexture()
{
	return m_pTexture;
}

//=========================================
// キーボードの情報の取得
//=========================================
CInputKeyboard* CApplication::GetInputKeyboard()
{
	return m_pInputKeyboard;
}

//=========================================
// ワールド座標をスクリーン座標に変換する関数
//=========================================
D3DXVECTOR3 CApplication::WorldToScreen(D3DXVECTOR3 pos)
{
	// ビュー行列と射影行列の取得
	D3DXMATRIX mtx;

	D3DXMatrixTranslation(&mtx, pos.x, pos.y, pos.z);

	CCamera *pCamera = GetCamera();

	D3DXMATRIX view = pCamera->GetView();
	D3DXMATRIX proj = pCamera->GetProjection();

	mtx = mtx * view * proj;

	D3DXVECTOR3 pOut(mtx._41, mtx._42, mtx._43);

	// zで割って-1か1の範囲に収める
	pOut.x /= pOut.z, pOut.y /= pOut.z/*, pOut.z /= pOut.z*/;

	// ビューポート行列（スクリーン行列）の作成
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);

	viewport._11 = SCREEN_WIDTH / 2.0f;
	viewport._22 = -SCREEN_HEIGHT / 2.0f;
	viewport._41 = SCREEN_WIDTH / 2.0f;
	viewport._42 = SCREEN_HEIGHT / 2.0f;

	D3DXVec3TransformCoord(&pOut, &pOut, &viewport);

	return pOut;
}

//==================================================
// ゲームモードの次のモードの設定
//==================================================
void CApplication::SetNextMode(MODE mode)
{
	m_NextMode = mode;
	m_mode = mode;
}

//=========================================
// ゲームモードの変更
//=========================================
void CApplication::ChangeMode()
{
	if (m_NextMode == MODE_MAX)
	{
		return;
	}

	// オブジェクト全ての終了処理
	CObject::UninitAll();

	//モードの設定
	CFade::Create(m_NextMode);

	switch (m_NextMode)
	{
	case MODE_TITLE:
		// カメラの初期化処理
		m_pCamera->Init(D3DXVECTOR3(0.0f, 0.0f, -150.0f));

		m_pMode = new CTitle;
		break;

	case MODE_GAME:
		// カメラの初期化処理
		m_pCamera->Init(D3DXVECTOR3(0.0f, 1000.0f, -150.0f));
		m_pRader->Init(D3DXVECTOR3(0.0f, 5000.0f, -150.0f));
		m_pMode = new CGame;
		break;

	case MODE_RESULT:
		// カメラの初期化処理
		m_pCamera->Init(D3DXVECTOR3(0.0f, 1000.0f, -150.0f));
		m_pMode = new CResult;
		break;

	default:
		break;
	}

	// 次のモードの初期化
	m_NextMode = MODE_MAX;

	if (m_pMode != nullptr)
	{
		m_pMode->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}

