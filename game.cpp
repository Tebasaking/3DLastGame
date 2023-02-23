//=========================================
// game.cpp
// Author: 冨所知生
//=========================================
#include <algorithm>
#include "application.h"
#include "game.h"
#include "player_manager.h"
#include "playerUI.h"
#include "enemy_manager.h"
#include "sphere.h"
#include "meshfield.h"
#include "score.h"
#include "input.h"
#include "fade.h"
#include "enemy.h"
#include "camera.h"
#include "radar.h"
#include "number.h"
#include "render.h"
#include "time.h"
#include "score.h"
#include "sound.h"
#include "mouse.h"
#include "ranking.h"
#include "missile_alert.h"

bool CGame::m_bFinish = false;
CMesh* CGame::m_pMesh[3] = {};
CScore* CGame::m_pScore = nullptr;
CPlayerManager* CGame::m_pPlayerManager = nullptr;
CEnemy_Manager* CGame::m_pEnemyManager = nullptr;
CPlayerUI* CGame::m_PlayerUI = nullptr;
CAlert* CGame::m_pAlert = nullptr;
//=========================================
// コンストラクタ
//=========================================
CGame::CGame()
{
}

//=========================================
// デストラクタ
//=========================================
CGame::~CGame()
{
}

//=========================================
// 初期化 
//=========================================
HRESULT CGame::Init(const D3DXVECTOR3 &pos)
{
	CMouse *pMouse = CApplication::GetMouse();

	//CObject2D *pObject2D = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 4);
	//pObject2D->SetScale(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	//
	//pObject2D->SetTexture(CTexture::TEXTURE_TEST);
	
	// アラートの生成
	m_pAlert = CAlert::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_PlayerUI = new CPlayerUI(4);
	m_PlayerUI->Init(D3DXVECTOR3(0.0f,0.0f,0.0f));

	pMouse->SetShowCursor(true);
	pMouse->UseSetPosLock(true);

	m_bFinish = false;

	CRender *pRender = CApplication::GetRender();
	pRender->SetFog(true, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0001f));

	//タイマーの生成
	CTime *pTime = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 200.0f, 85.0f, 0.0f));

	// ゲームのタイムリミットを設定(分,秒);
	pTime->SetAlarm(2,0);

	// スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 85.0f, 0.0f));

	// 海
	m_pMesh[0] = CMesh::Create(D3DXVECTOR3(0.0f, -350.0f, 0.0f), CMesh::TYPE_SEA);
	m_pMesh[2] = CMesh::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), CMesh::TYPE_WAVE);

	// プレイヤーマネージャ―の取得
	m_pPlayerManager = CPlayerManager::Create(D3DXVECTOR3(0.0f, 1000.0f, 5.0f));
	// エネミーのマネージャーの取得
	m_pEnemyManager = CEnemy_Manager::Create();

	// スフィア
	CSphere *pSphere = CSphere::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f));

	// 陸
	m_pMesh[1] = CMesh::Create(D3DXVECTOR3(0.0f, -400.0f, 0.0f), CMesh::TYPE_GROUND);

	m_pRadar = nullptr;
	m_pRadar = CRadar::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetObjectinfo(), CRadar::RADAR_MAP);

	//サウンド生成
	CSound::PlaySound(CSound::SOUND_LABEL_BGM000);

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CGame::Uninit()
{
	// ランキングに現在のスコアを保存する
	CRanking::SetCurrentScore(m_pScore->GetScore());

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_pMesh[nCnt] = nullptr;
	}

	// スコアの終了処理
	m_pScore->Uninit();
	m_pScore = nullptr;

	m_PlayerUI->Release();

	//m_pPlayerManager->Uninit();

	Release();
}

//=========================================
// 更新処理
//=========================================
void CGame::Update()
{
	CInput *pKeyboard = CInput::GetKey();

	// プレイヤーマネージャ―の更新処理
	m_pPlayerManager->Update();

	// エネミーマネージャ―の更新処理
	m_pEnemyManager->Update();

	// プレイヤーUIの更新処理
	m_PlayerUI->Update();

	// ゲームが終了するかしないか
	//EnemyManage();

	if (m_bFinish)
	{
		// ゲームを終了しリザルト画面へ
		//モードの設定
		CFade::SetFade(CApplication::MODE_RESULT);
	}
}

//=========================================
// エネミーを削除する
//=========================================
void CGame::DeleteEnemy(CEnemy* pEnemy)
{
}

//=========================================
// ゲームセット
//=========================================
void CGame::EnemyManage()
{
}

//=========================================
// スコアの加算処理
//=========================================
void CGame::Add(int Score)
{
	m_pScore->Add(Score);
}