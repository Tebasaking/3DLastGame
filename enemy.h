//=========================================
//
//	player3D.h
//	Author:冨所知生
//
//=========================================
#ifndef _ENEMY3D_H_		//このマクロ定義がされてなかったら
#define _ENEMY3D_H_		//２重インクルード防止のマクロ定義

#define MAX_PARTS (1)

#include "main.h"
#include "object.h"

#include "motion_model3D.h"

class CModel3D;
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
	//クリエイト処理
	static CEnemy* Create(const D3DXVECTOR3 &pos);

	//番号設定処理
	void SetNumber(int Number) { m_Number = Number; }
	//番号取得処理
	int GetNumber() { return m_Number; }

	CModel3D *GetModel() { return m_apModel[0]; }

private:
	float m_scale;										// 大きさ

	CModel3D*				m_apModel[MAX_PARTS];		// モデルの最大数
	D3DXVECTOR3				m_pos;						// 座標
	D3DXVECTOR3				m_rot;						// 回転
	EnemyState				m_state;					// 状態
};

#endif