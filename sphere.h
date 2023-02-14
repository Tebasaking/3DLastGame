//=========================================
//
//	object2D.h
//	Author:冨所知生
//
//=========================================
#ifndef _SPHERE_H_		//このマクロ定義がされてなかったら
#define _SPHERE_H_		//２重インクルード防止のマクロ定義

#include "object.h"
#include "texture.h"

#define SPHERE_SIZE (10000.0f)
#define SPHERE_X_BLOCK (20)
#define SPHERE_Z_BLOCK (20)
#define SPHERE_VERTEX_NUM ((SPHERE_X_BLOCK + 1) * (SPHERE_Z_BLOCK + 1))
#define SPHERE_INDEX_NUM  (((SPHERE_X_BLOCK + 1) * 2) * ( SPHERE_Z_BLOCK  *(SPHERE_Z_BLOCK - 1)) * SPHERE_Z_BLOCK * 2)
#define SPHERE_PRIMITIVE_NUM (SPHERE_X_BLOCK * SPHERE_Z_BLOCK * 2 + 4 * (SPHERE_Z_BLOCK  - 1))

class CSphere :public CObject
{
public:
	CSphere();			//コンストラクタ
	virtual ~CSphere();			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CSphere* Create(const D3DXVECTOR3 &pos);
	//テクスチャの設定処理
	void SetTexture(CTexture::TEXTURE texture);	// テクスチャの設定

	void SetPosition(const D3DXVECTOR3& pos);
	void SetAnim(const float Num, const int Pattern);
	void SetSize(const float Size);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//バッファ
	LPDIRECT3DINDEXBUFFER9  m_pIdxVtxBuff;	//メッシュフィールドバッファ
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 m_pos;						// 座標
	CTexture::TEXTURE m_texture;			// テクスチャの列挙型
	D3DXVECTOR3 m_size;						// 拡大率
	float m_scale;							// 大きさ
	D3DXVECTOR3 m_rot;
	int m_LineVtx = (SPHERE_X_BLOCK + 1);
};

#endif