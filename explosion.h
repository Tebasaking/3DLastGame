//=========================================
//
//	explosion.h
//	Author:冨所知生
//
//=========================================
#ifndef _EXPLOSION_H_		//このマクロ定義がされてなかったら
#define _EXPLOSION_H_		//２重インクルード防止のマクロ定義

#include "main.h"
#include "billboard.h"

class CExplosion :public CBillboard
{
public:
	CExplosion();			//コンストラクタ
	virtual ~CExplosion() override;			//デストラクタ

												//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CExplosion* Create(const D3DXVECTOR3 &pos, const D3DXQUATERNION &quaternion);

private:
	D3DXVECTOR3 m_move;				// 移動量
	D3DXVECTOR3 m_pos;				// 座標
	int m_size;						// サイズ
	D3DXQUATERNION m_quaternion;	// クォータニオン
	static const int ANIM_SPEED;
	static const int ANIM_MAX;
	int nAnimCount;
	int nAnimNum;
};

#endif