//**************************************************************************************************
//
// カメラ処理(camera.h)
// Author ; 冨所知生
// 概要 : カメラの設定処理
//
//**************************************************************************************************

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "camera_Game.h"
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
#include "sound.h"
#include "utility.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
const float CCamera::CAMERA_NEAR = 30.0f;		// ニア
const float CCamera::CAMERA_FUR = 100000000.0f;	// ファー

//=============================================================================
// コンストラクタ
// Author	: 冨所知生
// 概要		: インスタンス生成時に行う処理
//=============================================================================
CCamera_Game::CCamera_Game() :
	m_event(EVENT_NORMAL),
	m_nCntFly(0.0f)
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
CCamera_Game::~CCamera_Game()
{

}

//=============================================================================
// 初期化
// Author : 冨所知生
// 概要 : 視点と注視点の間の距離を算出する
//=============================================================================
HRESULT CCamera_Game::Init(D3DXVECTOR3 pos)
{
	switch (m_Objectmode)
	{
	case CObject::NORMAL_MODE:
		m_posV = pos;
		m_posR = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 固定
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
		m_viewport.MinZ = 0.0f;
		m_viewport.MaxZ = 1.0f;
		break;

	case CObject::RADAR_MODE:
		m_posV = D3DXVECTOR3(0.0f, 5000.0f, -150.0f);
		m_posR = D3DXVECTOR3(0.0f, 5000.0f, 0.0f);
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 固定
		m_rot = D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f);
		m_quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
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

	m_mode = TYPE_FREE;

	m_nCntMoveSound = 10000;


	return S_OK;
}

//=============================================================================
// 終了
// Author : 冨所知生
// 概要 : 終了
//=============================================================================
void CCamera_Game::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// 更新
// Author : 冨所知生
// 概要 : 更新
//=============================================================================
void CCamera_Game::Update(void)
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

	m_quaternion += (m_Destquaternion - m_quaternion) * 0.1f;
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	CDebugProc::Print("カメラの座標 : (%f,%f,%f) \n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("カメラの回転 : (%f,%f,%f,%f) \n", m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
}

//=============================================================================
// カメラの自由移動
// Author : 冨所知生
// 概要 : 
//=============================================================================
void CCamera_Game::FreeMove(void)
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
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
	if (pKeyboard->GetPress(DIK_W))
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
void CCamera_Game::ShoulderMove()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// SEの停止
	CSound::StopSound(CSound::SOUND_SE_FLIGHT);

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
// Author : 唐﨑結斗
// Author : YudaKaito
//=========================================
void CCamera_Game::MouseMove(void)
{
	CMouse *pMouse = CApplication::GetMouse();

	// 回転のベクトルを設定。
	m_Dest = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	// クリックの情報を保管
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	if (hasRightClick)
	{
		Rotate();
		VPosRotate();
	}
}

//=========================================
//	通常カメラの描画処理
//	Author: 冨所知生
//=========================================
void CCamera_Game::UpdateNormal()
{
	// キーボードの取得
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	JoyPadMove();	// ジョイパッド移動

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
//	飛行イベントの処理
//	Author : 冨所知生
//=========================================
void CCamera_Game::FlightEvent()
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
bool CCamera_Game::Limit_Used_Mouse()
{
	CMouse *pMouse = CApplication::GetMouse();
	D3DXVECTOR3 axis = {};

	// クリックの情報を保管
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
void CCamera_Game::CameraWork(D3DXQUATERNION que)
{
	if (m_quaternion == D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f))
	{
		return;
	}

	m_quaternion = GetQuaternion();

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
void CCamera_Game::Up()
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

//=========================================
// マウスを利用したカメラの制限
// Author : 冨所知生
//=========================================
bool CCamera_Game::Limit_Used_Mouse()
{
	CMouse *pMouse = CApplication::GetMouse();
	D3DXVECTOR3 axis = {};

	// クリックの情報を保管
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
void CCamera_Game::CameraWork(D3DXQUATERNION que)
{
	if (m_quaternion == D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f))
	{
		return;
	}

	m_quaternion = GetQuaternion();

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
void CCamera_Game::Up()
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