//=========================================
//
//	player3D.h
//	Author:冨所知生
//
//=========================================
#ifndef _PLAYER_H_		//このマクロ定義がされてなかったら
#define _PLAYER_H_		//２重インクルード防止のマクロ定義
#define MAX_PARTS (1)

#include "object3D.h"
#include "model.h"
#include "motion_model3D.h"

class CModel3D;
class CRadar;

class CPlayer3D : public CMotionModel3D
{
public:
	enum PLAYER_MODE
	{
		MODE_FLY = 0,
		MODE_ROBOT,
		MODE_MAX
	};

	enum PLAYER_STATE
	{
		NORMAL_STATE = 0,
		DEATH_STATE,
		MAX_STATE
	};
	
	explicit CPlayer3D(int nPriority = CObject::LAYER_ONE);	//コンストラクタ
	virtual ~CPlayer3D() override;			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 終了処理
	void Uninit(void) override;
	// クリエイト処理
	static CPlayer3D* Create(const D3DXVECTOR3 &pos);
	// プレイヤーのモードの設定
	void SetMode(PLAYER_MODE mode) { m_mode = mode; }
	// プレイヤーの無敵判定
	void SetInvincible() { m_bInvincible = true; }
	
	// 無敵カウント
	void InvincibleCheck();
	// ロックオン処理
	void LockOn(void);
	// 回転処理
	void Rotate(void);
	// 弾の発射処理
	void Bullet(D3DXVECTOR3 pos);
	// 地面との当たり判定
	void GroundCollison();
	// 死亡の処理
	void Death();

	// 更新処理(飛行)
	void UpdateFly(void);
	// 更新処理(人型)
	void UpdateRob(void);

	//=========================================
	// ゲッター
	//=========================================
	bool GetCollision() { return m_bCollision; }		// 地面に当たっているか当たっていないか

	//=========================================
	// 飛行状態でしか使わないもの
	//=========================================
	// 姿勢制御処理
	void Attitude(void);

	//=========================================
	// 人型状態でしか使わないもの
	//=========================================
	// 近接攻撃処理
	void Slash(void);
	// ジャンプ処理
	void Jump(void);
	// 移動処理
	void Move(void);
	// 弾の加算処理
	void AddBullet() { m_BulletHave += 2; }

	// BulletDelayの取得処理
	int GetBulletDelay() { return m_BulletDelay;}
	int GetBulletHave() { return m_BulletHave; }
	bool GetInvincible() { return m_bInvincible; }

	const D3DXQUATERNION& GetQuaternion() const { return m_quaternion; }

private:
	float	m_scale;									// 大きさ
	float	m_MoveAmount;								// 移動量
	bool	m_bCollision;								// 当たり判定
	bool	m_bJump;									// ジャンプ中かジャンプ中じゃないか
	bool	m_bMove;									// 移動しているかしていないか
	bool	m_bMove_Motion_Check;						// 移動モーションが使われているかいないか
	bool	m_bMotion;									// モーションを使っているか
	bool	m_bInvincible;								// プレイヤーの無敵判定

	D3DXVECTOR3 m_size;									// 拡大率

	CModel3D*				m_apModel[MAX_PARTS];		// モデルの最大数
	CModel3D*				m_pRobot;					// ロボットのモデル読み込み
	D3DXMATRIX				m_WorldMtx;					// ワールドマトリックス
	D3DXVECTOR3				m_MouseMove;				// マウスの移動処理
	D3DXVECTOR3				m_changeRot;				// 変換した角度の値
	PLAYER_MODE				m_mode;						// モード
	PLAYER_STATE			m_state;					// プレイヤーの状態
	CRadar*					m_Radar;					// レーダー
	CObject*				m_Nearest_object;			// 最も近いオブジェクト
	int						m_BulletHave;				// バレットを持っている数
	int						m_MotionCnt;				// モーションカウント
	int						m_nNumHandParts;			// 手のパーツ番号
	int						m_StepCnt;					// 足音のカウント
	int						m_BulletDelay;				// バレットのカウント
	int						m_InvincibleCnt;			// 無敵カウント
	int						m_GameOverCnt;				// ゲームオーバーのカウント
};

#endif