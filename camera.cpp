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
#include "debug_proc.h"
#include "game.h"
#include "meshfield.h"
#include "object.h"
#include "joypad.h"
#include "player3D.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
const float CCamera::CAMERA_NEAR = 30.0f;		// ニア
const float CCamera::CAMERA_FUR = 100000000.0f;	// ファー

//=============================================================================
// コンストラクタ
// Author	: 唐﨑結斗
// 概要		: インスタンス生成時に行う処理
//=============================================================================
CCamera::CCamera() :
	m_posV(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// 視点
	m_posR(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// 注視点
	m_vecU(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// 上方向ベクトル
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// 向き
	m_rotMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),			 	// 移動方向
	m_quaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f)),	// クオータニオン
	m_viewType(TYPE_CLAIRVOYANCE),							// 投影方法の種別
	m_event(EVENT_NORMAL),
	m_fDistance(0.0f),										// 視点から注視点までの距離
	m_nCntFly(0.0f),
	m_fRotMove(0.0f)										// 移動方向
{
	m_mtxWorld = {};	// ワールドマトリックス
	m_mtxProj = {};		// プロジェクションマトリックス
	m_mtxView = {};		// ビューマトリックス
	m_Objectmode = (CObject::Object_mode)3;
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
	switch (m_Objectmode)
	{
	case CObject::NORMAL_MODE:
		m_posV = D3DXVECTOR3(0.0f, 1000.0f, -150.0f);
		m_posR = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 固定
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_viewport.MinZ = 0.0f;
		m_viewport.MaxZ = 1.0f;
		break;

	case CObject::RADAR_MODE:
		m_posV = D3DXVECTOR3(0.0f, 5000.0f, -150.0f);
		m_posR = D3DXVECTOR3(0.0f, 5000.0f, 0.0f);
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 固定
		m_rot = D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f);
		m_viewport.MinZ = 0.0f;
		m_viewport.MaxZ = 1.0f;
		m_viewType = TYPE_PARALLEL;

		break;
	}

	// 視点と注視点の距離
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

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
	if (m_event == EVENT_NORMAL)
	{
		// カメラのモードごとの処理
		switch (m_Objectmode)
		{
		case CObject::NORMAL_MODE:
			UpdateNormal();
			break;

		case CObject::RADAR_MODE:
			UpdateRadar();

			break;
		}

		D3DXVECTOR3 Result = m_Dest - m_rotMove;
		m_rotMove += Result * 0.25f;
	}
	else
	{
		FlightEvent();
	}

	CDebugProc::Print("カメラの座標 : (%f,%f,%f) \n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("カメラの回転 : (%f,%f,%f,%f) \n", m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
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

	// ビューポートの適応
	pDevice->SetViewport(&m_viewport);

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
			(float)SCREEN_WIDTH * 5,							// 幅
			(float)SCREEN_HEIGHT * 5,							// 高さ
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
	static const float MIN_MOUSE_MOVED = 2.0f;		// この値以上動かさないと反応しない

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

	if (m_mode == TYPE_SHOULDER && Limit_Used_Mouse())
	{
		rollDir.x = 0.0f;
	}

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
			if (m_Objectmode != CObject::RADAR_MODE)
			{
				// クオータニオンによる行列回転
				D3DXMATRIX mtxRot, mtxVec;
				D3DXMatrixTranslation(&mtxVec, inverseVec.x, inverseVec.y, inverseVec.z);		// 行列移動関数

				D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);							// クオータニオンを回転行列に変換

				D3DXVECTOR3 axis;
				D3DXVECTOR3 vecX;

				if (inverseVec.y >= 0.0f)
				{
					vecX = D3DXVECTOR3(1, 0, 0);
				}
				else if (inverseVec.y <= 0.0f)
				{
					vecX = D3DXVECTOR3(-1, 0, 0);
				}

				D3DXVec3TransformCoord(&axis, &vecX, &mtxRot);

				// クオータニオンの計算
				D3DXQUATERNION quaternion;
				//D3DXQuaternionRotationAxis(&quaternion, &axis, D3DXVec3Length(&m_pRoll->GetMove()));	// 回転軸と回転角度を指定

				D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// 回転軸と回転角度を指定

				// クオータニオンのノーマライズ
				D3DXQuaternionNormalize(&quaternion, &quaternion);

				// クオータニオンを適用
				m_quaternion *= quaternion;
			}
			else
			{
				if (m_mode == TYPE_SHOULDER)
				{
					D3DXVECTOR3 axis;

					if (inverseVec.x < 0.0f)
					{
						D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
						D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, -inverseVec.x, 0.0f);
						D3DXVec3Cross(&axis, &xAxis, &zAxis);	// 外積で回転軸を算出。
					}
					else
					{
						D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
						D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, inverseVec.x, 0.0f);
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
			}
		}
	}

	switch (m_mode)
	{
	case TYPE_SHOULDER:
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
	break;

	case TYPE_FREE:
		// Z軸の回転
	{
		if (m_Objectmode != CObject::RADAR_MODE)
		{
			D3DXVECTOR3 axis;

			if (inverseVec.x < 0.0f)
			{
				D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
				D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, inverseVec.x, 0.0f);
				D3DXVec3Cross(&axis, &xAxis, &zAxis);	// 外積で回転軸を算出。
			}
			else
			{
				D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
				D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, -inverseVec.x, 0.0f);
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
	}
	break;
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

	// グラウンドの取得
	CMesh *pGround = CGame::GetGround();

	if (pGround != nullptr)
	{// 陸の当たり判定
		CPlayer3D *pPlayer = CPlayerManager::GetPlayer();

		if (pPlayer->GetCollision())
		{
			m_posV.y = pPlayer->GetPosition().y + 50.0f;

			pGround->Collision(&m_posV);
			m_posR.y = pPlayer->GetPosition().y;
		}
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

	if (m_bUp)
	{// 上昇処理
		Up();
	}

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
	CPlayer3D *pPlayer = CPlayerManager::GetRobot();
	// グラウンドの取得
	CMesh *pGround = CGame::GetGround();

	D3DXVECTOR3 Result = {};

	if (pGround != nullptr)
	{
		if (!m_bUp)
		{// 陸の当たり判定
			if (pPlayer->GetCollision())
			{
				Result.y = (pPlayer->GetPosition().y + 110.0f) - m_posV.y;

				m_posV.y += Result.y;

				m_Gravity = 0;
				pGround->Collision(&m_posV);
				m_posR.y = pPlayer->GetPosition().y;
			}
			else
			{// 重力の適応
			m_Gravity += 0.3f;

			// カメラを下降させる
			m_posV.y -= m_Gravity;
			VPosRotate();
			m_posR.y -= m_Gravity;

			pGround->Collision(&m_posV);
			}
		}
	}
}

//=========================================
// マウスの移動
// Author : 唐﨑結斗
// Author : YudaKaito
//=========================================
void CCamera::MouseMove(void)
{
	CMouse *pMouse = CApplication::GetMouse();

	// 回転のベクトルを設定。
	m_Dest = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	// クリックの情報を保管
	bool hasLeftClick = pMouse->GetPress(CMouse::MOUSE_KEY_LEFT);
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	if (hasRightClick)
	{
		Rotate();
		VPosRotate();
	}
	if (hasLeftClick)
	{
		/*左クリックしている最中回転する*/
		Rotate();
		RPosRotate();
	}
}

//=========================================
// ジョイパッドの移動
// Author : 冨所知生
// 概要 : ジョイパッド使用時のカメラの旋回
//=========================================
void CCamera::JoyPadMove(void)
{
	CJoypad *pJoypad = CApplication::GetJoy();
	
	if (pJoypad->GetUseJoyPad() >= 1)
	{
		// 回転のベクトルを設定。
		m_Dest.x = pJoypad->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0) * pJoypad->GetStick(CJoypad::JOYKEY_LEFT_STICK, 0).y * 100.0f;

		if ((pJoypad->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0) >= D3DX_PI * 0.25f && pJoypad->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0) <= D3DX_PI * 0.75f) ||
			(pJoypad->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0) >= -D3DX_PI * 0.75f && pJoypad->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0) <= -D3DX_PI * 0.25f))
		{
			// 回転のベクトルを設定。
			m_Dest.y = pJoypad->GetStick(CJoypad::JOYKEY_LEFT_STICK, 0).x * 100.0f;
		}
	
	Rotate();
	VPosRotate();
	}
}

