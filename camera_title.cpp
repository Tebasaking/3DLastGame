//**************************************************************************************************
//
// カメラ処理(camera.h)
// Auther：唐?ｱ結斗
// Author : 湯田海都
// Author : 冨所知生
// 概要 : カメラの設定処理
//
//**************************************************************************************************

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "camera_player.h"
#include "camera_title.h"
#include "application.h"
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
#include "sound.h"
#include "utility.h"

//=============================================================================
// コンストラクタ
// Author	: 唐?ｱ結斗
// 概要		: インスタンス生成時に行う処理
//=============================================================================
CCameraTitle::CCameraTitle() :
	m_rotMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),				// 移動方向
	m_fRotMove(0.0f),										// 移動方向
	m_nCntCameraWork(0),
	m_Gravity(0),
	m_nCntFly(0)
{
}

//=============================================================================
// デストラクタ
// Author : 唐?ｱ結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCameraTitle::~CCameraTitle()
{

}

//=============================================================================
// 初期化
// Author : 唐?ｱ結斗
// 概要 : 視点と注視点の間の距離を算出する
//=============================================================================
HRESULT CCameraTitle::Init(D3DXVECTOR3 pos)
{
	CCamera::Init(pos);

	m_posR = D3DXVECTOR3(pos);
	m_quaternion = D3DXQUATERNION(-0.099572f, -0.867236f, -0.120078f, 0.472830f);
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;

	// 視点と注視点の距離
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	m_mode = TYPE_FREE;

	m_nCntMoveSound = 10000;

	m_bUp = false;
	m_bWork = false;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐?ｱ結斗
// 概要 : 終了
//=============================================================================
void CCameraTitle::Uninit(void)
{
}

//=============================================================================
// 更新
// Author : 唐?ｱ結斗
// 概要 : 更新
//=============================================================================
void CCameraTitle::Update(void)
{
	m_Destquaternion = D3DXQUATERNION(m_quaternion);

	m_quaternion += (m_Destquaternion - m_quaternion) * 0.1f;
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	CDebugProc::Print("camera_title\n");
	CDebugProc::Print("カメラの座標 : (%f,%f,%f) \n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("カメラの回転 : (%f,%f,%f,%f) \n", m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
}

//=============================================================================
// 回転の計算
// Author : 唐?ｱ結斗
// Author : YudaKaito
// 概要 : 
//=============================================================================
void CCameraTitle::Rotate()
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

				D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// 回転軸と回転角度を指定

																		// クオータニオンのノーマライズ
				D3DXQuaternionNormalize(&quaternion, &quaternion);

				// クオータニオンを適用
				m_Destquaternion *= quaternion;
				D3DXQuaternionNormalize(&m_Destquaternion, &m_Destquaternion);
			}
		}

		if (m_Objectmode == CObject::RADAR_MODE)
		{// レーダーモードの時
			CPlayer3D *pPlayer = CPlayerManager::GetPlayer();

			if (pPlayer != nullptr)
			{
				D3DXQUATERNION PlayerQua = pPlayer->GetQuaternion();

				m_quaternion = D3DXQUATERNION(m_quaternion.x, PlayerQua.x, m_quaternion.z, m_quaternion.w);
			}
		}
	}

	switch (m_mode)
	{
	case TYPE_SHOULDER:
		// Y軸の回転
	{
		D3DXVECTOR3 axis;
		// クオータニオンによる行列回転
		D3DXMATRIX mtxRot, mtxVec;

		D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);
		D3DXVec3TransformCoord(&axis, &D3DXVECTOR3(1, 0, 0), &mtxRot);

		D3DXVECTOR3 vecX;

		if (inverseVec.x >= 0.0f)
		{
			vecX = D3DXVECTOR3(0, -1, 0);
		}
		else if (inverseVec.x <= 0.0f)
		{
			vecX = D3DXVECTOR3(0, 1, 0);
		}

		D3DXVec3TransformCoord(&axis, &vecX, &mtxRot);

		if (inverseVec.x != 0.0f)
		{
			// クオータニオンの計算
			D3DXQUATERNION quaternion;

			D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// 回転軸と回転角度を指定

																	// クオータニオンのノーマライズ
			m_Destquaternion *= quaternion;
		}
	}
	break;

	case TYPE_FREE:
		// Z軸の回転
	{
		if (m_Objectmode != CObject::RADAR_MODE)
		{

			D3DXVECTOR3 axis;
			// クオータニオンによる行列回転
			D3DXMATRIX mtxRot, mtxVec;

			D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);
			D3DXVec3TransformCoord(&axis, &D3DXVECTOR3(0, 0, 1), &mtxRot);

			D3DXVECTOR3 vecX;

			if (inverseVec.x >= 0.0f)
			{
				vecX = D3DXVECTOR3(0, 0, 1);
			}
			else if (inverseVec.x <= 0.0f)
			{
				vecX = D3DXVECTOR3(0, 0, -1);
			}

			D3DXVec3TransformCoord(&axis, &vecX, &mtxRot);

			if (inverseVec.x != 0.0f)
			{
				// クオータニオンの計算
				D3DXQUATERNION quaternion;

				D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// 回転軸と回転角度を指定

				// クオータニオンのノーマライズ
				D3DXQuaternionNormalize(&quaternion, &quaternion);

				m_Destquaternion *= quaternion;
				D3DXQuaternionNormalize(&m_Destquaternion, &m_Destquaternion);
			}
		}
	}
	break;
	}

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&m_Destquaternion, &m_Destquaternion);
}

