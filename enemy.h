//=========================================
//
//	player3D.h
//	Author:冨所知生
//
//=========================================
#ifndef _ENEMY3D_H_		//このマクロ定義がされてなかったら
#define _ENEMY3D_H_		//２重インクルード防止のマクロ定義

#define MAX_PARTS (1)

#include "object.h"

#include "motion_model3D.h"

class CModel3D;
class CTarget;
class CRadar;
class CEnemy :public CMotionModel3D
{
public:
	//オブジェクトの種類
	enum EnemyState
	{
		ENEMY_IDOL,			// エネミーの待機状態
		ENEMY_WARNNING,		// エネミーがプレイヤーを発見した状態
		//見失った状態とかも作りたいけど余裕があれば。
	};

	// エネミーの種類
	enum EnemyType
	{
		ENEMY_FLY,			// 空にいる敵
		ENEMY_GROUND,		// 陸にいる敵
		ENEMY_MAX
	};

	CEnemy();			//コンストラクタ
	virtual ~CEnemy() override;			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//死亡処理
	void Death(void);
	//エネミー同士の当たり判定
	void EnemyCollision(void);
	//クリエイト処理
	static CEnemy* Create(const D3DXVECTOR3 &pos);

	//番号設定処理
	void SetNumber(int Number) { m_Number = Number; }
	//番号取得処理
	int GetNumber() { return m_Number; }

	// ステートの設定
	void SetState(EnemyState state) { m_state = state; }
	// タイプの設定
	void SetType(EnemyType type) { m_type = type; }
	// 弾の発射
	void Bullet(CObject *obj);

	CModel3D *GetModel() { return m_apModel[0]; }

private:
	CTarget*				m_Target;					// 自身につけるターゲット
	CRadar*					m_Radar;					// レーダー
	float					m_scale;					// 大きさ
	bool					m_bColision;				// 当たっているかどうか

	int						m_AttackCount;				// 攻撃間隔

	EnemyType				m_type;						// タイプの設定
	CModel3D*				m_apModel[MAX_PARTS];		// モデルの最大数
	EnemyState				m_state;					// 状態
	D3DXMATRIX				m_WorldMtx;					// ワールドマトリックス
	D3DXVECTOR3				m_size;						// サイズ設定
	D3DXVECTOR3				m_posOld;
	D3DXVECTOR3				m_DestMove;					// デスト
};

#endif