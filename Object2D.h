//=========================================
//
//	object2D.h
//	Author:冨所知生
//
//=========================================
#ifndef _OBJECT2D_H_		//このマクロ定義がされてなかったら
#define _OBJECT2D_H_		//２重インクルード防止のマクロ定義

#include "main.h"
#include "object.h"
#include "texture.h"

class CObject2D :public CObject
{
public:
	explicit CObject2D(int nPriority = LAYER_ONE);
	virtual ~CObject2D();			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CObject2D* Create(const D3DXVECTOR3 &pos,int nPriority);
	//テクスチャの設定処理
	void SetTexture(CTexture::TEXTURE texture);	// テクスチャの設定

	void VtxUpdate();
	void SetColor(const D3DXCOLOR &col);
	void SetPosition(const D3DXVECTOR3& pos);
	void SetRotation(const D3DXVECTOR3& rot) { m_rot = rot; }
	void SetAnim(const int Num,const int Pattern);
	void SetScale(const D3DXVECTOR3 Size);

	D3DXVECTOR3 GetRotation() { return m_rot; }
	D3DXVECTOR3 GetTest(){ return m_pos; };

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//バッファ
	D3DXVECTOR3 m_pos;						//座標
	CTexture::TEXTURE m_texture;			// テクスチャの列挙型
	D3DXVECTOR3 m_size;						// 大きさ
	D3DXVECTOR3	m_scale;					// 拡大率
	D3DXVECTOR3 m_rot;
	D3DXCOLOR m_col;						// 色
	float m_fLength;						// 長さ
	float m_fAngle;							// 対角線
};

#endif