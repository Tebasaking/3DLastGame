//**************************************************************************************************
//
// カメラ処理(camera.h)
// Auther：唐﨑結斗
// 概要 : カメラの設定処理
//
//**************************************************************************************************
#ifndef _CAMERA_H_			// このマクロ定義がされてなかったら
#define _CAMERA_H_			// 二重インクルード防止のマクロ定義

//***************************************************************************
// インクルード
//***************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMove;

//=============================================================================
// カメラクラス
// Author : 唐﨑結斗
// 概要 : カメラ設定を行うクラス
//=============================================================================
class CObject;
class CCamera
{
private:
	//--------------------------------------------------------------------
	// 定数定義
	//--------------------------------------------------------------------
	static const float CAMERA_NEAR;			// ニアの最大値
	static const float CAMERA_FUR;			// ファーの最大値

public:
	//=============================================================================
	// 投影方法のタイプ列挙型
	// Author : 唐﨑結斗
	// 概要 : 投影方法の種別
	//=============================================================================
	enum VIEW_TYPE
	{
		TYPE_CLAIRVOYANCE = 0,		// 透視投影
		TYPE_PARALLEL,				// 平行投影
		MAX_VIEW_TYPE,				// タイプの最大数
	};

	enum CAMERA_TYPE
	{
		TYPE_FREE,					// 自由にカメラを動かせる状態
		TYPE_SHOULDER,				// 肩越し視点状態
		MAX_CAMERA_TYPE,			// カメラタイプの最大数
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CCamera();
	~CCamera();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init();						// 初期化
	void Uninit(void);												// 終了
	void Update(void);												// 更新
	void UpdateNormal(void);										// 通常処理
	void UpdateRadar(void);											// レーダーの処理
	void Set();														// 設定
	void SetViewType(VIEW_TYPE type) { m_viewType = type; }			// タイプの設定
	void SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	//ビューポートの大きさ設定
	void AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	//ビューポートの拡縮
	// オブジェクトのモードの設定
	void SetObjMode(CObject::Object_mode mode) { m_Objectmode = mode; }
	
	//--------------------------------------------------------------------
	// ゲッタ―
	//--------------------------------------------------------------------
	const D3DXVECTOR3 GetRot() { return m_rot; }					// 角度の取得
	const D3DXVECTOR3 GetPosR() { return m_posR; }					// 注視点の取得
	const D3DXVECTOR3 GetPosV() { return m_posV; }					// 視点の取得
	const D3DXVECTOR3 GetVec() { return m_VecGet;}					// マウスベクトルの取得
	D3DXMATRIX GetView() { return m_mtxView; }						// カメラの情報の取得
	D3DXMATRIX GetProjection() { return m_mtxProj; }				// カメラの情報の取得
	CAMERA_TYPE GetMode() { return m_mode; }						// カメラのモード情報の取得
	D3DVIEWPORT9 GetViewport() { return m_viewport; }				//ビューポートの取得
	CObject::Object_mode GetObjType() { return m_Objectmode; }
	const D3DXQUATERNION GetQuaternion() { return m_quaternion; }	// 視点角度の取得

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Rotate();					// 回転
	void RPosRotate();				// 回転
	void VPosRotate();				// 回転
	void FreeMove(void);			// 通常移動
	void ShoulderMove(void);		// 肩越し時の移動 
	void MouseMove(void);			// マウス移動を回転に代入
	void JoyPadMove(void);			// ジョイパッド移動を回転に代入

//--------------------------------------------------------------------
// メンバ変数
//--------------------------------------------------------------------
	CMove				*m_pRoll;			// 移動クラスのインスタンス(角度)
	D3DXQUATERNION		m_quaternion;		// クオータニオン
	D3DXQUATERNION		m_Destquaternion;	// クオータニオンのデスト
	D3DXMATRIX			m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX			m_mtxProj;			// プロジェクションマトリックス
	D3DXMATRIX			m_mtxView;			// ビューマトリックス
	D3DXVECTOR3			m_posV;				// 視点
	D3DXVECTOR3			m_posR;				// 注視点
	D3DXVECTOR3			m_vecU;				// 上方向ベクトル
	D3DXVECTOR3			m_rot;				// 向き
	D3DXVECTOR3			m_rotMove;			// 移動方向
	D3DXVECTOR3			m_VecGet;			// マウスのベクトル
	VIEW_TYPE			m_viewType;			// 投影の種別
	CAMERA_TYPE			m_mode;				// カメラのモード
	D3DVIEWPORT9		m_viewport;			// ビューポート
	CObject::Object_mode m_Objectmode;		// オブジェクトのモード
	float				m_fDistance;		// 視点から注視点までの距離
	float				m_fRotMove;			// 移動方向

	float				CAMERA_MOVE_SPEED = 0.5f;
	float				MOVE_SPEED = 5.0f;

	D3DXVECTOR3			m_axisVec;			// 回転方向のベクトル
	int nRotateType = -1;
};

#endif
