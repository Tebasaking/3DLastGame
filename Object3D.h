////=========================================
////
////	3Dモデルのヘッダーファイル
////	Author : 冨所知生
////
////=========================================
//#ifndef _OBJECT3D_H_								
//#define _OBJECT3D_H_							
//
//#include "main.h"
//#include "object.h"
//#include "texture.h"
//
//class CObject3D :public CObject
//{
//public:
//	CObject3D();			//コンストラクタ
//	virtual ~CObject3D();			//デストラクタ
//
//	// 初期化処理
//	HRESULT Init(const D3DXVECTOR3 &pos) override;
//	// 更新処理
//	void Update(void) override;
//	// 描画処理
//	void Draw(void) override;
//	// 影の描画処理
//	void DrawShadow(void);
//	// 終了処理
//	void Uninit(void) override;
//	// クリエイト処理
//	static CObject3D* Create(const D3DXVECTOR3 &pos);
//	// テクスチャの設定処理
//	void SetTexture(CTexture::TEXTURE texture);	// テクスチャの設定
//	
//	// モデルの設定
//	void SetModel(const char* name);
//	void SetPosition(const D3DXVECTOR3& pos);
//	void SetRot(const D3DXVECTOR3& pos);
//	void SetSize(const float Size);
//	void SetQuaternion(const D3DXQUATERNION quaternion);
//	float NormalizRot(float fRot);
//	float LimitedRot(float fRot, float LimitRot);
//	
//	const D3DXVECTOR3& GetPosition() const override { return m_pos; };
//	const D3DXVECTOR3& GetRot() const override { return m_rot; }
//	const float& GetSize() const override { return m_size; }
//	const D3DXQUATERNION& GetQuaternion() const { return m_quaternion; }
//
//private:
//	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// バッファ
//	CTexture::TEXTURE m_texture;						// テクスチャの列挙型
//	float m_size;										// 大きさ
//
//	LPD3DXMESH				m_pMeshModel = NULL;		// メッシュ情報へのポインタ
//	LPD3DXBUFFER			m_pBuffMatModel = NULL;		// マテリアル情報へのポインタ
//	DWORD					m_nNumMatModel = 0;			// マテリアル情報の数
//	D3DXVECTOR3				m_pos;						// 位置
//	D3DXVECTOR3				m_rot;						// 回転
//	D3DXVECTOR3				m_rotModel;					// 向き
//	D3DXMATRIX				m_mtxWorldModel;			// ワールドマトリックス
//	LPD3DXEFFECT			pEffect;					// シェーダー
//	IDirect3DTexture9		*pTex0 = NULL;				// テクスチャ保存用
//
//	D3DXQUATERNION			m_quaternion;				//クォータニオン
//};
//
//#endif