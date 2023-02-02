//=========================================
//
//	プレイヤーの処理
//	Author:冨所知生
//
//=========================================
#include "player3D.h"
#include "camera.h"
#include "application.h"
#include "bullet3D.h"
#include "input.h"
#include "inputkeyboard.h"
#include "texture.h"
#include "billboard.h"
#include "Object2D.h"
#include "mouse.h"
#include "game.h"
#include "meshfield.h"
#include "bullet.h"
#include "target.h"
#include "model3D.h"
#include "player_manager.h"
#include "radar.h"
#include "motion.h"
#include "enemy.h"
#include "bullet.h"
#include "parts.h"
#include "debug_proc.h"
#include "sound.h"
#include "explosion.h"

int BulletDelay = 0;

//=========================================
//コンストラクタ
//=========================================
CPlayer3D::CPlayer3D()
{
	SetObjectType(OBJECT_PLAYER);
	m_mode = MODE_MAX;
	m_quaternion = D3DXQUATERNION(1.0f, 1.0f, 1.0f, 1.0f);
	m_Nearest_object = nullptr;
	m_nNumHandParts = 10;
}

//=========================================
//デストラクタ
//=========================================
CPlayer3D::~CPlayer3D()
{
}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CPlayer3D::Init(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	m_apModel[0] = new CModel3D;

	m_apModel[0]->SetModelID(0);
	m_apModel[0]->Init();
	m_apModel[0]->SetPos(pos);

	//拡大率の設定
	m_apModel[0]->SetSize(D3DXVECTOR3(1.0f,1.0f,1.0f));

	//大きさの設定
	m_size = m_apModel[0]->GetMaterial()->size;

	m_Radar = nullptr;

	// プレイヤーをレーダー上に表示させる
	m_Radar = CRadar::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetObjectinfo(), CRadar::RADAR_PLAYER);

	//=========================================
	// 人型モデルの読み込み
	//=========================================
	m_pRobot = new CModel3D;

	m_pRobot->SetModelID(1);
	m_pRobot->Init();
	m_pRobot->SetPos(pos);
	m_pRobot->SetSize(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	m_pAttack = new CModel3D;
	m_pAttack->Init();
	m_pAttack->SetPos(pos);
	m_pAttack->SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pAttack->SetSize(m_pAttack->GetMaterial()->size);

	//拡大率の設定
	/*m_apModel[0]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));*/

	//大きさの設定
	m_size = m_pRobot->GetMaterial()->size;

	SetSize(m_size);

	SetPosition(pos);

	CMotionModel3D::Init(pos);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CPlayer3D::Update()
{
	// 古い座標の設定
	SetPosOld(m_pos);

	switch (m_mode)
	{
	case MODE_FLY:
		// 飛行形態の更新処理
		UpdateFly();
		break;

	case MODE_ROBOT:
		// 人型形態の更新処理
		UpdateRob();
		break;
	}

	// ロックオン処理
	LockOn();

	// 座標の設定
	m_apModel[0]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	// 座標の設定
	m_pRobot->SetPos(m_pos);

	// 座標の設定
	SetPosition(m_pos);

	// 飛行モデルの更新処理
	for (int nCnt = 0; nCnt < 1; nCnt++)
	{
		m_apModel[nCnt]->Update();
	}

	// ロボットモデルの更新処理
	m_pRobot->Update();

	// モーションの更新処理
	CMotionModel3D::Update();
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CPlayer3D::Uninit()
{
	for (int nCnt = 0; nCnt < 1; nCnt++)
	{
		m_apModel[nCnt]->Uninit();
	}

	// ロボットモデルの終了処理
	m_pRobot->Uninit();

	// モーションの終了処理
	CMotionModel3D::Uninit();

	//オブジェクトの解放処理
	CObject::Release();

}

//=========================================
//オブジェクトの描画処理
//=========================================
void CPlayer3D::Draw()
{
	// マネージャ―のモードと一致した時
	if (m_mode == (CPlayer3D::PLAYER_MODE)CPlayerManager::GetMode())
	{
		PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

		// 計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans, mtxScale;

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_WorldMtx);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);                // 行列移動関数
		D3DXMatrixMultiply(&m_WorldMtx, &m_WorldMtx, &mtxTrans);					// 行列掛け算関数

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_WorldMtx);

		switch (m_mode)
		{
		case MODE_FLY:
			// 飛行形態の更新処理
			m_apModel[0]->Draw();
			break;

		case MODE_ROBOT:
			// 人型形態の更新処理
			m_pRobot->Draw();
			break;
		}

		// モーションの描画処理
		CMotionModel3D::Draw();
	}
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CPlayer3D* CPlayer3D::Create(const D3DXVECTOR3 &pos)
{
	int nNumCreate = m_nNumAll;

	CPlayer3D* pCPlayer3D = nullptr;

	pCPlayer3D = new CPlayer3D;

	if (pCPlayer3D != nullptr)
	{
		pCPlayer3D->Init(pos);
	}

	return pCPlayer3D;
}

