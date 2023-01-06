//=========================================
//
//	object2D.h
//	Author:冨所知生
//
//=========================================
#ifndef _MESH_FIELD_H_		//このマクロ定義がされてなかったら
#define _MESH_FIELD_H_		//２重インクルード防止のマクロ定義

#include "main.h"
#include "object.h"
#include "texture.h"
#include "Object3D.h"

#define MESH_SIZE (1000.0f)
#define MESH_X_BLOCK (20)
#define MESH_Z_BLOCK (20)
#define MESH_VERTEX_NUM ((MESH_X_BLOCK + 1) * (MESH_Z_BLOCK + 1))
#define MESH_INDEX_NUM  (((MESH_X_BLOCK + 1) * 2) * ( MESH_Z_BLOCK  *(MESH_Z_BLOCK - 1)) * MESH_Z_BLOCK * 2)
#define MESH_PRIMITIVE_NUM (MESH_X_BLOCK * MESH_Z_BLOCK * 2 + 4 * (MESH_Z_BLOCK  - 1))							//ポリゴンの数？

class CMesh :public CObject
{
public:

	enum MeshType
	{
		TYPE_SEA = 0,
		TYPE_GROUND,
	};

	CMesh();			//コンストラクタ
	virtual ~CMesh();			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CMesh* Create(const D3DXVECTOR3 &pos, MeshType type);
	//テクスチャの設定処理
	void SetTexture(CTexture::TEXTURE texture);	// テクスチャの設定
	
	void SetPosition(const D3DXVECTOR3& pos);
	void SetAnim(const float Num, const int Pattern);
	void SetSize(const float Size);

	void NorCreate();								// メッシュフィールドの法線作成
	bool Collision(D3DXVECTOR3 *pos);				// 判定処理

	// メッシュタイプの設定
	void SetType(MeshType type) { m_type = type; }
	

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//バッファ
	LPDIRECT3DINDEXBUFFER9  m_pIdxVtxBuff;	//メッシュフィールドバッファ
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3				m_pos;			// 座標
	CTexture::TEXTURE m_texture;			// テクスチャの列挙型
	D3DXVECTOR3 m_size;						// 大きさ
	float m_scale;							// 拡大率
	float m_polygon;						// ポリゴン数
	D3DXVECTOR3 m_rot;
	MeshType	m_type;						// メッシュのタイプ
	// 法線格納
	D3DXVECTOR3	m_norBox[MESH_PRIMITIVE_NUM];
};

#endif