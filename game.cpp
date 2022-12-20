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

CMesh* CGame::m_pMesh = nullptr;
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
	pPlayerManager = CPlayerManager::Create(D3DXVECTOR3(0.0f, -0.0f, 5.0f));

	CSphere *pSphere = CSphere::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f));
	CScore *pScore = CScore::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
	m_pMesh = CMesh::Create(D3DXVECTOR3(0.0f, -350.0f, 0.0f));
	//CEnemy *pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	//pEnemy->SetMotion("data/MOTION/motion.txt");

	//// エネミー
	//m_EnemyList.emplace_back(pEnemy);

	return S_OK;
}

//=========================================
// 更新処理
//=========================================
void CGame::Update()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	
	// プレイヤーマネージャ―の更新処理
	pPlayerManager->Update();

	//// ゲームが終了するかしないか
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