//=========================================
// ロックオン処理
// 概要 : 画面上で最も近い敵を狙う
//=========================================
void CPlayer3D::LockOn()
{
	CObject *object = CObject::GetObjectTop();

	// 比較用最大サイズ
	int MAX_SIZE = 0;

	//=========================================
	// 現在存在するターゲットのサイズを比較し、
	// 一番大きいターゲットのオブジェクトを、
	// 最も近いオブジェクトとして保存する。
	//=========================================
	while (object)
	{
		if (object != nullptr)
		{
			EObjType ObjType = object->GetObjectType();

			if (ObjType == OBJECT_TARGET)
			{
				CTarget *pTarget = (CTarget*)object;

				if (MAX_SIZE <= pTarget->GetSize())
				{
					// 比較用に現在の最大サイズを保存
					MAX_SIZE = pTarget->GetSize();

					// 最大サイズのオブジェクトの保存
					m_Nearest_object = pTarget->GetTargetObject();
				}
			}
		}
		object = object->GetObjectNext();
	}
}

//=========================================
// プレイヤーの回転処理
// Author : 冨所知生
// 概要 : プレイヤーの回転
//=========================================
void CPlayer3D::Rotate()
{
	//カメラ情報の取得
	CCamera *pCamera = CApplication::GetCamera();

	m_quaternion = pCamera->GetQuaternion();

	SetQuaternion(m_quaternion);

}

//=========================================
//	飛行状態のプレイヤーの処理
//	Author : 冨所知生
//	概要 : プレイヤーの飛行状態
//=========================================
void CPlayer3D::UpdateFly()
{
	// クォータニオンを取得する
	D3DXQUATERNION quaternion = m_apModel[0]->GetQuaternion();

	// 目標のpos
	D3DXVECTOR3	m_posDest;
	//カメラ
	CCamera *pCamera = CApplication::GetCamera();

	m_pos = m_apModel[0]->GetPos();				// 座標の取得
	m_posDest = pCamera->GetPosR();				// カメラの座標の取得

	D3DXVECTOR3 m_posResult = m_posDest - m_pos;

	m_pos.x += m_posResult.x / 5;
	m_pos.z += m_posResult.z / 5;
	m_pos.y += m_posResult.y / 5;

	m_pos = MtxPos(D3DXVECTOR3(0.0f, -5.0f, 0.0f), quaternion, m_pos);

	int MOVE_SPEED = 5.0f;

	// 回転
	Rotate();

	//ロックオン処理
	LockOn();

	// 姿勢制御処理
	//Attitude();

	// 弾の発射処理
	Bullet(m_pos);

	// 座標の設定
	m_apModel[0]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	GroundCollison();
}

//=========================================
// 人型形態の更新処理
//=========================================
void CPlayer3D::UpdateRob()
{
	// プレイヤーの回転処理
	Rotate();

	// 目標のpos
	D3DXVECTOR3	m_posDest;

	CCamera *pCamera = CApplication::GetCamera();

	m_pos = m_apModel[0]->GetPos();				// 座標の取得
	m_posDest = pCamera->GetPosR();				// カメラの座標の取得

	m_posDest.y -= 100.0f;

	D3DXVECTOR3 m_posResult = m_posDest - m_pos;
	D3DXVECTOR3 posDiss = m_pos - pCamera->GetPosV();

	m_pos.x += m_posResult.x / 5;
	m_pos.z += m_posResult.z / 5;
	m_pos.y += m_posResult.y / 5;

	// 弾の発射処理
	Bullet(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z));

	// 地面との当たり判定
	GroundCollison();

	// ジャンプ処理
	Jump();

	// 攻撃処理
	Slash();

	if (m_bMotion)
	{
		m_MotionCnt++;

		if (m_MotionCnt >= 60)
		{
			m_bMotion = false;
			m_MotionCnt = 0;
		}
	}

	// 移動処理
	Move();

	if (!m_bMotion && !m_bMove)
	{
		CMotion *pMotion = GetMotion();

		pMotion->SetNumMotion(0);
	}

	// 座標の設定
	m_pRobot->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
}

