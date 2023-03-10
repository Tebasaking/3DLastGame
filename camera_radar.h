//**************************************************************************************************
//
// レーダー状のカメラ処理(camera.h)
// Author : 唐?ｱ結斗
// Author : 湯田海都
// Author : 冨所知生
// 概要 : カメラの設定処理
//
//**************************************************************************************************
#ifndef _CAMERA_RADAR_H_			// このマクロ定義がされてなかったら
#define _CAMERA_RADAR_H_			// 二重インクルード防止のマクロ定義

//***************************************************************************
// インクルード
//***************************************************************************
#include "main.h"
#include "camera.h"

//=============================================================================
// カメラクラス
// Author : 唐?ｱ結斗
// Author : 湯田海都
// Author : 冨所知生
// 概要 : カメラ設定を行うクラス
//=============================================================================
class CCameraRadar : public CCamera
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
	// Author : 唐?ｱ結斗
	// 概要 : 投影方法の種別
	//=============================================================================
	enum CAMERA_TYPE
	{
		TYPE_FREE,					// 自由にカメラを動かせる状態
		TYPE_SHOULDER,				// 肩越し視点状態
		MAX_CAMERA_TYPE,			// カメラタイプの最大数
	};

	enum EVENT
	{
		EVENT_NORMAL,
		EVENT_FLY,					// イベント中のカメラ設定
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CCameraRadar();
	~CCameraRadar();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	virtual HRESULT Init(D3DXVECTOR3 pos);								// 初期化
	virtual void Uninit();												// 終了
	virtual void Update();												// 更新

	// floatを利用したカメラの制限(結構無理やり)
	bool Limit_Used_Mouse();
	// クォータニオンを極座標に変換する処理
	D3DXVECTOR3 QuaternionToPolar(double w, double x, double y, double z);
	// ラジアンをクォータニオンに変換
	D3DXQUATERNION radian_to_quaternion(double radian);
	// オイラー角をクォータニオンに変換
	D3DXQUATERNION fromEuler(float yaw, float pitch, float roll);
	// クォータニオンをラジアンに変換
	double quaternion_to_radian(double qw, double qx, double qy, double qz);
	// オブジェクトのモードの設定
	void SetObjMode(CObject::Object_mode mode) { m_Objectmode = mode; }
	// クォータニオンを回転行列に変換
	D3DXVECTOR3 DirectionVectorFromQuaternion(const D3DXQUATERNION& qRotation);
	
private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Rotate();				// 回転
	void MouseMove();			// マウス移動を回転に代入
	void JoyPadMove();			// ジョイパッド移動を回転に代入

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	D3DXVECTOR3			m_vecU;				// 上方向ベクトル
	D3DXVECTOR3			m_VecGet;			// マウスのベクトル
	D3DXVECTOR3			m_Dest;				// マウスのDest
	D3DXVECTOR3			m_axisVec;			// 回転方向のベクトル
	CAMERA_TYPE			m_mode;				// カメラのモード

	float				m_fRotMove;			// 移動方向
	float				m_MouseMove;		// 中心から移動したマウスの量
	float				CAMERA_MOVE_SPEED = 0.5f;
	float				MOVE_SPEED = 5.0f;
	float				m_Gravity;			// 重力

	bool				m_bWork;			// カメラワークが終了したかしていないか
	bool				m_bUp;				// 上昇しているかしていないか

	int					m_nCntFly;			// 飛行
	int					m_nCntCameraWork;	// カメラワークの終了までの時間
	int					m_nCntMoveSound;	// 移動中の音

	int					nRotateType = -1;
};

#endif
