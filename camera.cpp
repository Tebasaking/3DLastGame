//**************************************************************************************************
//
// カメラ処理(camera.h)
// Auther：唐﨑結斗
// Author : 湯田海斗
// Author : 冨所知生
// 概要 : カメラの設定処理
//
//**************************************************************************************************

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "camera.h"
#include "application.h"
#include "render.h"
#include "input.h"
#include "mouse.h"
#include "inputkeyboard.h"
#include "move.h"
#include "calculation.h"
#include "player_manager.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
const float CCamera::CAMERA_NEAR = 1.0f;	// ニア
const float CCamera::CAMERA_FUR = 100000000.0f;	// ファー

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CCamera::CCamera() :
	m_posV(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// 視点
	m_posR(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// 注視点
	m_vecU(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// 上方向ベクトル
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// 向き
	m_rotMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),			 	// 移動方向
	m_quaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f)),	// クオータニオン
	m_viewType(TYPE_CLAIRVOYANCE),							// 投影方法の種別
	m_fDistance(0.0f),										// 視点から注視点までの距離
	m_fRotMove(0.0f)										// 移動方向
{
	m_mtxWorld = {};	// ワールドマトリックス
	m_mtxProj = {};		// プロジェクションマトリックス
	m_mtxView = {};		// ビューマトリックス
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 視点と注視点の間の距離を算出する
//=============================================================================
HRESULT CCamera::Init()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -150.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 固定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 視点と注視点の距離
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	// 角度の算出
	m_rot.y = 0.0f;
	m_rot.x = 0.0f;
	m_rot.z = 0.0f;

	// クォータニオンの適応
	D3DXQuaternionRotationYawPitchRoll(&m_quaternion, m_rot.y, m_rot.x, m_rot.z);

	// メモリの確保
	m_pRoll = new CMove;
	assert(m_pRoll != nullptr);
	m_pRoll->SetMoving(0.015f, 10.0f, 0.0f, 0.1f);

	m_mode = TYPE_FREE;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : 終了
//=============================================================================
void CCamera::Uninit(void)
{
	if (m_pRoll != nullptr)
	{// 終了処理
	 // メモリの解放
		delete m_pRoll;
		m_pRoll = nullptr;
	}
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新
//=============================================================================
void CCamera::Update(void)
{
	MouseMove();	// マウス移動

	m_mode = (CCamera::CAMERA_TYPE)CPlayerManager::GetMode();

	// 状態ごとに移動方法を変える
	switch (m_mode)
	{
	case TYPE_FREE:
		FreeMove();		// 移動
		break;
	case TYPE_SHOULDER:
		ShoulderMove();	// 肩越しモード
		break;
	}

	// キーボードの取得
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// エンターキーが押された
	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		// m_modeがFREEの時はSHOULDERに、SHOULDERの時はFREEにする
		m_mode = (m_mode == TYPE_FREE) ? TYPE_SHOULDER : TYPE_FREE;
	}
}

//=============================================================================
// カメラの設定
// Author : 唐﨑結斗
// 概要 : ビューマトリックスの設定
//=============================================================================
void CCamera::Set()
{// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);			// 行列初期化関数

	D3DXMATRIX mtxRot, mtxTrans;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_posV.x, m_posV.y, m_posV.z);		// 行列移動関数
	D3DXMatrixInverse(&mtxTrans, NULL, &mtxTrans);						// 逆行列に計算
	D3DXMatrixMultiply(&m_mtxView, &m_mtxView, &mtxTrans);				// 行列掛け算関数

	// 向きの反映
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);		// クオータニオンによる行列回転
	D3DXMatrixInverse(&mtxRot, NULL, &mtxRot);					// 逆行列に計算
	D3DXMatrixMultiply(&m_mtxView, &m_mtxView, &mtxRot);		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProj);			// 行列初期化関数

	switch (m_viewType)
	{
	case TYPE_CLAIRVOYANCE:
		// プロジェクションマトリックスの作成(透視投影)
		D3DXMatrixPerspectiveFovLH(&m_mtxProj,				// プロジェクションマトリックス
			D3DXToRadian(60.0f),							// 視野角
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// アスペクト比
			CAMERA_NEAR,									// ニア
			CAMERA_FUR);									// ファー
		break;

	case TYPE_PARALLEL:
		// プロジェクションマトリックスの作成(平行投影)
		D3DXMatrixOrthoLH(&m_mtxProj,						// プロジェクションマトリックス
			(float)SCREEN_WIDTH,							// 幅
			(float)SCREEN_HEIGHT,							// 高さ
			CAMERA_NEAR,									// ニア
			CAMERA_FUR);									// ファー
		break;

	default:
		assert(false);
		break;
	}

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);
}

