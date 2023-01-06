//=========================================
//
//	プレイヤーの処理
//	Author:冨所知生
//
//=========================================
#include "player3D.h"
#include "camera.h"
#include "application.h"
#include "input.h"
#include "inputkeyboard.h"
#include "texture.h"
#include "billboard.h"
#include "Object2D.h"
#include "mouse.h"
#include "game.h"
#include "meshfield.h"
#include "bullet.h"
#include "model3D.h"
#include "player_manager.h"

int BulletDelay = 0;

//=========================================
//コンストラクタ
//=========================================
CPlayer3D::CPlayer3D()
{
	SetObjectType(OBJECT_PLAYER);
	m_mode = MODE_MAX;
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
	m_size = m_apModel[0]->GetSize();

	//=========================================
	// 人型モデルの読み込み
	//=========================================
	m_pRobot = new CModel3D;

	m_apModel[0]->SetModelID(1);
	m_apModel[0]->Init();
	m_apModel[0]->SetPos(pos);

	//拡大率の設定
	m_apModel[0]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//大きさの設定
	m_size = m_pRobot->GetSize();

	SetPosition(pos);

	CMotionModel3D::Init(pos);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CPlayer3D::Update()
{
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
//=========================================
void CPlayer3D::LockOn()
{
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

	m_apModel[0]->SetQuaternion(m_quaternion);
	m_pRobot->SetQuaternion(m_quaternion);
}

//=========================================
//	飛行状態のプレイヤーの処理
//	Author : 冨所知生
//	概要 : プレイヤーの飛行状態
//=========================================
void CPlayer3D::UpdateFly()
{
	// 目標のpos
	D3DXVECTOR3	m_posDest;
	//カメラ
	CCamera *pCamera = CApplication::GetCamera();

	m_pos = m_apModel[0]->GetPos();				// 座標の取得
	m_posDest = pCamera->GetPosR();				// カメラの座標の取得

	m_posDest.y -= 20.0f;
	D3DXVECTOR3 m_posResult = m_posDest - m_pos;
	D3DXVECTOR3 posDiss = m_pos - pCamera->GetPosV();

	m_pos.x += m_posResult.x / 5;
	m_pos.z += m_posResult.z / 5;
	m_pos.y += m_posResult.y / 5;

	int MOVE_SPEED = 5.0f;

	// 回転
	Rotate();

	//ロックオン処理
	LockOn();

	// 姿勢制御処理
	Attitude();

	// 弾の発射処理
	Bullet(m_pos);

	// 座標の設定
	m_apModel[0]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	// グラウンドの取得
	CMesh *pGround = CGame::GetGround();

	if (pGround != nullptr)
	{
		// 陸の当たり判定
		bool bCollision = pGround->Collision(&m_pos);

		// 当たり判定の設定
		SetCollision(bCollision);
	}
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

	m_posDest.y -= 30.0f;
	D3DXVECTOR3 m_posResult = m_posDest - m_pos;
	D3DXVECTOR3 posDiss = m_pos - pCamera->GetPosV();

	m_pos.x += m_posResult.x / 5;
	m_pos.z += m_posResult.z / 5;
	m_pos.y += m_posResult.y / 5;

	// グラウンドの取得
	CMesh *pGround = CGame::GetGround();

	if (pGround != nullptr)
	{
		// 陸の当たり判定
		bool bCollision = pGround->Collision(&m_pos);

		// 当たり判定の設定
		SetCollision(bCollision);
	}

	// 弾の発射処理
	Bullet(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z));

	// 座標の設定
	m_pRobot->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
	//m_posDest.y -= 10.0f;
	//m_posDest.z += 100.0f;

	/*D3DXVECTOR3 m_posResult = m_posDest - m_pos;

	m_pos.x += m_posResult.x / 5;
	m_pos.z += m_posResult.z / 5;
	m_pos.y += m_posResult.y / 5;*/
}

//=========================================
// 弾の発射処理
//=========================================
void CPlayer3D::Bullet(D3DXVECTOR3 pos)
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// マネージャ―のモードと一致した時
	if (m_mode == (CPlayer3D::PLAYER_MODE)CPlayerManager::GetMode())
	{
		//弾の発射
		if (pKeyboard->GetPress(DIK_SPACE))
		{
			BulletDelay++;

			//バレットの発射レート
			if (BulletDelay >= 10)
			{
				int BulletSpeed = 50;

				// カメラ情報の取得
				CCamera *pCamera = CApplication::GetCamera();

				// 人型の時プレイヤーの向きが変わらないのでカメラのクォータニオンを入れる
				CBullet::Create(pos, pCamera->GetQuaternion());

				BulletDelay = 0;
			}
		}
	}
}

//=========================================
// 姿勢制御処理
//=========================================
void CPlayer3D::Attitude()
{
	// マウスの取得
	CMouse *pMouse = CApplication::GetMouse();

	//マウスの移動量
	D3DXVECTOR3 MouseMove = pMouse->GetMouseMove();

	bool hasLeftClick = pMouse->GetPress(CMouse::MOUSE_KEY_LEFT);

	if (hasLeftClick)
	{
		if (pMouse->GetMouseMove().x > 0.0f)
		{//マウスが右方向に行ったとき機体を左に傾ける
			m_MouseMove.x -= 0.01f;
		}
		else if (pMouse->GetMouseMove().x < 0.0f)
		{//マウスが左方向に行ったとき機体を右に傾ける
			m_MouseMove.x += 0.01f;
		}
		else
		{
			//マウスを動かしてないときに徐々にマウスを戻してく処理
			if (m_MouseMove.x >= 0.0f)
			{
				m_MouseMove.x -= 0.01f;
			}
			else if (m_MouseMove.x <= 0.0f)
			{
				m_MouseMove.x += 0.01f;
			}
		}

		//角度の制限
		if (m_MouseMove.x >= D3DX_PI * 0.35f)
		{
			m_MouseMove.x = D3DX_PI * 0.35f;
		}
		//角度の制限
		if (m_MouseMove.x <= D3DX_PI * -0.35f)
		{
			m_MouseMove.x = D3DX_PI * -0.35f;
		}

		//D3DXVECTOR3 rot = GetRotation();

		////回転
		//SetRotation(D3DXVECTOR3(rot.x, rot.y, m_MouseMove.x));
	}
}