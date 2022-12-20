//=========================================
//
//	bullet3D.h
//	Author:冨所知生
//
//=========================================
#ifndef _BULLET3D_H_		//このマクロ定義がされてなかったら
#define _BULLET3D_H_		//２重インクルード防止のマクロ定義

#include "main.h"
#include "billboard.h"

class CBullet :public CBillboard
{
public:
	CBullet();			//コンストラクタ
	virtual ~CBullet() override;			//デストラクタ

											//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CBullet* Create(const D3DXVECTOR3 &pos, const D3DXQUATERNION &quaternion);

private:
	D3DXVECTOR3 m_pos;				// 座標
	D3DXVECTOR3 m_posOld;			// 一つ前の座標
	D3DXVECTOR3 m_move;				// 移動量
	D3DXQUATERNION m_quaternion;	// クォータニオン
};

#endif