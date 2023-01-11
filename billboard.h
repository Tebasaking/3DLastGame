//=========================================
//
//	object2D.h
//	Author:冨所知生
//
//=========================================
#ifndef _BILLBOARD_H_		//このマクロ定義がされてなかったら
#define _BILLBOARD_H_		//２重インクルード防止のマクロ定義

#include "object.h"
#include "texture.h"

class CBillboard :public CObject
{
public:
	explicit CBillboard(int nPriority = 2);	//コンストラクタ
	virtual ~CBillboard();			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CBillboard* Create(const D3DXVECTOR3 &pos);
	//テクスチャの設定処理
	void SetTexture(CTexture::TEXTURE texture);	// テクスチャの設定

	void SetPosition(const D3DXVECTOR3& pos);
	void SetAnim(const float Num, const int Pattern);
	void SetSize(const float Size);
	void SetColor(const D3DXCOLOR &col);

	D3DXCOLOR GetColor() { return m_col; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//バッファ
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3				m_pos;			//座標
	CTexture::TEXTURE		m_texture;		// テクスチャの列挙型
	D3DXVECTOR3				m_size;			// 大きさ
	float					m_scale;		// 拡大率
	D3DXCOLOR m_col;						// 色
	D3DXVECTOR3 m_rot;
};

#endif