//=============================================================================
// 回転の計算
// Author : 唐﨑結斗
// Author : YudaKaito
// 概要 : 
//=============================================================================
void CCamera::Rotate()
{
	// 入力情報の取得
	static const float MIN_MOUSE_MOVED = 3.0f;		// この値以上動かさないと反応しない

	if (!(D3DXVec3Length(&m_rotMove) > MIN_MOUSE_MOVED) && !(D3DXVec3Length(&m_rotMove) < -MIN_MOUSE_MOVED))
	{
		return;
	}

	// デッドゾーンの設定
	if (m_rotMove.x >= -MIN_MOUSE_MOVED && m_rotMove.x <= MIN_MOUSE_MOVED)
	{
		m_rotMove.x = 0.0f;
	}
	if (m_rotMove.y >= -MIN_MOUSE_MOVED && m_rotMove.y <= MIN_MOUSE_MOVED)
	{
		m_rotMove.y = 0.0f;
	}

	/* ↓指定した長さ以上で動かしてる↓ */

	static const float ROTATE_MOUSE_MOVED = 0.45f;	// 回転速度

	// 移動方向の算出
	D3DXVECTOR3 rollDir = (m_rotMove * (D3DX_PI / 180.0f) * ROTATE_MOUSE_MOVED);

	//m_pRoll->Moving(rollDir);

	// マウスのベクトル軸取得
	m_axisVec.y = rollDir.x;
	m_axisVec.x = -rollDir.y;

	D3DXVECTOR3 inverseVec = m_axisVec;

	D3DXVec3Normalize(&inverseVec, &inverseVec);

	m_VecGet = inverseVec;

	// X軸の回転
	{
		if (inverseVec.y != 0.0f)
		{
			// クオータニオンによる行列回転
			D3DXMATRIX mtxRot, mtxVec;
			D3DXMatrixTranslation(&mtxVec, inverseVec.x, inverseVec.y, inverseVec.z);		// 行列移動関数
			D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);			// クオータニオンを回転行列に変換
			D3DXMatrixMultiply(&mtxVec, &mtxVec, &mtxRot);					// 行列掛け算関数

			D3DXVECTOR3 axis;
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, mtxVec._42, 0.0f);
			D3DXVECTOR3 vecY = m_posR - m_posV;		// 視点から注視点の距離ベクトル
			D3DXVec3Normalize(&vecY, &vecY);		// 回転軸

			D3DXVec3Cross(&axis, &vec, &vecY);	// 外積で回転軸を算出。

			// クオータニオンの計算
			D3DXQUATERNION quaternion;
			//D3DXQuaternionRotationAxis(&quaternion, &axis, D3DXVec3Length(&m_pRoll->GetMove()));	// 回転軸と回転角度を指定

			D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// 回転軸と回転角度を指定

			// クオータニオンのノーマライズ
			D3DXQuaternionNormalize(&quaternion, &quaternion);

			// クオータニオンを適用
			m_quaternion *= quaternion;
		}
	}

	// Y軸の回転
	{
		D3DXVECTOR3 axis;

		if (inverseVec.x < 0.0f)
		{
			D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
			D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, 0.0f, -inverseVec.x);
			D3DXVec3Cross(&axis, &xAxis, &zAxis);	// 外積で回転軸を算出。
		}
		else
		{
			D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
			D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, 0.0f, inverseVec.x);
			D3DXVec3Cross(&axis, &xAxis, &zAxis);	// 外積で回転軸を算出。
		}

		if (inverseVec.x != 0.0f)
		{
			// クオータニオンの計算
			D3DXQUATERNION quaternion;

			D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// 回転軸と回転角度を指定

																	// クオータニオンのノーマライズ
			D3DXQuaternionNormalize(&quaternion, &quaternion);

			// クオータニオンを適用
			m_quaternion *= quaternion;
		}
	}

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
}

//=============================================================================
// 注視点の回転
// Author : 唐﨑結斗
// Author : YudaKaito
// 概要 : 
//=============================================================================
void CCamera::RPosRotate()
{
	m_posR = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, m_fDistance), m_posV, m_quaternion);
}

