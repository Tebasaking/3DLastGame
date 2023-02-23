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
#include "enemy_manager.h"
#include "explosion.h"
#include "effect.h"
#include "player3D.h"
#include "player_manager.h"

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
	m_PlayerPos = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);

	m_apModel[0] = new CModel3D;

	m_apModel[0]->SetModelID(3);
	m_apModel[0]->Init();
	m_apModel[0]->SetPos(pos);
	m_state = ENEMY_WARNNING;

	m_pos = pos;

	//拡大率の設定
	m_apModel[0]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// 座標の設定
	SetPosition(pos);

	// 古い座標の設定
	SetPosOld(m_pos);
	
	// サイズの設定
	m_size = m_apModel[0]->GetMaterial()[0].size;

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
	auto FloatRandom = [](float fMax, float fMin)
	{
		return ((rand() / (float)RAND_MAX) * (fMax - fMin)) + fMin;
	};

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

	CObject *object = nullptr;
	CObject *PlayerObj = nullptr;

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		object = CObject::GetObjectTop(nCnt);

		//プレイヤーの座標を取得
		while (object)
		{
			if (object != nullptr)
			{
				EObjType ObjType = object->GetObjectType();

				if (ObjType == OBJECT_PLAYER)
				{
					m_PlayerPos = object->GetPosition();
					PlayerRot = object->GetRot();
					PlayerObj = object;
					break;
				}
			}
			object = object->GetObjectNext();
		}
	}

	Search();

	D3DXVECTOR3 rot = GetRot();

	//if (SearchEye(PlayerPos, m_pos, D3DX_PI * 0.5f, 100.0f, rot.y))
	//{
	//	m_state = ENEMY_WARNNING;
	//}

	float EnemySpeed = 8.0f;

	//=========================================
	// エネミーの現在の状態
	//=========================================
	//=========================================
	// 非戦闘状態の敵の行動処理
	//=========================================
	if (m_state == ENEMY_CAUTION)
	{
		rot.y += 0.01f;

		if (m_type == ENEMY_FLY)
		{
			// 移動量
			m_move = D3DXVECTOR3(sinf(rot.y) * EnemySpeed,
				-(rot.x * EnemySpeed),
				cosf(rot.y) * EnemySpeed);

			m_CntWar++;
		}
		if (m_CntWar >= 120 + (int)FloatRandom(1200,100))
		{
			m_CntWar = 0;
			m_state = ENEMY_WARNNING;
		}
	}

	if (m_state == ENEMY_IDOL)
	{
		rot.y += 0.01f;

		if (m_type == ENEMY_FLY)
		{
			// 移動量
			m_move = D3DXVECTOR3(sinf(rot.y) * EnemySpeed,
				-(rot.x * EnemySpeed),
				cosf(rot.y) * EnemySpeed);
		}

		if (m_type == ENEMY_GROUND)
		{
			D3DXVECTOR3 CollisionCheck = m_pos;

			if (!pGround->Collision(&CollisionCheck))
			{
				m_move = D3DXVECTOR3(sinf(rot.y) * EnemySpeed,
					-10.0f,
					cosf(rot.y) * EnemySpeed);
			}
			else
			{
				m_move = D3DXVECTOR3(sinf(rot.y) * EnemySpeed,
					-0.0f,
					cosf(rot.y) * EnemySpeed);
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

		D3DXVECTOR3 EnemyVec = m_PlayerPos - GetPosition();

		// ベクトルを算出
		D3DXVECTOR3 MoveVec = {};

		D3DXVec3Normalize(&MoveVec, &m_move);
		D3DXVec3Normalize(&EnemyVec, &EnemyVec);

		// 進行方向ベクトルから出した現在地から見たエネミーへの角度
		float AdvanceRot = acos((MoveVec.x * EnemyVec.x) + (MoveVec.y * EnemyVec.y) + (MoveVec.z * EnemyVec.z));

		float AdvanceRotVec = AdvanceRot * (180 * D3DX_PI);

		D3DXVECTOR3 Vec = EnemyVec / AdvanceRotVec;

		// 1フレームに動く角度を設定する
		float OneRadian = (1 * (180 / D3DX_PI));

		D3DXVec3Normalize(&Vec, &Vec);

		D3DXVECTOR3 A = Vec;

		D3DXVECTOR3 AB = Vec *  OneRadian;
		D3DXVec3Normalize(&AB, &AB);

		//m_m_move = AB * m_MissileSpeed;

		m_move = AB * EnemySpeed;

		// 角度の設定
		D3DXVECTOR3 rotDest = AtanRot(m_pos + m_move, m_pos);
		rotDest.x -= D3DX_PI * 0.5f;
		rotDest.z += D3DX_PI;

		D3DXVECTOR3 rotResult = rotDest - rot;
		
		rotResult = NormalizeRotXYZ(rotResult);
		
		// 回転の加算
		rot += rotResult * 0.01f;

		rot = NormalizeRotXYZ(rot);

		m_AttackCount++;

		// 飛行機だけ移動させる
		if (m_type == ENEMY_FLY)
		{
			m_move = D3DXVECTOR3(sinf(rot.y) * EnemySpeed,
				-(rot.x * EnemySpeed),
				cosf(rot.y) * EnemySpeed);
		}
		else if (m_type == ENEMY_GROUND)
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (AdvanceRot <= D3DX_PI * 0.25f)
		{
			// アタックカウントが300~400以上の時
			if (PlayerObj != nullptr && m_AttackCount >= 300 + FloatRandom(200,0))
			{
				// プレイヤーに対してミサイルを発射する
				Bullet(PlayerObj);

				if (m_type == ENEMY_FLY)
				{
					// エネミーの状態を待機状態にする
					m_state = ENEMY_CAUTION;
				}
			}
		}
	}

	// 移動量を加算する
	m_pos += m_move;

	if (pGround != nullptr)
	{
		if (m_type == ENEMY_FLY)
		{
			D3DXVECTOR3 GroundPos = m_pos;
			GroundPos.y -= 100.0f;

			// 陸の当たり判定
			if (pGround->Collision(&GroundPos) && m_state == ENEMY_DEATH)
			{
				CExplosion::Create(m_pos, m_quaternion);

				// エネミーの総数
				((CGame*)CApplication::GetModeObject())->DeleteEnemy(this);

				CApplication::GetGame()->GetEM()->Death(this);

				CGame::Add(100);

				// 初期化
				CEnemy::Uninit();
			}

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

	//死亡状態の判定
	if (m_state == ENEMY_DEATH)
	{
		// 角度の設定
		D3DXVECTOR3 rotDest = D3DXVECTOR3(D3DX_PI * 0.5f, rot.y, rot.z);
		D3DXVECTOR3 rotResult = rotDest - rot;

		rotResult = NormalizeRotXYZ(rotResult);

		// 回転の加算
		rot += rotResult * 0.01f;

		float EnemySpeed = 5.0f;

		m_move = D3DXVECTOR3(sinf(rot.y) * EnemySpeed,
			-(rot.x * EnemySpeed),
			cosf(rot.y) * EnemySpeed);

		// プレイヤーの後ろを求める
		D3DXVECTOR3 BackPos = MtxPosRot(D3DXVECTOR3(0.0f, 0.0f, -50.0f), rot, m_pos);

		auto FloatRandom = [](float fMax, float fMin)
		{
			return ((rand() / (float)RAND_MAX) * (fMax - fMin)) + fMin;
		};

		for (int i = 0; i < 10; i++)
		{
			D3DXVECTOR3 pos = GetPosition();

			pos.x += FloatRandom(2.5f, -2.5f);
			pos.y += FloatRandom(2.5f, -2.5f);
			pos.z += FloatRandom(2.5f, -2.5f);

			CEffect *pEffect = CEffect::Create(D3DXVECTOR3(pos.x + FloatRandom(1.0f, -1.0f), pos.y, pos.z + FloatRandom(1.0f, -1.0f)), 0, 0, CTexture::TEXTURE_PARTIClE);
			pEffect->SetSize(D3DXVECTOR3(15.0f, 15.0f, 0.0f));
			pEffect->SetMoveVec(D3DXVECTOR3(0.0f, FloatRandom(1.5f, -1.5f), FloatRandom(10.5f, -10.5f)));
			pEffect->SetLife(25);
			pEffect->SetRenderMode(CEffect::MODE_ADD);

			pEffect->SetColor(D3DXCOLOR(1.0f, FloatRandom(0.5f, 0.0f), 0.1f, 1.0f));

			pEffect->SetTex(CTexture::TEXTURE_PARTIClE);
			pEffect->SetQuaternion(m_quaternion);
		}
	}

	// 座標の設定
	SetPosition(m_pos);

	// エネミーの当たり判定
	EnemyCollision();

	// エネミーの回転の設定
	m_apModel[0]->SetRot(rot);

	// 回転の設定
	SetRotation(rot);

	// モーションの更新処理
	CMotionModel3D::Update();

	// カメラの視点
	//CDebugProc::Print("エネミーのスクリーン座標 : (%.3f , %.3f , %.3f )\n", m_pos.x, m_pos.y, m_pos.z);
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
	// デバイスの取得
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

	if (nLife <= 0 && m_state != ENEMY_DEATH)
	{
		CApplication::GetGame()->GetPM()->GetPlayer()->AddBullet();

		if (m_Target != nullptr)
		{
			// ターゲットの終了処理
			m_Target->Uninit();
		}

		if (m_Radar != nullptr)
		{
			// レーダーの終了処理
			m_Radar->Uninit();
		}

		m_state = ENEMY_DEATH;
	}
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CEnemy* CEnemy::Create(const D3DXVECTOR3 &pos, const EnemyType &type,const int &Wave)
{
	CEnemy* pCEnemy = nullptr;

	pCEnemy = new CEnemy;

	if (pCEnemy != nullptr)
	{
		pCEnemy->SetWave(Wave);
		pCEnemy->SetType(type);
		pCEnemy->SetPosition(pos);
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
	CBullet3D::Create(D3DXVECTOR3(50.0f, 50.0f, 0.0f), m_quaternion, obj, this, 60 , D3DX_PI * 0.1f);
	CBullet3D::Create(D3DXVECTOR3(-50.0f, 50.0f, 0.0f), m_quaternion, obj, this, 60 , D3DX_PI * 0.1f);

	m_AttackCount = 0;
}

//=========================================
// エネミー同士の当たり判定
//=========================================
void CEnemy::EnemyCollision()
{
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		CObject *object = CObject::GetObjectTop(nCnt);

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
}

//=========================================
// タイプとモーションの設定
//=========================================
void CEnemy::SetType(EnemyType type)
{
	// タイプの設定
	m_type = type;

	switch (m_type)
	{
	case ENEMY_FLY:
		SetMotion("data/MOTION/fly_motion.txt");
		break;

	case ENEMY_GROUND:
		SetMotion("data/MOTION/tank.txt");
		break;
	}
}

//=========================================
// プレイヤーが一定の範囲内に入ったか判定する処理
//=========================================
void CEnemy::Search()
{
	//// 視点と注視点の距離
	//D3DXVECTOR3 posDiss = m_pos - m_PlayerPos;
	//float fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	//if ((fDistance >= -1000.0f && fDistance <= 1000.0f ) && m_state != ENEMY_DEATH)
	//{// エネミーを敵対状態にする
	//	m_state = ENEMY_WARNNING;
	//}
}

//=========================================
// ターゲットを赤く染める
//=========================================
void CEnemy::TargetSetColor(D3DXCOLOR col)
{
	if(m_Target != nullptr)
	{ 
	m_Target->SetColor(col);
	}
}