//=============================================================================
// カメラの自由移動
// Author : 唐?ｱ結斗
// Author : YudaKaito
// Author : 冨所知生
// 概要 : 
//=============================================================================
void CCameraTitle::FreeMove(void)
{
	CInput *pKeyboard = CInput::GetKey();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		m_nCntMoveSound++;

		if (m_nCntMoveSound >= 60 * 19)
		{
			CSound::PlaySound(CSound::SOUND_SE_FLIGHT);
			m_nCntMoveSound = 0;
		}
	}

	//移動キーが押された
	if (pKeyboard->Press(DIK_W) || pKeyboard->Press(JOYPAD_R2))
	{// 加速処理
		MOVE_SPEED += 0.1f;
		CAMERA_MOVE_SPEED += 0.1f;
	}
	else
	{// プレイヤーが操作していないとき減速する
		MOVE_SPEED -= 0.1f;
		CAMERA_MOVE_SPEED -= 0.1f;
	}

	if (MOVE_SPEED >= 20.0f)
	{
		MOVE_SPEED = 20.0f;
	}
	if (CAMERA_MOVE_SPEED >= 20.0f)
	{
		CAMERA_MOVE_SPEED = 20.0f;
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
void CCameraTitle::ShoulderMove()
{
	CInput *pKeyboard = CInput::GetKey();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// SEの停止
	CSound::StopSound(CSound::SOUND_SE_FLIGHT);

	if (m_bUp)
	{// 上昇処理
		Up();
	}

	/* 移動キーが押された*/
	if (pKeyboard->Press(DIK_W) || pKeyboard->Press(JOYPAD_R2))
	{
		m_fDistance -= MOVE_SPEED;
		VPosRotate();
		m_fDistance += MOVE_SPEED;
		RPosRotate();
	}

	if (pKeyboard->Press(DIK_A))
	{// 移動キーが押された
		D3DXVECTOR3 Pythagoras = D3DXVECTOR3(m_posV.z - m_posR.z, 0.0f, m_posV.x - m_posR.x);

		move.x += Pythagoras.x;
		move.z += -Pythagoras.z;
	}

	if (pKeyboard->Press(DIK_S))
	{// 移動キーが押された
		m_fDistance += MOVE_SPEED;
		VPosRotate();
		m_fDistance -= MOVE_SPEED;
		RPosRotate();
	}

	if (pKeyboard->Press(DIK_D))
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

				m_Gravity = 0.0f;

				// 地面の当たり判定
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
// Author : 唐?ｱ結斗
// Author : YudaKaito
//=========================================
void CCameraTitle::MouseMove(void)
{
}

//=========================================
// ジョイパッドの移動
// Author : 冨所知生
// 概要 : ジョイパッド使用時のカメラの旋回
//=========================================
void CCameraTitle::JoyPadMove(void)
{
}

//=========================================
//	飛行イベントの処理
//	Author : 冨所知生
//=========================================
void CCameraTitle::FlightEvent()
{
	// 飛行をカウント
	m_nCntFly++;

	// カメラを上昇させる
	m_posV.y += 5.0f;
	VPosRotate();
	m_posR.y += 5.0f;

	D3DXQUATERNION Dest = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f) - m_quaternion;
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
bool CCameraTitle::Limit_Used_Mouse()
{

	return false;
}

//=========================================
// カメラワークの処理
// Author : 冨所知生
//=========================================
void CCameraTitle::CameraWork(D3DXQUATERNION que)
{
	m_quaternion = GetQuaternion();

	if (m_quaternion.w >= 0.99999f)
	{
		return;
	}

	// クォータニオンのデスト
	D3DXQUATERNION Result = m_quaternion - que;

	D3DXQuaternionNormalize(&Result, &Result);

	m_Destquaternion += Result;

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&m_Destquaternion, &m_Destquaternion);

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
void CCameraTitle::Up()
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
