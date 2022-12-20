//=========================================
//
//	エネミーの処理
//	Author:冨所知生
//
//=========================================
#include "enemy.h"
#include "application.h"
#include "input.h"
#include "inputkeyboard.h"
#include "texture.h"
#include "game.h"
#include "model.h"
#include "model3D.h"

//=========================================
//グローバル変数
//=========================================
int nEnemyNumber = 0;

//=========================================
//コンストラクタ
//=========================================
CEnemy::CEnemy()
{
	SetObjectType(OBJECT_ENEMY);
	m_state = ENEMY_IDOL;
	SetHP(5);
}

//=========================================
//デストラクタ
//=========================================
CEnemy::~CEnemy()
{
}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CEnemy::Init(const D3DXVECTOR3 &pos)
{
	m_apModel[0] = new CModel3D;

	m_apModel[0]->SetModelID(3);
	m_apModel[0]->Init();
	m_apModel[0]->SetPos(pos);

	//拡大率の設定
	m_apModel[0]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//大きさの設定
	SetSize(m_apModel[0]->GetSize());

	m_scale = 1.0f;

	m_pos = pos;
	
	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CEnemy::Update()
{
	m_pos = m_apModel[0]->GetPos();
	m_rot = m_apModel[0]->GetRot();

	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerRot;

	Death();

	//プレイヤーの座標を取得
	while(0)
	{
		CObject *object = CObject::GetObjectTop();

		if (object != nullptr)
		{
			EObjType ObjType = object->GetObjectType();

			if (ObjType == OBJECT_PLAYER)
			{
				PlayerPos = object->GetPosition();
				PlayerRot = object->GetRot();
				break;
			}
		}
	}

	//=========================================
	//待機状態のエネミーの行動処理
	//=========================================
	if (m_state == ENEMY_IDOL)
	{
		m_rot.y += 0.01f;

		if (SearchEye(PlayerPos, m_pos, D3DX_PI / 2.0f, 50.0f, m_rot.y))
		{
			m_state = ENEMY_WARNNING;
		}
	}

	//=========================================
	// 戦闘状態のエネミーの行動処理
	//=========================================
	if (m_state == ENEMY_WARNNING)
	{
		// プレイヤーとエネミーの距離
		D3DXVECTOR3 posDiss = PlayerPos - m_pos;
		D3DXVECTOR3 rotDest = D3DXVECTOR3
		(atan2f(posDiss.x, posDiss.z),
			atan2f(posDiss.y, sqrtf((posDiss.x * posDiss.x) + (posDiss.z * posDiss.z))),
			0.0f);

		//エネミーの角度調節、プレイヤーの方向を見る
		m_rot.y += m_apModel[0]->NormalizeRot(rotDest.x - m_rot.y) * 0.01f;
		m_rot.z = m_apModel[0]->LimitedRot(-m_rot.y, 0.25f);
		m_rot.x = 0.1f * (-posDiss.y * 0.3f);
	}

	float EnemySpeed = 3.0f;

	D3DXVECTOR3 move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
		-(m_rot.x * EnemySpeed),
		cosf(m_rot.y) * EnemySpeed);

	//エネミーの移動
	m_pos += move;

	m_apModel[0]->SetRot(m_rot);
	// エネミーのモデルの座標設定
	m_apModel[0]->SetPos(m_pos);

	// エネミーの座標設定
	SetPosition(m_pos);
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CEnemy::Uninit()
{
	m_apModel[0]->Uninit();
	CObject::Release();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CEnemy::Draw()
{
	// 初期化
	m_apModel[0]->Draw();
}

//=========================================
// 死亡判定
//=========================================
void CEnemy::Death()
{
	int nLife = GetHP();

	if (nLife <= 0)
	{
		// エネミーの総数
		((CGame*)CApplication::GetModeObject())->DeleteEnemy(this);

		// 初期化
		CEnemy::Uninit();
	}
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CEnemy* CEnemy::Create(const D3DXVECTOR3 &pos)
{
	CEnemy* pCEnemy = nullptr;

	pCEnemy = new CEnemy;

	if (pCEnemy != nullptr)
	{
		pCEnemy->Init(pos);
		
		// エネミー番号の指定
		pCEnemy->SetNumber(nEnemyNumber);
		// エネミー番号の加算
		nEnemyNumber++;
	}

	return pCEnemy;
}