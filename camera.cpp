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
#include "calculation.h"
#include "debug_proc.h"
#include "utility.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
const float CCamera::CAMERA_NEAR = 30.0f;		// ニア
const float CCamera::CAMERA_FUR = 50000.0f;	// ファー

//=============================================================================
// コンストラクタ
// Author	: 唐﨑結斗
// 概要		: インスタンス生成時に行う処理
//=============================================================================
CCamera::CCamera() :
	m_posV(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// 視点
	m_posR(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// 注視点
	m_rotMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),			 	// 移動方向
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_quaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f)),	// クオータニオン
	m_viewType(TYPE_CLAIRVOYANCE),							// 投影方法の種別
	m_event(EVENT_NORMAL),
	m_Dest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_fDistance(0.0f),										// 視点から注視点までの距離
	m_Destquaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f))
{
	m_mtxProj = {};		// プロジェクションマトリックス
	m_mtxView = {};		// ビューマトリックス
	m_Objectmode = CObject::Object_mode::NONE_MODE;
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
HRESULT CCamera::Init(D3DXVECTOR3 pos)
{
	m_posV = pos;

	// 視点と注視点の距離
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : 終了
//=============================================================================
void CCamera::Uninit(void)
{
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
		D3DXVECTOR3 Result = m_Dest - m_rotMove;
		m_rotMove += Result * 0.25f;
	}

	m_quaternion += (m_Destquaternion - m_quaternion) * 0.1f;
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	/*CDebugProc::Print("カメラの座標 : (%f,%f,%f) \n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("カメラの回転 : (%f,%f,%f,%f) \n", m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);*/
}

//=============================================================================
// カメラの設定
// Author : 冨所知生
// 概要 : ビューマトリックスの設定
//=============================================================================
void CCamera::Set()
{
	// 揺れカウンターを減らす
	m_nCntFrame--;

	if (m_nCntFrame >= 0)
	{
		D3DXVECTOR3 adjustPos = {};

		adjustPos.x = FloatRandom(m_Magnitude, -m_Magnitude);
		adjustPos.y = FloatRandom(m_Magnitude, -m_Magnitude);
		adjustPos.z = FloatRandom(m_Magnitude, -m_Magnitude);

		m_posV += adjustPos;
		m_posR += adjustPos;
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);			// 行列初期化関数

	D3DXMATRIX mtxRot, mtxTrans;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_posV.x, m_posV.y, m_posV.z);	// 行列移動関数
	D3DXMatrixInverse(&mtxTrans, NULL, &mtxTrans);					// 逆行列に計算
	D3DXMatrixMultiply(&m_mtxView, &m_mtxView, &mtxTrans);			// 行列掛け算関数

	// 向きの反映
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);	// クオータニオンによる行列回転
	D3DXMatrixInverse(&mtxRot, NULL, &mtxRot);				// 逆行列に計算
	D3DXMatrixMultiply(&m_mtxView, &m_mtxView, &mtxRot);	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

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
			(float)SCREEN_WIDTH * 5,						// 幅
			(float)SCREEN_WIDTH * 5,						// 高さ
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
// カメラの設定
// Author : 冨所知生
// 概要 : ビューマトリックスの設定
//=============================================================================
void CCamera::Set2()
{
	//// 揺れカウンターを減らす
	//m_nCntFrame--;

	//if (m_nCntFrame >= 0)
	//{
	//	D3DXVECTOR3 adjustPos = {};

	//	adjustPos.x = FloatRandom(m_Magnitude, -m_Magnitude);
	//	adjustPos.y = FloatRandom(m_Magnitude, -m_Magnitude);
	//	adjustPos.z = FloatRandom(m_Magnitude, -m_Magnitude);

	//	m_posV += adjustPos;
	//	m_posR += adjustPos;
	//}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);			// 行列初期化関数

	D3DXVECTOR3 vecUp(0.0f, 0.0f, 1.0f);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &vecUp);

	D3DXMATRIX mtxRot, mtxTrans;

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);			// 行列回転関数
	D3DXMatrixMultiply(&m_mtxView, &m_mtxView, &mtxRot);						// 行列掛け算関数 

	//	// 位置を反映
	//	D3DXMatrixTranslation(&mtxTrans, m_posV.x, m_posV.y, m_posV.z);	// 行列移動関数
	////	D3DXMatrixInverse(&mtxTrans, NULL, &mtxTrans);					// 逆行列に計算
	//	D3DXMatrixMultiply(&m_mtxView, &m_mtxView, &mtxTrans);			// 行列掛け算関数

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

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
			(float)SCREEN_WIDTH * 10,						// 幅
			(float)SCREEN_WIDTH * 10,						// 高さ
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
// カメラを揺らす処理
// Author : 冨所知生
//=========================================
void CCamera::ShakeCamera(int ShakeFrame, float Magnitude)
{
	m_nCntFrame = ShakeFrame;
	m_Magnitude = Magnitude;
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
