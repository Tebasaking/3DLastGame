//=========================================
//
//	バレット3Dの処理
//	Author:冨所知生
//
//=========================================
#include "bullet3D.h"
#include "model3D.h"
#include "explosion.h"
#include "game.h"
#include "meshfield.h"
#include "application.h"
#include "sound.h"
#include "enemy.h"
#include "debug_proc.h"
#include "camera_player.h"
#include "calculation.h"
#include "missile_locus.h"
#include "missile_alert_manager.h"
#include "player3D.h"

//=========================================
//コンストラクタ
//=========================================
CBullet3D::CBullet3D()
{
	SetObjectType(OBJECT_PLAYER);
}

//=========================================
//デストラクタ
//=========================================
CBullet3D::~CBullet3D()
{
}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CBullet3D::Init(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	m_fAngleLimit = 0;

	m_MissileSpeed = 0;

	m_pModel = new CModel3D;

	// とりあえず距離を伸ばす
	m_length = 10000.0f;

	m_MissileCnt = 0;

	// Rotの設定===============================================

	D3DXVECTOR3 BulletPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	BulletPos = GetPosition();
	m_posTarget = m_TargetObj->GetPosition();

	D3DXVECTOR3 FellowRot = AtanRot(m_posTarget, BulletPos);

	m_pModel->SetRot(D3DXVECTOR3(FellowRot.x, FellowRot.y, 0.0f));

	//==========================================================

	//ミサイルアラートの生成====================================

	if (m_TargetObj != nullptr)
	{
		if (m_TargetObj->GetObjectType() == OBJECT_PLAYER)
		{// 狙われたターゲットがプレイヤーだった場合のみ生成する
			m_pAlert = nullptr;

			m_pAlert = CMissileAlertManager::Create(this);

			m_pAlert->SetObject(this);
		}
	}

	//==========================================================

	// 初期化
	m_pModel->Init();

	m_pModel->SetModelID(13);

	//拡大率の設定
	m_pModel->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CBullet3D::Update()
{
	// 古い座標の設定
	SetPosOld(m_pos);

	// 古い座標に現在の座標を代入する
	m_posOld = m_pos;

	// 弾の移動制御
	BulletMove();

	m_TargetObj;

	// 当たり判定
	if (Collision(m_TargetObj, false))
	{
		// 弾が目標オブジェクトと衝突したら消滅する処理
		CExplosion::Create(m_TargetObj->GetPosition(), m_quaternion);

		if (m_TargetObj != nullptr)
		{
			if (m_TargetObj->GetObjectType() == OBJECT_ENEMY)
			{// エネミーのステートを変化する
				CEnemy* pEnemy = (CEnemy*)m_TargetObj;

				pEnemy->SetState(CEnemy::ENEMY_WARNNING);

				m_TargetObj->ManageHP(-5);
			}
			else if (m_TargetObj->GetObjectType() == OBJECT_PLAYER)
			{
				CPlayer3D *pPlayer = (CPlayer3D*)m_TargetObj;

				if (!pPlayer->GetInvincible())
				{// 無敵判定
					m_TargetObj->ManageHP(-1);
					pPlayer->SetInvincible();
				}

				//カメラ情報の取得
				CApplication::GetCamera()->ShakeCamera(60.0f, 10.0f);
			}
		}
		Uninit();
	}
	
	// グラウンドの取得
	CMesh *pGround = CGame::GetGround();

	if (pGround != nullptr && m_TargetObj != nullptr)
	{
		// 陸の当たり判定
		if (pGround->Collision(&m_pos))
		{
			Uninit();
			CExplosion::Create(m_pos, m_quaternion);
		}
	}

	// 軌跡を生成するタイミング
	m_LocusCnt++;

	if (m_LocusCnt % 1 == 0)
	{
		// ミサイルの軌跡の生成
		CMissile_Locus::Create(m_pos, m_TargetObj);
	}

	//=========================================
	// ミサイルの自壊
	//=========================================
	m_MissileCnt++;

	if (m_MissileCnt >= 300)
	{
		Uninit();
	}
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CBullet3D::Uninit()
{
	if (m_pAlert != nullptr)
	{
		m_pAlert->Uninit();
		m_pAlert = nullptr;
	}

	// モデルの終了処理
	m_pModel->Uninit();
	delete m_pModel;
	m_pModel = nullptr;

	m_TargetObj = nullptr;
	m_pShooter = nullptr;

	//オブジェクトの解放処理
	CObject::Release();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CBullet3D::Draw()
{
	// デバイスの取得
	PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_WorldMtx);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);                // 行列移動関数
	D3DXMatrixMultiply(&m_WorldMtx, &m_WorldMtx, &mtxTrans);					// 行列掛け算関数

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_WorldMtx);

	// モデルの描画処理
	m_pModel->Draw();
}