//=========================================
// 弾の発射処理
//=========================================
void CPlayer3D::Bullet(D3DXVECTOR3 pos)
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	if (m_Nearest_object && m_mode == (CPlayer3D::PLAYER_MODE)CPlayerManager::GetMode())
	{
		if (!(m_Nearest_object->GetSize().x == 0.0f && m_Nearest_object->GetSize().y == 0.0f))
		{
			if (pKeyboard->GetPress(DIK_L))
			{
				BulletDelay++;

				if (BulletDelay >= 10)
				{
					int BulletSpeed = 50;

					// 両翼から弾を発射する
					CBullet3D::Create(D3DXVECTOR3(50.0f, 0.0f, 0.0f), m_quaternion, m_Nearest_object, this, 30);
					CBullet3D::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), m_quaternion, m_Nearest_object, this, 30);

					BulletDelay = 0;
				}
			}
		}
	}
}

//=========================================
// 姿勢制御処理
//=========================================
void CPlayer3D::Attitude()
{
	//// マウスの取得
	//CMouse *pMouse = CApplication::GetMouse();

	////マウスの移動量
	//D3DXVECTOR3 MouseMove = pMouse->GetMouseMove();

	//// 入力情報の取得
	//static const float MIN_MOUSE_MOVED = 2.0f;		// この値以上動かさないと反応しない

	//if (!(!(D3DXVec3Length(&MouseMove) > MIN_MOUSE_MOVED) && !(D3DXVec3Length(&MouseMove) < -MIN_MOUSE_MOVED)))
	//{
	//	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	//	if (hasRightClick)
	//	{
	//		if (pMouse->GetMouseMove().x > 0.0f)
	//		{//マウスが右方向に行ったとき機体を左に傾ける
	//			m_MouseMove.x -= 0.01f;
	//		}
	//		else if (pMouse->GetMouseMove().x < 0.0f)
	//		{//マウスが左方向に行ったとき機体を右に傾ける
	//			m_MouseMove.x += 0.01f;
	//		}
	//		else
	//		{
	//			//マウスを動かしてないときに徐々にマウスを戻してく処理
	//			if (m_MouseMove.x >= 0.0f)
	//			{
	//				m_MouseMove.x -= 0.01f;
	//			}
	//			else if (m_MouseMove.x <= 0.0f)
	//			{
	//				m_MouseMove.x += 0.01f;
	//			}
	//		}

	//		//角度の制限
	//		if (m_MouseMove.x >= D3DX_PI * 0.35f)
	//		{
	//			m_MouseMove.x = D3DX_PI * 0.35f;
	//		}
	//		//角度の制限
	//		if (m_MouseMove.x <= D3DX_PI * -0.35f)
	//		{
	//			m_MouseMove.x = D3DX_PI * -0.35f;
	//		}

	//		D3DXVECTOR3 rot = GetRotation();

	//		SetRotation(D3DXVECTOR3(rot.x, rot.y, m_MouseMove.x));

	//		////回転
	//		//m_apModel[0]->SetRot(D3DXVECTOR3(rot.x, rot.y, m_MouseMove.x));
	//	}
	//}
	//else
	//{

	//	D3DXVECTOR3 rot = GetRotation();

	//	rot.z += (0.0f - rot.z) * 0.05f;

	//	SetRotation(D3DXVECTOR3(rot.x, rot.y, rot.z));

	//	m_MouseMove.x = rot.z;
	//}
}

