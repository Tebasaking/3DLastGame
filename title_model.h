//=========================================
//
//	title_model.h
//	Author:冨所知生
//
//=========================================
#ifndef _TITLE_MODEL_H_		//このマクロ定義がされてなかったら
#define _TITLE_MODEL_H_		//２重インクルード防止のマクロ定義

#define MAX_PARTS (1)

#include "object.h"

#include "motion_model3D.h"

class CModel3D;
class CTitleModel :public CMotionModel3D
{
public:
	// エネミーの種類
	enum ModelType
	{
		ENEMY_FLY,			// 空にいる敵
		ENEMY_GROUND,		// 陸にいる敵
		ENEMY_MAX
	};

	explicit CTitleModel(int nPriority = CObject::LAYER_FIVE);			//コンストラクタ
	virtual ~CTitleModel() override;			//デストラクタ

										//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CTitleModel* Create(const D3DXVECTOR3 &pos, const ModelType &type);

	// タイプの設定とタイプに応じたモーションの設定
	void SetType(ModelType type);

	CModel3D *GetModel() { return m_apModel[0]; }

private:
	float					m_scale;					// 大きさ
	bool					m_bColision;				// 当たっているかどうか

	int						m_AttackCount;				// 攻撃間隔
	int						m_Wave;						// 自身が所属しているウェーブ数
	int						m_CntWar;					// 闘いを再会するまでの時間

	ModelType				m_type;						// タイプの設定
	CModel3D*				m_apModel[MAX_PARTS];		// モデルの最大数
	D3DXMATRIX				m_WorldMtx;					// ワールドマトリックス
	D3DXVECTOR3				m_size;						// サイズ設定
	D3DXVECTOR3				m_PlayerPos;				// プレイヤーの座標
	D3DXVECTOR3				m_posOld;
	D3DXVECTOR3				m_DestMove;					// デスト
};

#endif