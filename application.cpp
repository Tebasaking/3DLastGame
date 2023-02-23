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
#include "camera_player.h"
#include "camera_radar.h"
#include "camera_title.h"
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
#include "camera_title.h"
#include "fade.h"
#include "model3D.h"
#include "texture3D.h"
#include "joypad.h"
#include "time_over.h"
#include "game_over.h"

CDebugProc *CApplication::m_pDebug = nullptr;
CRender *CApplication::m_pRender = nullptr;
CPlayer *CApplication::m_pPlayer = nullptr;
CEnemy *CApplication::m_pEnemy = nullptr;
CTexture* CApplication::m_pTexture = nullptr;
CMode* CApplication::m_pMode = nullptr;
CGame* CApplication::m_pGame = nullptr;
CCameraTitle* CApplication::m_pTitle = nullptr;
CTexture3D* CApplication::m_pTexture3D = nullptr;
CSound*	CApplication::m_pSound = nullptr;		//サウンド

CCameraRadar* CApplication::m_pRader = nullptr;
CCameraPlayer* CApplication::m_pCamera = nullptr;

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
	m_pCamera = new CCameraPlayer;
	m_pRader = new CCameraRadar;
	m_pDebug = new CDebugProc;
	m_pTexture3D = new CTexture3D;
	m_pTitle = new CCameraTitle;

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

	CInput::Create();

	if (FAILED(CInput::GetKey()->Init(hInstance, hWnd)))
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
		//m_pRader->SetViewSize(25.0f, SCREEN_HEIGHT * 0.5f + 5.0f, SCREEN_WIDTH * 0.25f * 0.75f, SCREEN_HEIGHT * 0.5f * 0.75f);
		m_pRader->SetViewSize(25.0f, SCREEN_HEIGHT - 285, SCREEN_WIDTH * 0.25f * 0.75f, SCREEN_HEIGHT * 0.5f * 0.75f);

	}

	// カメラの初期化設定
	if (m_pTitle != nullptr)
	{	// カメラの初期化設定3
		m_pTitle->SetObjMode(CObject::TITLE_MODE);
		m_pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, -150.0f));
		m_pTitle->SetViewSize(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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
	//入力処理の更新処理
	CInput::GetKey()->Update();

	// モードの変更
	ChangeMode();

	// ライトの更新処理
	UpdateLight();

	// カメラの更新処理
	m_pCamera->Update();

	// タイトルカメラの更新処理
	m_pTitle->Update();

	// レーダーの更新処理
	m_pRader->Update();

	// レンダーの更新処理
	m_pRender->Update();
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
	//入力処理の終了処理
	CInput::GetKey()->Uninit();

	if (m_pRender != nullptr)
	{
		// レンダーの終了処理
		m_pRender->Uninit();
		delete m_pRender;
		m_pRender = nullptr;
	}

	if (m_pTexture3D != nullptr)
	{
		// テクスチャの終了処理
		m_pTexture3D->Uninit();
		delete m_pTexture3D;
		m_pTexture3D = nullptr;
	}

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
		// テクスチャの終了処理
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	if (m_pCamera != nullptr)
	{
		// カメラの終了処理
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	if (m_pRader != nullptr)
	{
		delete m_pRader;
		m_pRader = nullptr;
	}
	if (m_pTitle != nullptr)
	{
		delete m_pTitle;
		m_pTitle = nullptr;
	}
	if (m_pDebug != nullptr)
	{
		// デバックの終了処理
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = nullptr;
	}
	if (m_pRader != nullptr)
	{
		// レーダーの終了処理
		m_pRader->Uninit();
		delete m_pRader;
		m_pRader = nullptr;
	}
	if (m_pTitle != nullptr)
	{
		// タイトルカメラの終了処理
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = nullptr;
	}
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		//delete m_pMode;
		//m_pMode = nullptr;
	}

	// オブジェクトの終了処理
	CObject::UninitAll();

	// モデルの終了処理
	CModel3D::UninitModel();

	// ライトの終了処理
	UninitLight();
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
		//m_pCamera->Init(D3DXVECTOR3(0.0f, 0.0f, -150.0f));
		m_pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, -150.0f));
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

	case MODE_TIME_OVER:
		m_pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, -150.0f));
		// タイムオーバーの初期化処理
		m_pMode = new CTimeOver;
		break;

	case MODE_GAME_OVER:
		m_pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, -150.0f));
		// ゲームオーバーの初期化処理
		m_pMode = new CGameOver;
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