//=============================================================================
// 視点の回転
// Author : 唐﨑結斗
// Author : YudaKaito
// 概要 : 
//=============================================================================
void CCamera::VPosRotate()
{
	m_posV = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, -m_fDistance), m_posR, m_quaternion);
}

//=============================================================================
// カメラの自由移動
// Author : 唐﨑結斗
// Author : YudaKaito
// Author : 冨所知生
// 概要 : 
//=============================================================================
void CCamera::FreeMove(void)
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//移動キーが押された
	if (pKeyboard->GetPress(DIK_W))
	{
		MOVE_SPEED += 0.1f;
		CAMERA_MOVE_SPEED += 0.1f;
	}
	else
	{// プレイヤーが操作していないとき減速する
		MOVE_SPEED -= 0.01f;
		CAMERA_MOVE_SPEED -= 0.01f;
	}

	if (MOVE_SPEED >= 10.0f)
	{
		MOVE_SPEED = 10.0f;
	}
	if (CAMERA_MOVE_SPEED >= 10.0f)
	{
		CAMERA_MOVE_SPEED = 10.0f;
	}
	if (CAMERA_MOVE_SPEED <= 5.0f)
	{
		CAMERA_MOVE_SPEED = 5.0f;
	}
	if (MOVE_SPEED <= 5.0f)
	{
		MOVE_SPEED = 5.0f;
	}

		//常に前進し続ける
		m_fDistance -= MOVE_SPEED;
		VPosRotate();
		m_fDistance += MOVE_SPEED;
		RPosRotate();

	D3DXVec3Normalize(&move, &move);

	if (D3DXVec3Length(&move) != 0.0f)
	{
		// 視点位置の更新
		m_posV = m_posV + move * CAMERA_MOVE_SPEED;
		m_posR = m_posR + move * CAMERA_MOVE_SPEED;
	}
}

//=========================================
//　カメラの肩越し移動
//	Author : 冨所知生
//=========================================
void CCamera::ShoulderMove()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	/* 移動キーが押された*/
	if (pKeyboard->GetPress(DIK_W))
	{
		m_fDistance -= MOVE_SPEED;
		VPosRotate();
		m_fDistance += MOVE_SPEED;
		RPosRotate();
	}

	if (pKeyboard->GetPress(DIK_A))
	{// 移動キーが押された
		D3DXVECTOR3 Pythagoras = D3DXVECTOR3(m_posV.z - m_posR.z, 0.0f, m_posV.x - m_posR.x);

		move.x += Pythagoras.x;
		move.z += -Pythagoras.z;
	}
	if (pKeyboard->GetPress(DIK_S))
	{// 移動キーが押された
		m_fDistance += MOVE_SPEED;
		VPosRotate();
		m_fDistance -= MOVE_SPEED;
		RPosRotate();
	}
	if (pKeyboard->GetPress(DIK_D))
	{// 移動キーが押された
		D3DXVECTOR3 Pythagoras = D3DXVECTOR3(m_posV.z - m_posR.z, 0.0f, m_posV.x - m_posR.x);

		move.x += -Pythagoras.x;
		move.z += Pythagoras.z;
	}

	if (MOVE_SPEED >= 10.0f)
	{
		MOVE_SPEED = 10.0f;
	}
	if (CAMERA_MOVE_SPEED >= 10.0f)
	{
		CAMERA_MOVE_SPEED = 10.0f;
	}
	if (CAMERA_MOVE_SPEED <= 5.0f)
	{
		CAMERA_MOVE_SPEED = 5.0f;
	}
	if (MOVE_SPEED <= 5.0f)
	{
		MOVE_SPEED = 5.0f;
	}

	D3DXVec3Normalize(&move, &move);

	if (D3DXVec3Length(&move) != 0.0f)
	{
		// 視点位置の更新
		m_posV = m_posV + move * CAMERA_MOVE_SPEED;
		m_posR = m_posR + move * CAMERA_MOVE_SPEED;
	}

}

//=========================================
// マウスの移動
// Author : 唐﨑結斗
// Author : YudaKaito
// 概要	  :  
//=========================================
void CCamera::MouseMove(void)
{
	CMouse *pMouse = CApplication::GetMouse();

	// 回転のベクトルを設定。
	m_rotMove = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	// クリックの情報を保管
	bool hasLeftClick = pMouse->GetPress(CMouse::MOUSE_KEY_LEFT);
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	if (hasLeftClick)
	{
	/*左クリックしている最中回転する*/
		Rotate();
		RPosRotate();
	}

	if (hasRightClick)
	{
		Rotate();
		VPosRotate();
	}
}