//**************************************************************************************************
//
// 自由カメラ処理(camera.h)
// Auther：唐﨑結斗
// 概要 : カメラの設定処理
//
//**************************************************************************************************
#ifndef _CAMERA_PLAYER_H_			// このマクロ定義がされてなかったら
#define _CAMERA_PLAYER_H_			// 二重インクルード防止のマクロ定義

//***************************************************************************
// インクルード
//***************************************************************************
#include "main.h"
#include "camera.h"

//=============================================================================
// カメラクラス
// Author : 唐﨑結斗
// 概要 : カメラ設定を行うクラス
//=============================================================================
class CCameraPlayer : public CCamera
{
public:
	enum CAMERA_TYPE
	{
		TYPE_FREE,					// 自由にカメラを動かせる状態
		TYPE_SHOULDER,				// 肩越し視点状態
		MAX_CAMERA_TYPE,			// カメラタイプの最大数
	};

public:
	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CCameraPlayer();
	~CCameraPlayer();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(D3DXVECTOR3 pos) override;							// 初期化
	void Uninit(void) override;										// 終了
	void Update(void) override;										// 更新
	void Up(void);													// 上に上昇する
	void SetUp(bool check) { m_bUp = check; }
	void SetDeathGround() { m_bDeathGround = true; }

	// floatを利用したカメラの制限(結構無理やり)
	bool Limit_Used_Mouse();
	// 設定した目標地点に視点を移動させる処理
	void CameraWork(D3DXQUATERNION que);
	
	//--------------------------------------------------------------------
	// ゲッタ―
	//--------------------------------------------------------------------
	const D3DXVECTOR3 GetVec() { return m_VecGet;}					// マウスベクトルの取得
	CAMERA_TYPE GetMode() { return m_mode; }						// カメラのモード情報の取得
	float GetMoveSpeed() { return MOVE_SPEED; }

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Rotate();			// 回転
	void FreeMove();		// 通常移動
	void ShoulderMove();	// 肩越し時の移動 
	void MouseMove();		// マウス移動を回転に代入
	void JoyPadMove();		// ジョイパッド移動を回転に代入
	void FlightEvent();		// フライトイベント
	void Death();			// 死亡イベントの開始

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	D3DXVECTOR3			m_rotMove;			// 移動方向
	D3DXVECTOR3			m_VecGet;			// マウスのベクトル
	D3DXVECTOR3			m_Dest;				// マウスのDest
	D3DXVECTOR3			DefPos;		
	D3DXVECTOR3			m_axisVec;			// 回転方向のベクトル
	CAMERA_TYPE			m_mode;				// カメラのモード

	float				m_fRotMove;			// 移動方向
	float				m_MouseMove;		// 中心から移動したマウスの量
	float				CAMERA_MOVE_SPEED = 0.5f;
	float				MOVE_SPEED = 5.0f;
	float				m_Gravity;			// 重力

	int					m_nCntFly;			// 飛行
	int					m_nCntMoveSound;	// 移動中の音
	int					m_nCntCameraWork;	// カメラワークの終了までの時間

	bool				m_bUp;				// 上昇しているかしていないか
	bool				m_bDeathGround;		// 死亡衝突
};

#endif
