//=========================================
//
//	bullet3D.h
//	Author:冨所知生
//
//=========================================
#ifndef _BULLET3D_H_		//このマクロ定義がされてなかったら
#define _BULLET3D_H_		//２重インクルード防止のマクロ定義

#include "object3D.h"
#include "object.h"
#include "model.h"
#include "missile_alert.h"

class CModel3D;
class CObject;
class CMissileAlertManager;
class CRadar;
class CBullet3D : public CObject
{
public:
	enum MISSILE_STATE
	{
		IDOL_STATE = 0,		// 通常
		LOCK_ON_STATE,		// ロックオン
		MAX_STATE,			// 計算方法の最大数
	};

	CBullet3D();							//コンストラクタ
	virtual ~CBullet3D() override;			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 終了処理
	void Uninit(void) override;
	// 弾の制御
	void BulletMove(void);
	// ロックオン処理
	D3DXVECTOR3 LockOn();
	// 追従処理
	void Follow();

	// 探索カウントの目標を設定
	void SetSearchValue(int val) { m_SearchValue = val; }
	// 角度の設定
	void SetRot(D3DXVECTOR3 rot) { m_ShooterRot = rot; }
	void SetQue(D3DXQUATERNION que) { m_quaternion = que; }
	void SetAngleLimit(float Limit) { m_fAngleLimit = Limit; }
	// ミサイルの速度の取得
	float GetSpeed() { return m_MissileSpeed; }

	// クォータニオンをオイラー角に変換する
	D3DXVECTOR3 QuaternionChange();

	// アラートを有効にするかどうかを取得
	bool GetAlert() { return m_bAlert; }

	// クリエイト処理
	// 引数 : ずらす座標の大きさ、クォータニオン、目標オブジェクト情報、射手オブジェクト情報、誘導のタイミングをカウントする目標値
	static CBullet3D* Create(const D3DXVECTOR3 &pos, const D3DXQUATERNION &quaternion, CObject *object, CObject *Shooter,int val,float Limit);

private:
	CMissileAlertManager*	m_pAlert;					// ミサイルアラート
	CModel3D*				m_pModel;					// モデルの最大数
	CObject*				m_TargetObj;				// ターゲットオブジェクト
	CObject*				m_pShooter;					// シューター
	CRadar*					m_pRadar;					// レーダー
	D3DXQUATERNION			m_quaternion;				// クォータニオン
	D3DXVECTOR3				m_Rot;						// オイラー角
	D3DXVECTOR3				m_pos;						// 座標
	D3DXVECTOR3				m_TargetPos;				// ターゲットの座標
	D3DXVECTOR3				m_posOld;					// 古い座標
	D3DXVECTOR3				m_posTarget;				// 目標の座標
	D3DXVECTOR3				m_ShooterRot;				// 射手の角度
	D3DXVECTOR3				m_FllowRot;					// 追尾している方向
	D3DXVECTOR3				m_move;						// 進行方向を保存する
	D3DXMATRIX				m_WorldMtx;					// ワールドマトリックス
	MISSILE_STATE			m_state;					// ステート
	float					m_length;					// 弾と目標値の距離
	float					m_MissileSpeed;				// ミサイルの速度
	float					m_fAngleLimit;				// 視野の限界値
	int						m_FrontMoveCnt;				// 真っすぐ飛ぶカウント
	int						m_SearchCnt;				// 探索カウント
	int						m_SearchValue;				// 探索カウントの目標値
	int						m_MissileCnt;				// ミサイルカウント
	int						m_LocusCnt;					// 爆発を表示するカウント

	bool					m_bAlert;					// アラート
};

#endif