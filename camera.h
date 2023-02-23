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

	enum EVENT
	{
		EVENT_NORMAL,
		EVENT_DEATH,
		EVENT_FLY,		// イベント中のカメラ設定
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CCamera();
	~CCamera();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	virtual HRESULT Init(D3DXVECTOR3 pos);							// 初期化
	virtual void Uninit(void);										// 終了
	virtual void Update(void);										// 更新
	/*virtual*/ void Set();												// 設定
	void Set2();												// 設定
	void SetViewType(VIEW_TYPE type) { m_viewType = type; }			// タイプの設定
	void SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	// ビューポートの大きさ設定
	void AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	// ビューポートの拡縮
	void SetEvent(EVENT event) { m_event = event; }

	// オブジェクトのモードの設定
	void SetObjMode(CObject::Object_mode mode) { m_Objectmode = mode; }
	// 回転の設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	// カメラを揺らす
	void ShakeCamera(int ShakeFrame, float Magnitude);
	
	//--------------------------------------------------------------------
	// ゲッタ―
	//--------------------------------------------------------------------
	const D3DXVECTOR3 GetPosR() { return m_posR; }					// 注視点の取得
	const D3DXVECTOR3 GetPosV() { return m_posV; }					// 視点の取得
	D3DXMATRIX GetView() { return m_mtxView; }						// カメラの情報の取得
	D3DXMATRIX GetProjection() { return m_mtxProj; }				// カメラの情報の取得
	D3DVIEWPORT9 GetViewport() { return m_viewport; }				//ビューポートの取得
	CObject::Object_mode GetObjType() { return m_Objectmode; }
	const D3DXQUATERNION GetQuaternion() { return m_quaternion; }	// 視点角度の取得
	D3DXVECTOR3 GetRot() { return m_rot; }
	float GetDistance() { return m_fDistance; }

protected:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void RPosRotate();			// 回転
	void VPosRotate();			// 回転

protected:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	D3DXQUATERNION		m_quaternion;		// クオータニオン
	D3DXQUATERNION		m_Destquaternion;	// クオータニオンのデスト
	D3DXMATRIX			m_mtxProj;			// プロジェクションマトリックス
	D3DXMATRIX			m_mtxView;			// ビューマトリックス
	D3DXVECTOR3			m_posV;				// 視点
	D3DXVECTOR3			m_posR;				// 注視点
	D3DXVECTOR3			m_rotMove;			// 移動方向
	D3DXVECTOR3			m_Dest;				// マウスのDest
	D3DXVECTOR3			m_rot;				// 回転の設定
	VIEW_TYPE			m_viewType;			// 投影の種別
	D3DVIEWPORT9		m_viewport;			// ビューポート
	EVENT				m_event;			// イベント管理
	CObject::Object_mode m_Objectmode;		// オブジェクトのモード

	float				m_fDistance;		// 視点から注視点までの距離

	bool				m_bWork;			// カメラワークが終了したかしていないか

	/* 揺れ */
	int					m_nCntFrame;		// 揺れるフレームカウント
	int					m_Magnitude;		// 揺れ

	int					nRotateType = -1;
};

#endif
