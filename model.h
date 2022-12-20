////=========================================
////
////	3Dモデルのヘッダーファイル
////	Author : 冨所知生
////
////=========================================
//#ifndef _MODEL_H_								
//#define _MODEL_H_							
//
//#include "main.h"
//#include "object.h"
//#include "texture.h"
//
//class CModel
//{
//public:
//	CModel();			//コンストラクタ
//	virtual ~CModel();			//デストラクタ
//
//	// 初期化処理
//	HRESULT Init(const D3DXVECTOR3 &pos);
//	// 更新処理
//	void Update(void);
//	// 描画処理
//	void Draw();
//	void Draw(D3DXMATRIX mtxParent);
//	// 影の描画処理
//	void DrawShadow(void);
//	// 終了処理
//	void Uninit(void);
//	// テクスチャの設定処理
//	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }	// テクスチャの設定
//
//	// モデルの設定
//	void SetModel(const char* name);
//	void SetPosition(const D3DXVECTOR3& pos);
//	void SetRot(const D3DXVECTOR3& pos);
//	void SetScale(const float Size);
//	void SetQuaternion(const D3DXQUATERNION quaternion);
//	void SetParent(CModel *pModel) { m_pParents = pModel; }
//	D3DXMATRIX GetMtxWorld() { return m_mtx; }
//
	//float NormalizRot(float fRot);
	//float LimitedRot(float fRot, float LimitRot);
//
//	D3DXVECTOR3 GetModelSize();
//	const D3DXVECTOR3& GetPosition() const { return m_pos; };
//	const D3DXVECTOR3& GetRot() const { return m_rot; }
//	const D3DXVECTOR3& GetSize() const { return m_size; }
//	const D3DXQUATERNION& GetQuaternion() const { return m_quaternion; }
//
//	const float& GetScale() const { return m_scale; }
//	CModel *GetParent() { return m_pParents; }								// 親モデルのゲッター
//
//private:
//	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// バッファ
//	CTexture::TEXTURE		m_texture;					// テクスチャの列挙型
//	float m_scale;										// 大きさ
//	const char* m_filename;								// モデルのファイル名
//	CModel *m_pParents;									// 親モデルへのポインタ
//
//	LPD3DXMESH				m_pMeshModel = NULL;		// メッシュ情報へのポインタ
//	LPD3DXBUFFER			m_pBuffMatModel = NULL;		// マテリアル情報へのポインタ
//	DWORD					m_nNumMatModel = 0;			// マテリアル情報の数
//	D3DXVECTOR3				m_pos;						// 位置
//	D3DXVECTOR3				m_rot;						// 回転
//	D3DXVECTOR3				m_rotModel;					// 向き
//	D3DXVECTOR3				vtxMin;						// 最小
//	D3DXVECTOR3				vtxMax;						// 最大
//	D3DXVECTOR3				m_size;						// モデルサイズ
//	D3DXMATRIX				m_mtx;						// ワールドマトリックス
//	IDirect3DTexture9		*pTex0 = NULL;				// テクスチャ保存用
//};
//
//#endif