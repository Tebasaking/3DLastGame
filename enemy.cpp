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
#include "target.h"
#include "debug_proc.h"
#include "meshfield.h"
#include "radar.h"

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
	SetHP(100);
	m_type = ENEMY_MAX;
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

	m_pos = pos;

	//拡大率の設定
	m_apModel[0]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// 座標の設定
	SetPosition(pos);

	//大きさの設定
	SetSize(m_apModel[0]->GetSize());

	m_scale = 1.0f;
	
	// モーションの初期化処理
	CMotionModel3D::Init(pos);

	m_Target = nullptr;

	//ターゲット(敵の強調表示)の設定
	m_Target = CTarget::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f),this);

	m_Target->SetObject(this);

	m_Radar = nullptr;

	// エネミーをレーダー上に表示させる
	m_Radar = CRadar::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetObjectinfo());

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CEnemy::Update()
{
	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerRot;

	// 死亡処理
	Death();

	// 座標の取得
	m_pos = GetPosition();

	// オブジェクトの取得
	CObject *object = CObject::GetObjectTop();

	//プレイヤーの座標を取得
	while(object)
	{
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
		object = object->GetObjectNext();
	}

	//=========================================
	//待機状態のエネミーの行動処理
	//=========================================
	/*if (m_state == ENEMY_IDOL)
	{*/
	float EnemySpeed = 3.0f;
	D3DXVECTOR3 move;

	if(m_type == ENEMY_FLY)
	{ 
		// 移動量
		move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
			-(m_rot.x * EnemySpeed),
			cosf(m_rot.y) * EnemySpeed);
	}

	if (m_type == ENEMY_GROUND)
	{
		// 移動量
		move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
			-10.0f,
			cosf(m_rot.y) * EnemySpeed);
	}

	m_rot.y += 0.01f;

	//=========================================
	//// 戦闘状態のエネミーの行動処理
	////=========================================
	//if (m_state == ENEMY_WARNNING)
	//{
	//	// プレイヤーとエネミーの距離
	//	D3DXVECTOR3 posDiss = PlayerPos - pos;
	//	D3DXVECTOR3 rotDest = D3DXVECTOR3
	//	(atan2f(posDiss.x, posDiss.z),
	//		atan2f(posDiss.y, sqrtf((posDiss.x * posDiss.x) + (posDiss.z * posDiss.z))),
	//		0.0f);

	//	//エネミーの角度調節、プレイヤーの方向を見る
	//	rot.y += m_apModel[0]->NormalizeRot(rotDest.x - rot.y) * 0.01f;
	//	rot.z = m_apModel[0]->LimitedRot(-rot.y, 0.25f);
	//	rot.x = 0.1f * (-posDiss.y * 0.3f);
	//}

	//エネミーの移動
	m_pos += move;

	// グラウンドの取得
	CMesh *pGround = CGame::GetGround();

	if (pGround != nullptr)
	{
		// 陸の当たり判定
		bool bCollision = pGround->Collision(&m_pos);
	}

	// エネミーの回転の設定
	m_apModel[0]->SetRot(m_rot);

	// 座標の設定
	SetPosition(m_pos);

	// 回転の設定
	SetRotation(m_rot);

	// モーションの更新処理
	CMotionModel3D::Update();

	// カメラの視点
	CDebugProc::Print("エネミーのスクリーン座標 : (%.3f , %.3f , %.3f )\n", m_pos.x, m_pos.y, m_pos.z);
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CEnemy::Uninit()
{
	// モデルの解放
	m_apModel[0]->Uninit();

	// モーションの終了処理
	CMotionModel3D::Uninit();

	CObject::Release();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CEnemy::Draw()
{
	PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;
	D3DXMATRIX mtxParents;

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;
	// マテリアルデータへのポインタ
	D3DXVECTOR3 scale(5.0f, 5.0f, 5.0f);

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_WorldMtx);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);                // 行列移動関数
	D3DXMatrixMultiply(&m_WorldMtx, &m_WorldMtx, &mtxTrans);					// 行列掛け算関数

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_WorldMtx);

	// 初期化
	m_apModel[0]->Draw();

	// モーションの描画処理
	CMotionModel3D::Draw();

	SetMtxWorld(m_WorldMtx);
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