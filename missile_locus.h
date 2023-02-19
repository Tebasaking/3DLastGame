//=========================================
//
//	explosion.h
//	Author:冨所知生
//
//=========================================
#ifndef _MISSILE_LOCUS_H_		//このマクロ定義がされてなかったら
#define _MISSILE_LOCUS_H_		//２重インクルード防止のマクロ定義

#include "billboard.h"

class CMissile_Locus :public CBillboard
{
public:
	CMissile_Locus();			//コンストラクタ
	virtual ~CMissile_Locus() override;			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CMissile_Locus* Create(const D3DXVECTOR3 &pos, CObject *object);

private:
	D3DXVECTOR3 m_move;				// 移動量
	D3DXVECTOR3 m_pos;				// 座標
	D3DXCOLOR	m_col;				// 色
	int m_size;						// サイズ
	D3DXQUATERNION m_quaternion;	// クォータニオン

	static const int		ANIM_SPEED;
	static const int		ANIM_MAX;
	int						nAnimCount;
	int						nAnimNum;
	int						nAnimNumY;
	float					m_RandAlfa;		// α値をX(ランダム)減らす
};

#endif