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
#include <DirectXMath.h>

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
#include "camera_player.h"
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
	m_posV = D3DXVECTOR3(0.0f, 5000.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_viewType = TYPE_PARALLEL;
	SetViewType(m_viewType);

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

		m_posV = D3DXVECTOR3(PlayerPos.x, GetPosV().y, PlayerPos.z);
		m_posR = D3DXVECTOR3(PlayerPos.x, GetPosR().y, PlayerPos.z);
	}

	//	CMouse *pMouse = CApplication::GetMouse();

	// 回転を行う。
	Rotate();

	//// デバッグ用
	//CDebugProc::Print("=========== camera_radar ===========\n");
	//CDebugProc::Print("カメラの座標 : (%.1f,%.1f,%.1f) \n", m_posV.x, m_posV.y, m_posV.z);
	//CDebugProc::Print("カメラの座標R : (%.1f,%.1f,%.1f) \n", m_posR.x, m_posR.y, m_posR.z);
	//CDebugProc::Print("カメラの回転 : (%.2f,%.2f,%.2f) \n", GetRot().x, GetRot().y, GetRot().z);
	//CDebugProc::Print("====================================\n");
}

//=============================================================================
// 回転の計算
// Author : 唐﨑結斗
// Author : YudaKaito
// 概要 : 
//=============================================================================
void CCameraRadar::Rotate()
{
	// X軸の回転
	CPlayer3D *pPlayer = CPlayerManager::GetPlayer();

	// プレイヤーのクォータニオンの取得
	D3DXQUATERNION qua = CApplication::GetCamera()->GetQuaternion();
	D3DXQuaternionNormalize(&qua, &qua);

	D3DXMATRIX WorldMatrix;

	// プレイヤーの進行方向ベクトル(0,0,1)を現在のクォータニオンをベクトルに変換して２つのベクトルの角度を求める
	D3DXVECTOR3 Vec1 = D3DXVECTOR3(0, 0, 1);
	// クォータニオンを回転行列に変換
	D3DXMatrixRotationQuaternion(&WorldMatrix, &qua);

	D3DXVECTOR3 Vec2;
	D3DXVec3TransformCoord(&Vec2, &Vec1, &WorldMatrix);

	//CDebugProc::Print("Vec2 : (%.1f,%.1f,%.1f) \n", Vec2.x, Vec2.y, Vec2.z);
	//CDebugProc::Print("%.1f \n", atan2f(Vec2.x, Vec2.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, atan2f(Vec2.x, Vec2.z)));


	//// 内積計算
	//float dot = Vec1.x * Vec2.x + Vec1.z * Vec2.z;
	//
	//dot = acos(dot);

	//// 外積計算
	//float Cross = Vec2Cross(&Vec1, &Vec2);

	//if (Cross <= 0)
	//{
	//	dot *= -1;
	//}

	//SetRot(D3DXVECTOR3(GetRot().x, GetRot().y, dot));
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
	m_Dest = D3DXVECTOR3(pMouse->GetMouseCursorMove().y, pMouse->GetMouseCursorMove().x, pMouse->GetMouseCursorMove().z);

	// クリックの情報を保管
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	if (hasRightClick)
	{
		Rotate();
		//VPosRotate();
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
		//VPosRotate();
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

//=========================================
// クォータニオンをラジアンに変換する処理
//=========================================
double CCameraRadar::quaternion_to_radian(double qw, double qx, double qy, double qz)
{
	// Calculate quaternion magnitude
	double q_norm = std::sqrt(qw*qw + qx*qx + qy*qy + qz*qz);

	// Calculate rotation angle in radians
	double theta = 2.0*std::acos(qw / q_norm);

	// Calculate rotation axis
	double axis_x = qx / q_norm;
	double axis_y = qy / q_norm;
	double axis_z = qz / q_norm;

	// Calculate rotation matrix
	double R[3][3] = { { std::cos(theta / 2.0), -axis_x*std::sin(theta / 2.0), -axis_y*std::sin(theta / 2.0) },
	{ axis_x*std::sin(theta / 2.0), std::cos(theta / 2.0), -axis_z*std::sin(theta / 2.0) },
	{ axis_y*std::sin(theta / 2.0), axis_z*std::sin(theta / 2.0), std::cos(theta / 2.0) } };

	// Calculate radians
	double radian = std::acos((R[0][0] + R[1][1] + R[2][2] - 1.0) / 2.0);

	return radian;
}

//=========================================
// ラジアンをクォータニオンに変換
//=========================================
D3DXQUATERNION CCameraRadar::radian_to_quaternion(double radian)
{
	D3DXQUATERNION qua;

	// Calculate quaternion elements
	qua.w = std::cos(radian * 0.5f);
	qua.x = 0.0;
	qua.y = std::sin(radian * 0.5f);
	qua.z = std::sin(radian * 0.5f);

	// Normalize quaternion
	double q_norm = std::sqrt(qua.w*qua.w + qua.x * qua.x + qua.y * qua.y + qua.z * qua.z);
	qua.w /= q_norm;
	qua.x /= q_norm;
	qua.y /= q_norm;
	qua.z /= q_norm;

	return qua;
}

//=========================================
// クォータニオンをラジアンに変換する処理
//=========================================
D3DXVECTOR3 CCameraRadar::QuaternionToPolar(double w, double x, double y, double z)
{
	D3DXVECTOR3 radian;

	float r = std::sqrt(w*w + x*x + y*y + z*z); // 半径を計算
	radian.x = std::acos(w / r); // 第1の角度を計算
	radian.y = std::atan2(y, x); // 第2の角度を計算
	radian.z = std::atan2(z, std::sqrt(w*w + x*x + y*y)); // 第3の角度を計算

	return radian;
}

//=========================================
// オイラー角をクォータニオンに変換する
//=========================================
D3DXQUATERNION CCameraRadar::fromEuler(float yaw, float pitch, float roll)
{
	// 各軸の回転をクォータニオンに変換する
	D3DXQUATERNION qYaw;
	D3DXQuaternionRotationYawPitchRoll(&qYaw, yaw, 0, 0);

	D3DXQUATERNION qPitch;
	D3DXQuaternionRotationYawPitchRoll(&qPitch, 0, pitch, 0);

	D3DXQUATERNION qRoll;
	D3DXQuaternionRotationYawPitchRoll(&qRoll, 0, 0, roll);

	// 各クォータニオンを順番に掛け合わせる
	D3DXQUATERNION qRotation;
	D3DXQuaternionMultiply(&qRotation, &qYaw, &qPitch);
	D3DXQuaternionMultiply(&qRotation, &qRotation, &qRoll);

	// 結果を正規化する
	D3DXQuaternionNormalize(&qRotation, &qRotation);

	return qRotation;
}

//=========================================
// クォータニオンを回転行列に変換する
//=========================================
D3DXVECTOR3 CCameraRadar::DirectionVectorFromQuaternion(const D3DXQUATERNION& qRotation)
{
	// クォータニオンを回転行列に変換する
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationQuaternion(&rotationMatrix, &qRotation);

	// 方向ベクトルを取り出す
	D3DXVECTOR3 direction(0, 0, 1);
	D3DXVec3TransformNormal(&direction, &direction, &rotationMatrix);

	return direction;
}