//=====================================================================
// オブジェクトのクリエイト
// 引数 : 射手からの距離、クォータニオン、目標、射手、誘導のタイミング
//=====================================================================
CBullet3D* CBullet3D::Create(const D3DXVECTOR3 &pos ,const D3DXQUATERNION &quaternion, CObject *object, CObject *Shooter, int val, float Limit)
{
	int nNumCreate = m_nNumAll;

	CBullet3D* pCBullet3D = nullptr;
	CObject *pShooter = Shooter;

	pCBullet3D = new CBullet3D;

	if (pCBullet3D != nullptr)
	{
		pCBullet3D->m_TargetObj = object;
		pCBullet3D->m_pShooter = Shooter;
		pCBullet3D->Init(pCBullet3D->MtxPos(pos, quaternion, pShooter->GetPosition()));
		pCBullet3D->SetRot(pShooter->GetRot());
		pCBullet3D->SetQue(pShooter->GetQuaternion());
		pCBullet3D->SetSearchValue(val);
		pCBullet3D->SetAngleLimit(Limit);

		CSound::PlaySound(CSound::SOUND_SE_MISSILE);
	}

	return pCBullet3D;
}

//=========================================
// ロックオンの処理
//=========================================
D3DXVECTOR3 CBullet3D::LockOn()
{
	// 誘導カウントの加算
	m_SearchCnt++;

	//情報の取得
	D3DXVECTOR3 BulletPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	BulletPos = GetPosition();

	if (m_SearchCnt >= m_SearchValue)
	{// サーチカウントが一定の座標を超えた時
		m_posTarget = m_TargetObj->GetPosition();
		m_SearchCnt = 0;
	}

	// 敵のいる向き
	D3DXVECTOR3 sub = D3DXVECTOR3(0.0f, 0.0f, 0.f);
	D3DXVECTOR3 distance = m_posTarget - BulletPos;

	m_FllowRot = AtanRot(m_posTarget, BulletPos);

	m_length = D3DXVec3Length(&(BulletPos - m_posTarget));

	if (m_MissileSpeed >= m_length)
	{// ミサイルスピードがlength以上の時
		//m_MissileSpeed = 5.0f;
		m_SearchValue = 30;
	}

	sub.z = sinf(m_FllowRot.x) * cosf(m_FllowRot.y) * m_MissileSpeed;
	sub.x = sinf(m_FllowRot.x) * sinf(m_FllowRot.y) * m_MissileSpeed;
	sub.y = cosf(m_FllowRot.x)  * m_MissileSpeed;

	//CDebugProc::Print("BulletPos.y %.1f\n", BulletPos.y);
	//CDebugProc::Print("distance.y %.1f\n",distance.y);

	return sub;
}