//==================================================
// ビューポートの大きさ設定
// Author : 冨所知生
// 引数 : 画面左上の座標X,Y、幅、高さ
//==================================================
void CCamera::SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight)
{
	//引数を代入
	m_viewport.X = X;				//ビューポートの左上X座標
	m_viewport.Y = Y;				//ビューポートの左上Y座標
	m_viewport.Width = fWidth;		//ビューポートの幅
	m_viewport.Height = fHeight;	//ビューポートの高さ
}

//==================================================
// ビューポートの拡縮
// Author : 冨所知生
// 引数 : 開始位置X、開始位置Y、幅、高さ
//==================================================
void CCamera::AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight)
{
	//------------------------------
	// 幅の加算
	//------------------------------
	if (m_viewport.Width < SCREEN_WIDTH - 1.0f)
	{//幅がスクリーン内なら
	 //幅の加算
		m_viewport.Width += fWidth;

		if (m_viewport.X > 0)
		{//ビューポートの左上が画面の左上より大きいなら
			m_viewport.X += X;	//ビューポートの左上座標を移動
		}
	}

	//------------------------------
	// 高さの加算
	//------------------------------
	if (m_viewport.Height < SCREEN_HEIGHT - 1.0f)
	{//幅がスクリーン内なら
	 //高さの加算
		m_viewport.Height += fHeight;

		if (m_viewport.Y > 0)
		{//ビューポートの左上が画面の左上より大きいなら
			m_viewport.Y += Y;	//ビューポートの左上座標を移動
		}
	}
}

