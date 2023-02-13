//=========================================
// game.cpp
// Author: 冨所知生
//=========================================
#include <algorithm>
#include "application.h"
#include "game.h"
#include "player_manager.h"
#include "enemy_manager.h"
#include "sphere.h"
#include "meshfield.h"
#include "score.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "enemy.h"
#include "camera.h"
#include "radar.h"
#include "number.h"
#include "render.h"
#include "time.h"
#include "score.h"
#include "sound.h"

bool CGame::m_bFinish = false;
CMesh* CGame::m_pMesh[3] = {};
CScore* CGame::m_pScore = nullptr;
CPlayerManager* CGame::pPlayerManager = nullptr;
CEnemy_Manager* CGame::pEnemyManager = nullptr;
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
	m_bFinish = false;

	/*CRender *pRender = CApplication::GetRender();
	pRender->SetFog(true, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0001f));*/

	//タイマーの生成
	CTime *pTime = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 200.0f, 50.0f, 0.0f));
	pTime->SetAlarm(1,0);

	// スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 50.0f, 0.0f));

	// 海
	m_pMesh[0] = CMesh::Create(D3DXVECTOR3(0.0f, -350.0f, 0.0f), CMesh::TYPE_SEA);
	m_pMesh[2] = CMesh::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), CMesh::TYPE_WAVE);

	pPlayerManager = CPlayerManager::Create(D3DXVECTOR3(0.0f, 1000.0f, 5.0f));
	pEnemyManager = CEnemy_Manager::Create();

	CSphere *pSphere = CSphere::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f));
	//CScore *pScore = CScore::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

	// 陸
	m_pMesh[1] = CMesh::Create(D3DXVECTOR3(0.0f, -400.0f, 0.0f), CMesh::TYPE_GROUND);

	m_pRadar = nullptr;
	m_pRadar = CRadar::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetObjectinfo(), CRadar::RADAR_MAP);

	// エネミー
	//m_EnemyList.emplace_back(pEnemy);

	//サウンド生成
	CSound::PlaySound(CSound::SOUND_LABEL_BGM000);

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CGame::Uninit()
{
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_pMesh[nCnt] = nullptr;
	}

	// スコアの終了処理
	m_pScore->Uninit();
	m_pScore = nullptr;

	pPlayerManager = nullptr;

	Release();
}

//=========================================
// 更新処理
//=========================================
void CGame::Update()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// プレイヤーマネージャ―の更新処理
	pPlayerManager->Update();

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