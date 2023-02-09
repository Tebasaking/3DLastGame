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
#include <vector>

#define MESH_SIZE (100.0f)

class CMesh :public CObject
{
public:

	enum MeshType
	{
		TYPE_SEA = 0,
		TYPE_WAVE,
		TYPE_GROUND,
	};

	explicit CMesh(int nPriority = LAYER_ONE);	//コンストラクタ
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
	// タイプの取得処理
	MeshType GetType() { return m_type; }
	
	void SetMeshSize(float size) { m_MeshSize = size; }
	void SetPosition(const D3DXVECTOR3& pos);
	void SetAnim(const float Num, const int Pattern);
	void SetSize(const float Size);

	void NorCreate();								// メッシュフィールドの法線作成
	bool Collision(D3DXVECTOR3 *pos);				// 判定処理

	// メッシュタイプの設定
	void SetType(MeshType type) { m_type = type; }
	// ブロック数設定
	void SetBlock(float XBlock, float ZBlock) { m_BlockX = XBlock; m_BlockZ = ZBlock; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//バッファ
	LPDIRECT3DINDEXBUFFER9  m_pIdxVtxBuff;	//メッシュフィールドバッファ
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3				m_pos;			// 座標
	CTexture::TEXTURE m_texture;			// テクスチャの列挙型
	D3DXVECTOR3 m_size;						// 大きさ

	float m_scale;							// 拡大率
	float m_polygon;						// ポリゴン数
	float m_fAddWave;						// 波を追加する
	float m_fAmplitude;						// 振れ幅の設定
	float m_MeshSize;						// メッシュのサイズ
	float m_BlockX;							// Xブロック数
	float m_BlockZ;							// Yブロック数
	bool  m_bWave;							// ウェーブ
	
	float MESH_VERTEX_NUM;
	float MESH_INDEX_NUM;
	float MESH_PRIMITIVE_NUM;

	D3DXVECTOR3 m_rot;
	MeshType	m_type;						// メッシュのタイプ

	// 頂点の高さ
	std::vector<float> m_fVtxHeight;
};

#endif