//=========================================
// 近接攻撃処理
//=========================================
void CPlayer3D::Slash()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	CMotion *pMotion = GetMotion();

	if (pKeyboard->GetTrigger(DIK_Z) && !m_bMotion)
	{
		CSound::PlaySound(CSound::SOUND_SE_MARSHALL_ATTACK);

		// モーションの設定
		pMotion->SetNumMotion(2);

		m_bMotion = true;
	}

	if (m_MotionCnt == 30)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (pMotion != nullptr)
		{// 手のオブジェクトの位置
			CParts *pHand = pMotion->GetParts(m_nNumHandParts);
			D3DXMATRIX mtxParts = pHand->GetMtxWorld();
			D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mtxParts);
		}

		// 攻撃の当たり判定
		m_pAttack->SetPos(pos);

		// オブジェクトの取得
		CObject *pObj = CObject::GetObjectTop();
		CEnemy *pEnemy = nullptr;

		D3DXVECTOR3 EnemyPos = {};
		D3DXVECTOR3 EnemySize = {};

		//プレイヤーの座標を取得
		while (pObj)
		{
			if (pObj != nullptr)
			{
				EObjType ObjType = pObj->GetObjectType();

				if (ObjType == OBJECT_ENEMY)
				{
					pEnemy = dynamic_cast<CEnemy*> (pObj);

					EnemyPos = pEnemy->GetPosition();
					EnemySize = pEnemy->GetSize() * 10.0f;

					D3DXVECTOR3 size = m_pAttack->GetSize() * 5;

					D3DXVECTOR3 SizeTarget = pEnemy->GetSize();

					if ((pos.z - size.z) < (EnemyPos.z + SizeTarget.z)
						&& (pos.z + size.z) > (EnemyPos.z - SizeTarget.z)
						&& (pos.x - size.x) < (EnemyPos.x + SizeTarget.x)
						&& (pos.x + size.x) > (EnemyPos.x - SizeTarget.x)
						&& (pos.y - size.y) < (EnemyPos.y + SizeTarget.y)
						&& (pos.y + size.y) > (EnemyPos.y - SizeTarget.y))
					{// モデル内にいる(XYZ軸)
						pEnemy->ManageHP(-10);
						// 弾が目標オブジェクトと衝突したら消滅する処理
						CExplosion::Create(m_pos, m_quaternion);
						break;
					}
				}
			}
			pObj = pObj->GetObjectNext();
		}
	}
}

//=========================================
// ジャンプ処理
//=========================================
void CPlayer3D::Jump()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	CCamera *pCamera = CApplication::GetCamera();
	CMotion *pMotion = GetMotion();

	if (pKeyboard->GetTrigger(DIK_SPACE) && !m_bMotion)
	{
		// モーションの設定
		pMotion->SetNumMotion(3);

		m_bMotion = true;
		m_bJump = true;

		pCamera->SetUp(true);
	}
}

//=========================================
// 地面との当たり判定
//=========================================
void CPlayer3D::GroundCollison()
{
	// オブジェクトの取得
	CObject *pMesh = CObject::GetObjectTop();
	CMesh *pGround = nullptr;

	//プレイヤーの座標を取得
	while (pMesh)
	{
		if (pMesh != nullptr)
		{
			EObjType ObjType = pMesh->GetObjectType();

			if (ObjType == OBJECT_MESH)
			{
				pGround = dynamic_cast<CMesh*> (pMesh);

				if (pGround->GetType() == CMesh::TYPE_GROUND)
				{
					break;
				}
			}
		}
		pMesh = pMesh->GetObjectNext();
	}

	if (pGround != nullptr)
	{
		// 陸の当たり判定
		if (pGround->Collision(&m_pos))
		{
			if (!m_bMotion)
			{
				m_bJump = false;
				m_bCollision = true;
			}
		}
		else
		{
			m_bCollision = false;
		}

		// 当たり判定の設定
		SetCollision(m_bCollision);
	}
}

//=========================================
// 移動
//=========================================
void CPlayer3D::Move()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	CMotion *pMotion = GetMotion();

	if (m_mode != (CPlayer3D::PLAYER_MODE)CPlayerManager::GetMode())
	{
		return;
	}

	if ((pKeyboard->GetPress(DIK_W) || pKeyboard->GetPress(DIK_A) || pKeyboard->GetPress(DIK_D) || pKeyboard->GetPress(DIK_S)) && !m_bMotion && !m_bJump)
	{
		m_StepCnt++;
		m_bMove = true;
	}
	else
	{
		m_bMove_Motion_Check = false;
		m_bMove = false;
	}

	if (m_bMove && !m_bMove_Motion_Check)
	{
		m_bMove_Motion_Check = true;
		pMotion->SetNumMotion(1);
	}

	if (m_StepCnt == 18)
	{
		m_StepCnt = 0;
		CSound::PlaySound(CSound::SOUND_SE_STEP);
	}
}