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

#include "camera_radar.h"
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
const float CCameraRadar::CAMERA_NEAR = 30.0f;		// ニア
const float CCameraRadar::CAMERA_FUR = 100000000.0f;	// ファー

//=============================================================================
// コンストラクタ
// Author	: 唐﨑結斗
// 概要		: インスタンス生成時に行う処理
//=============================================================================
CCameraRadar::CCameraRadar() :
	m_viewType(TYPE_CLAIRVOYANCE),							// 投影方法の種別
	m_nCntFly(0),
	m_fRotMove(0.0f)										// 移動方向
{
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCameraRadar::~CCameraRadar()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 視点と注視点の間の距離を算出する
//=============================================================================
HRESULT CCameraRadar::Init(D3DXVECTOR3 pos)
{
	m_posV = D3DXVECTOR3(0.0f, 5000.0f, -150.0f);
	m_posR = D3DXVECTOR3(0.0f, 5000.0f, 0.0f);
	m_quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_viewType = TYPE_PARALLEL;

	// 視点と注視点の距離
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	m_mode = TYPE_FREE;

	m_nCntMoveSound = 10000;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : 終了
//=============================================================================
void CCameraRadar::Uninit(void)
{
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新
//=============================================================================
void CCameraRadar::Update(void)
{
	// プレイヤーの位置を取得して、X と Y の座標を合わせる。
	CPlayer3D *pPlayer = CPlayerManager::GetPlayer();
	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		m_posV = D3DXVECTOR3(PlayerPos.x, m_posV.y, PlayerPos.z);
		m_posR = D3DXVECTOR3(PlayerPos.x, m_posR.y, PlayerPos.z);
	}

	CMouse *pMouse = CApplication::GetMouse();

	// 回転のベクトルを設定。
	m_Dest = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	// 右クリック時、回転を行う。
	if (pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT))
	{
		m_quaternion = D3DXQUATERNION(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);

		Rotate();
		RPosRotate();
	}

	D3DXVECTOR3 Result = m_Dest - m_rotMove;
	m_rotMove += Result * 0.25f;

	// 慣性を持たせた回転
	m_quaternion += (m_Destquaternion - m_quaternion) * 0.1f;
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	// デバッグ用
	CDebugProc::Print("=========== camera_radar ===========\n");
	CDebugProc::Print("カメラの座標 : (%.1f,%.1f,%.1f) \n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("カメラの回転 : (%.2f,%.2f,%.2f,%.2f) \n", m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	CDebugProc::Print("====================================\n");
}

//=============================================================================
// 回転の計算
// Author : 唐﨑結斗
// Author : YudaKaito
// 概要 : 
//=============================================================================
void CCameraRadar::Rotate()
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

	// マウスのベクトル軸取得
	m_axisVec.y = rollDir.x;
	m_axisVec.x = -rollDir.y;

	D3DXVECTOR3 inverseVec = m_axisVec;

	D3DXVec3Normalize(&inverseVec, &inverseVec);

	m_VecGet = inverseVec;

	// X軸の回転
	CPlayer3D *pPlayer = CPlayerManager::GetPlayer();

	if (pPlayer != nullptr)
	{
		D3DXQUATERNION PlayerQua = pPlayer->GetQuaternion();

		m_quaternion = D3DXQUATERNION(m_quaternion.x, PlayerQua.x, m_quaternion.z, m_quaternion.w);
	}

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&m_Destquaternion, &m_Destquaternion);
}

//=========================================
// マウスの移動
// Author : 唐﨑結斗
// Author : YudaKaito
//=========================================
void CCameraRadar::MouseMove(void)
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
// ジョイパッドの移動
// Author : 冨所知生
// 概要 : ジョイパッド使用時のカメラの旋回
//=========================================
void CCameraRadar::JoyPadMove(void)
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

//=========================================
// マウスを利用したカメラの制限
// Author : 冨所知生
//=========================================
bool CCameraRadar::Limit_Used_Mouse()
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
