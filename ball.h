////=========================================
////
////	object2D.h
////	Author:冨所知生
////
////=========================================
//#ifndef _BALL_H_		//このマクロ定義がされてなかったら
//#define _BALL_H_		//２重インクルード防止のマクロ定義
//
//#include "main.h"
//#include "object.h"
//#include "texture.h"
//
//class CModel;
//class CBall :public CObject
//{
//public:
//	CBall();			//コンストラクタ
//	virtual ~CBall();			//デストラクタ
//
//								//初期化処理
//	HRESULT Init(const D3DXVECTOR3 &pos) override;
//	//更新処理
//	void Update(void) override;
//	//描画処理
//	void Draw(void) override;
//	//終了処理
//	void Uninit(void) override;
//	//クリエイト処理
//	static CBall* Create(const D3DXVECTOR3 &pos);
//	//テクスチャの設定処理
//	void SetTexture(CTexture::TEXTURE texture);	// テクスチャの設定
//	//コントロール処理
//	void Control_();
//
//	void SetPosition(const D3DXVECTOR3& pos);
//	const D3DXVECTOR3& GetPosition() const override { return m_pos; }
//	const D3DXVECTOR3& GetRot() const override { return m_rot; }
//	const float& GetSize() const override { return m_size; }
//
//private:
//	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//バッファ
//	LPDIRECT3DINDEXBUFFER9  m_pIdxVtxBuff;	//メッシュフィールドバッファ
//	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
//	D3DXVECTOR3 m_pos;						//座標
//	CTexture::TEXTURE m_texture;			// テクスチャの列挙型
//	float m_size;							// 大きさ
//	D3DXVECTOR3 m_rot;
//	CModel *pModel;							// モデルの個数
//};
//
//#endif