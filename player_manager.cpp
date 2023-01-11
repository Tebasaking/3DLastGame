//=========================================
//
//	プレイヤーの処理
//	Author:冨所知生
//
//=========================================
#include "player3D.h"
#include "player_manager.h"
#include "application.h"
#include "inputkeyboard.h"

CPlayerManager::PLAYER_MODE CPlayerManager::m_mode = MODE_FLY;
CPlayer3D *CPlayerManager::m_pFly = nullptr;
CPlayer3D *CPlayerManager::m_pRobot = nullptr;
//=========================================
//コンストラクタ
//=========================================
CPlayerManager::CPlayerManager()
{
	m_mode = MODE_FLY;
}

//=========================================
//デストラクタ
//=========================================
CPlayerManager::~CPlayerManager()
{
}

//========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CPlayerManager::Init(const D3DXVECTOR3 &pos)
{
	m_pFly = new CPlayer3D;
	m_pRobot = new CPlayer3D;

	// 初期化処理
	m_pFly->Init(pos); 
	m_pRobot->Init(pos);

	// モードの設定
	m_pFly->SetMode((CPlayer3D::PLAYER_MODE)MODE_FLY);
	m_pRobot->SetMode((CPlayer3D::PLAYER_MODE)MODE_ROBOT);

	// モーションの設定
	m_pFly->SetMotion("data/MOTION/fly_motion.txt");
	m_pRobot->SetMotion("data/MOTION/motion.txt");

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CPlayerManager::Update()
{
	// キーボードの取得
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	
	// エンターキーが押された
	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		// m_modeがFLYの時はROBOTに、ROBOTの時はFLYにする
		m_mode = (m_mode == MODE_FLY) ? MODE_ROBOT : MODE_FLY;
	}
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CPlayerManager::Uninit()
{
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CPlayerManager::Draw()
{
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CPlayerManager* CPlayerManager::Create(const D3DXVECTOR3 &pos)
{
	CPlayerManager* pPlayerManager = nullptr;

	pPlayerManager = new CPlayerManager;

	if (pPlayerManager != nullptr)
	{
		pPlayerManager->Init(pos);
	}

	return pPlayerManager;
}