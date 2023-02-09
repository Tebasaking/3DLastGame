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
#include "bullet3D.h"

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
	SetHP(10);
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

	// 古い座標の設定
	SetPosOld(m_pos);
	
	// サイズの設定
	m_size = m_apModel[0]->GetMaterial()->size;

	//大きさの設定
	SetSize(m_size);

	m_scale = 1.0f;

	// モーションの初期化処理
	CMotionModel3D::Init(pos);

	m_Target = nullptr;

	//ターゲット(敵の強調表示)の設定
	m_Target = CTarget::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f),this);

	m_Target->SetObject(this);

	m_Radar = nullptr;

	// エネミーをレーダー上に表示させる
	m_Radar = CRadar::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetObjectinfo(),CRadar::RADAR_ENEMY);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CEnemy::Update()
{
	// 古い座標の設定
	SetPosOld(m_pos);

	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerRot;

	// グラウンドの取得
	CMesh *pGround = CGame::GetGround();

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

	if (SearchEye(PlayerPos, m_pos, D3DX_PI * 0.5f, 100.0f, m_rot.y))
	{
		m_state = ENEMY_WARNNING;
	}

	// 移動量用
	D3DXVECTOR3 move;

	float EnemySpeed = 3.0f;

	//=========================================
	// エネミーの現在の状態
	//=========================================
	//=========================================
	// 非戦闘状態の敵の行動処理
	//=========================================
	if (m_state == ENEMY_IDOL)
	{
		m_rot.y += 0.01f;

		if (m_type == ENEMY_FLY)
		{
			// 移動量
			move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
				-(m_rot.x * EnemySpeed),
				cosf(m_rot.y) * EnemySpeed);
		}

		if (m_type == ENEMY_GROUND)
		{
			D3DXVECTOR3 CollisionCheck = m_pos;

			if (!pGround->Collision(&CollisionCheck))
			{
				move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
					-10.0f,
					cosf(m_rot.y) * EnemySpeed);
			}
			else
			{
				move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
					-0.0f,
					cosf(m_rot.y) * EnemySpeed);
			}
		}
	}

	//=========================================
	// 戦闘状態のエネミーの行動処理
	//=========================================
	if (m_state == ENEMY_WARNNING)
	{
		//情報の取得
		D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 posTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		
		Pos = GetPosition();
		posTarget = PlayerPos;

		// 敵のいる向き
		D3DXVECTOR3 sub = D3DXVECTOR3(0.0f, 0.0f, 0.f);
		// ターゲットと現在地の距離
		D3DXVECTOR3 distance = posTarget - Pos;

		D3DXVECTOR3 FellowRot = AtanRot(posTarget, Pos);
	
		sub.z = sinf(FellowRot.x) * cosf(FellowRot.y) * EnemySpeed;
		sub.x = sinf(FellowRot.x) * sinf(FellowRot.y) * EnemySpeed;
		sub.y = cosf(atan2f(Pos.y, distance.y))  * EnemySpeed;

		D3DXVECTOR3 rotDest = (D3DXVECTOR3(FellowRot.x - D3DX_PI * 0.5f, FellowRot.y, m_rot.z));
		D3DXVECTOR3 rotResult = rotDest - m_rot;

		rotResult = NormalizeRotXYZ(rotResult);
		
		// 回転の加算
		m_rot += rotResult * 0.01f;

		m_rot = NormalizeRotXYZ(m_rot);

		move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
			-0.0f,
			cosf(m_rot.y) * EnemySpeed);

		// アタックカウントを進める
		m_AttackCount++;

		// アタックカウントが300以上の時
		if (object != nullptr && m_AttackCount >= 300)
		{
			// プレイヤーに対してミサイルを発射する
			Bullet(object);
		}
	}

	// 移動量を加算する
	m_pos += move;

	if (pGround != nullptr)
	{
		if (m_type == ENEMY_FLY)
		{
			D3DXVECTOR3 GroundPos = m_pos;
			GroundPos.y -= 100.0f;
			// 陸の当たり判定
			pGround->Collision(&GroundPos);
			m_pos.y = GroundPos.y + 100.0f;
		}
		else if (m_type == ENEMY_GROUND)
		{
			D3DXVECTOR3 GroundPos = m_pos;
			GroundPos.y -= 100.0f;

			// 陸の当たり判定
			if (pGround->Collision(&GroundPos))
			{
				m_pos.y = GroundPos.y;
			}
		}
	}

	// 座標の設定
	SetPosition(m_pos);

	// エネミーの当たり判定
	EnemyCollision();

	// エネミーの回転の設定
	m_apModel[0]->SetRot(m_rot);

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

	// オブジェクトの解放
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

		CGame::Add(100);

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

//=========================================
// エネミーの弾発射処理
//=========================================
void CEnemy::Bullet(CObject *obj)
{
	// 両翼から弾を発射する
	CBullet3D::Create(D3DXVECTOR3(50.0f, 50.0f, 0.0f), m_quaternion, obj, this,60);
	CBullet3D::Create(D3DXVECTOR3(-50.0f, 50.0f, 0.0f), m_quaternion, obj, this,60);

	m_AttackCount = 0;
}

//=========================================
// エネミー同士の当たり判定
//=========================================
void CEnemy::EnemyCollision()
{
	// オブジェクトの取得
	CObject *object = CObject::GetObjectTop();

	//プレイヤーの座標を取得
	while (object)
	{
		if (object != nullptr)
		{
			EObjType ObjType = object->GetObjectType();

			if (ObjType == OBJECT_ENEMY && object != this)
			{
				SetCollision(Collision(object, true));
			}
		}
		object = object->GetObjectNext();
	}
}