//=========================================
// 弾の移動の制御
//=========================================
void CBullet3D::BulletMove()
{
	if (m_TargetObj != nullptr)
	{
		// 徐々にミサイルのスピードを速くする
		if (m_MissileSpeed <= 0.3f)
		{
			m_MissileSpeed += 0.1f;
		}
		else
		{
			m_MissileSpeed += 1.5f;
		}

		// 目標のマトリックスを取得
		D3DXMATRIX	*TargetMatrix = m_TargetObj->GetMtxWorld();

		// 目標座標
		m_TargetPos = m_TargetObj->GetPosition();

		m_FrontMoveCnt++;
		
		D3DXVECTOR3 MoveRot;
		D3DXVECTOR3 Pos;

		if (m_FrontMoveCnt <= 30)
		{
			if (m_pShooter != nullptr)
			{
				if (m_pShooter->GetObjectType() == CObject::OBJECT_PLAYER)
				{
					// クォータニオンを使用した移動の適応
					Pos = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, m_MissileSpeed), m_pos, m_quaternion);
				}
				else
				{
					// 移動角度の設定
					MoveRot = m_ShooterRot;

					D3DXVECTOR3 Rot = m_pShooter->GetRot();

					// 移動量
					Pos = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, m_MissileSpeed), m_pos, Rot);
				}
			}

			// 角度の設定
			m_FllowRot = AtanRot(Pos, m_pos);

			// 現在機体の正面方向
			D3DXVECTOR3 VecFront = Pos - m_pos;

			D3DXVec3Normalize(&VecFront, &VecFront);

			// 移動量
			m_move = VecFront * m_MissileSpeed;
		}
		else
		{
			D3DXVECTOR3 EnemyVec = m_TargetObj->GetPosition() - GetPosition();

			// ベクトルを算出
			D3DXVECTOR3 MoveVec = {};

			D3DXVec3Normalize(&MoveVec, &m_move);
			D3DXVec3Normalize(&EnemyVec, &EnemyVec);

			// 進行方向ベクトルから出した現在地から見たエネミーへの角度
			float AdvanceRot = acos((MoveVec.x * EnemyVec.x) + (MoveVec.y * EnemyVec.y) + (MoveVec.z * EnemyVec.z));

			if (AdvanceRot <= m_fAngleLimit)
			{
				m_state = LOCK_ON_STATE;

				AdvanceRot = AdvanceRot * (180 * D3DX_PI);

				D3DXVECTOR3 Vec = EnemyVec - MoveVec;

				// 1フレームに動く角度を設定する
				float OneFrameRadian = (8 * (180 / D3DX_PI));

				/*D3DXVec3Normalize(&Vec, &Vec);*/

				D3DXVECTOR3 AB = (Vec * D3DXVec3Length(&m_move)) / AdvanceRot * OneFrameRadian;

				D3DXVec3Normalize(&AB, &AB);

				m_move += AB;

				// 角度の設定
				m_FllowRot = AtanRot(m_pos + m_move, m_pos);
			}
			else
			{
				m_bAlert = true;
				m_state = IDOL_STATE;
			}
		}

		// 移動量を加算する
		m_pos += m_move;

		m_pModel->SetRot(D3DXVECTOR3(m_FllowRot.x, m_FllowRot.y, 0.0f));

		// 重力
		m_pos.y -= 3.0f;

		// 座標の設定
		SetPosition(m_pos);

		// モデルの座標を設定する
		m_pModel->SetPos(m_pos);

		// モデルの更新処理
		m_pModel->Update();
	}
}

//=========================================
// クォータニオンをRotに変換する
// Author : 冨所知生
// 概要 : クォータニオンをRotに変える
//=========================================
D3DXVECTOR3 CBullet3D::QuaternionChange()
{
	// クォータニオンを取得する
	D3DXVECTOR3 ChangeRot;

	float z1 = 2 * (m_quaternion.x * m_quaternion.y + m_quaternion.z * m_quaternion.w);
	float z2 = m_quaternion.x * m_quaternion.x - m_quaternion.y * m_quaternion.y - m_quaternion.z * m_quaternion.z - m_quaternion.w * m_quaternion.w;
	float z3 = z1 / z2;
	ChangeRot.z = atanf(z3);

	float y1 = 2 * (m_quaternion.x * m_quaternion.z - m_quaternion.y * m_quaternion.w);
	ChangeRot.y = atanf(y1);

	float x1 = 2 * (m_quaternion.x * m_quaternion.w + m_quaternion.y * m_quaternion.z);
	float x2 = m_quaternion.x * m_quaternion.x + m_quaternion.y * m_quaternion.y - m_quaternion.z * m_quaternion.z - m_quaternion.w * m_quaternion.w;
	float x3 = x1 / x2;
	ChangeRot.x = -atanf(x3);

	return ChangeRot;
}

//=========================================
//　ミサイルの追従処理
//=========================================
void CBullet3D::Follow()
{
	if (m_TargetObj != nullptr)
	{
		D3DXVECTOR3 EnemyVec = m_TargetObj->GetPosition() - GetPosition();

		// ベクトルを算出
		D3DXVECTOR3 MoveVec = {};

		D3DXVec3Normalize(&MoveVec, &m_move);
		D3DXVec3Normalize(&EnemyVec, &EnemyVec);

		float AdvanceRot = acos((MoveVec.x * EnemyVec.x) + (MoveVec.y * EnemyVec.z) + (MoveVec.z * EnemyVec.z));
	}
}