//=========================================
//	通常カメラの描画処理
//	Author: 冨所知生
//=========================================
void CCamera::UpdateNormal()
{
	// キーボードの取得
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	JoyPadMove();	// ジョイパッド移動

	m_mode = (CCamera::CAMERA_TYPE)CPlayerManager::GetMode();

	// 状態ごとに移動方法を変える
	switch (m_mode)
	{
	case TYPE_FREE:
		MouseMove();		// マウス移動
		FreeMove();			// 移動
		break;

	case TYPE_SHOULDER:
		if (m_bWork)
		{// マウスの移動を可能にする
			MouseMove();
		}

		ShoulderMove();	// 肩越しモード

		if (!m_bWork)
		{// カメラワーク
			CameraWork(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f));
		}

		break;
	}

	//==================================================================================

	// オブジェクトの取得
	CObject *object = CObject::GetObjectTop();

	D3DXVECTOR3 PlayerPos = {};
	D3DXVECTOR3	PlayerRot = {};

	//プレイヤーの座標を取得
	while (object)
	{
		if (object != nullptr)
		{
			CObject::EObjType ObjType = object->GetObjectType();

			if (ObjType == CObject::OBJECT_PLAYER)
			{
				PlayerPos = object->GetPosition();
				PlayerRot = object->GetRot();
				break;
			}
		}
		object = object->GetObjectNext();
	}

	if (object != nullptr)
	{
		// エンターキーが押された
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			switch (m_mode)
			{
			case TYPE_FREE:
				m_mode = TYPE_SHOULDER;
				break;

			case TYPE_SHOULDER:
				m_bWork = false;

				m_mode = TYPE_FREE;

				// 1.0f浮かせる処理
				m_posV.y += 100.0f;
				VPosRotate();
				m_posR.y += 100.0f;

				// 飛行イベント開始
				m_event = EVENT_FLY;

				break;
			}
		}
	}
}

//=========================================
//	レーダー用カメラの処理
//	Author : 冨所知生
//=========================================
void CCamera::UpdateRadar()
{
	CPlayer3D *pPlayer = CPlayerManager::GetPlayer();

	CMouse *pMouse = CApplication::GetMouse();

	// クリックの情報を保管
	bool hasLeftClick = pMouse->GetPress(CMouse::MOUSE_KEY_LEFT);
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	// 回転のベクトルを設定。
	m_Dest = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		m_posV = D3DXVECTOR3(PlayerPos.x, m_posV.y, PlayerPos.z);
		m_posR = D3DXVECTOR3(PlayerPos.x, m_posR.y, PlayerPos.z);
	}

	if (hasRightClick)
	{
		m_quaternion = D3DXQUATERNION(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);

		Rotate();
		RPosRotate();
	}
}

//=========================================
//	飛行イベントの処理
//	Author : 冨所知生
//=========================================
void CCamera::FlightEvent()
{
	// 飛行をカウント
	m_nCntFly++;

	// カメラを上昇させる
	m_posV.y += 5.0f;
	VPosRotate();
	m_posR.y += 5.0f;

	D3DXQUATERNION Dest = D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f) - m_quaternion;
	m_quaternion += Dest * 0.02f;

	if (m_nCntFly >= 120)
	{
		m_nCntFly = 0;
		m_event = EVENT_NORMAL;
	}
}

//=========================================
// マウスを利用したカメラの制限
// Author : 冨所知生
//=========================================
bool CCamera::Limit_Used_Mouse()
{
	CMouse *pMouse = CApplication::GetMouse();
	D3DXVECTOR3 axis = {};

	// クリックの情報を保管
	bool hasLeftClick = pMouse->GetPress(CMouse::MOUSE_KEY_LEFT);
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	if (hasRightClick)
	{
		if (m_rotMove.x > 0.0f)
		{
			m_MouseMove++;
		}
		else if (m_rotMove.x < 0.0f)
		{
			m_MouseMove--;
		}

		// 下方向の上限、上方向の上限
		if (m_MouseMove >= 5.0f || m_MouseMove <= -10.0f)
		{
			return true;
		}
	}

	return false;
}

//=========================================
// カメラワークの処理
// Author : 冨所知生
//=========================================
void CCamera::CameraWork(D3DXQUATERNION que)
{
	if (m_quaternion == D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f))
	{
		return;
	}

	m_quaternion = GetQuaternion();

	// クォータニオンのデスト
	D3DXQUATERNION Result = m_quaternion - que;

	D3DXQuaternionNormalize(&Result, &Result);

	m_quaternion += Result * 0.1f;

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	MouseMove();

	m_nCntCameraWork++;

	if (m_nCntCameraWork >= 120)
	{
		m_bWork = true;
		m_nCntCameraWork = 0;
	}
}

//=========================================
// 上昇処理
// Author : 冨所知生
//=========================================
void CCamera::Up()
{
	m_nCntCameraWork++;

	// カメラを上昇させる
	m_posV.y += 10.0f;
	VPosRotate();
	m_posR.y += 10.0f;

	if (m_nCntCameraWork >= 60)
	{
		m_bUp = false;
		m_nCntCameraWork = 0;
	}
}