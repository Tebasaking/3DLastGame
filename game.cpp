//=========================================
// game.cpp
// Author: 冨所知生
//=========================================
#include <algorithm>
#include "application.h"
#include "game.h"
#include "player_manager.h"
#include "sphere.h"
#include "meshfield.h"
#include "score.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "enemy.h"
#include "camera.h"

CMesh* CGame::m_pMesh[3] = {};
CPlayerManager* CGame::pPlayerManager = nullptr;
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
	pPlayerManager = CPlayerManager::Create(D3DXVECTOR3(0.0f, 1000.0f, 5.0f));

	CSphere *pSphere = CSphere::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f));
	//CScore *pScore = CScore::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
	
	// 海
	m_pMesh[0] = CMesh::Create(D3DXVECTOR3(0.0f, -350.0f, 0.0f), CMesh::TYPE_SEA);

	//// 陸
	//m_pMesh[1] = CMesh::Create(D3DXVECTOR3(0.0f, -400.0f, 0.0f), CMesh::TYPE_GROUND);

	// 戦闘機
	CEnemy *pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 1000.0, 100.0f));
	pEnemy->SetType(CEnemy::ENEMY_FLY);
	pEnemy->SetMotion("data/MOTION/fly_motion.txt");

	// タンク
	CEnemy *pTank = CEnemy::Create(D3DXVECTOR3(100.0f,0.0f,1000.0f));
	pTank->SetType(CEnemy::ENEMY_GROUND);
	pTank->SetMotion("data/MOTION/tank.txt");

	// エネミー
	m_EnemyList.emplace_back(pEnemy);

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
}

//=========================================
// エネミーを削除する
//=========================================
void CGame::DeleteEnemy(CEnemy* pEnemy)
{
	// リストからpEnemyを削除する
	auto itEnd = std::remove(std::begin(m_EnemyList), std::end(m_EnemyList), pEnemy);

	// 本当の意味で要素を取り除く
	m_EnemyList.erase(itEnd, std::cend(m_EnemyList)); 
}

//=========================================
// ゲームセット
//=========================================
void CGame::EnemyManage()
{
	if (m_EnemyList.size() <= 0)
	{
      	CFade::SetFade(CApplication::MODE_RESULT);
	}
}