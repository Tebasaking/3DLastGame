//=========================================
//
//	ビルボードの処理
//	Author:冨所知生
//
//=========================================
#include "application.h"
#include "meshfield.h"
#include "texture.h"
#include "calculation.h"

int nLineVtx = (MESH_X_BLOCK + 1);
//=========================================
//コンストラクタ
//=========================================
CMesh::CMesh()
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_texture = CTexture::TEXTURE_SEA;
}

//=========================================
//デストラクタ
//=========================================
CMesh::~CMesh()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CMesh::Init(const D3DXVECTOR3 &pos)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESH_VERTEX_NUM,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,													//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESH_INDEX_NUM,			//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,													//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pIdxVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = pos;

	for (int nCntZ = 0; nCntZ <= MESH_Z_BLOCK; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= MESH_X_BLOCK; nCntX++)
		{
			//頂点座標の設定（ワールド座標ではなくローカル座標を指定する）
			pVtx[nCntX + (MESH_X_BLOCK + 1) * nCntZ].pos = D3DXVECTOR3
			(-(MESH_SIZE * MESH_X_BLOCK) / 2 + nCntX * MESH_SIZE,					// x
				sinf(nCntX * D3DX_PI * 0.25f) * 10.0f,								// y
				(MESH_SIZE * MESH_Z_BLOCK) / 2 - nCntZ * MESH_SIZE);				// z

			//各頂点の法線の設定
			pVtx[nCntX + (MESH_X_BLOCK + 1) * nCntZ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCntX + (MESH_X_BLOCK + 1) * nCntZ].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャの設定
			pVtx[nCntX + (MESH_X_BLOCK + 1) * nCntZ].tex = D3DXVECTOR2((float)nCntX, (float)nCntZ);
		}
	}

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxVtxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//縦の頂点の数
	for (int nCntZ = 0; nCntZ < MESH_Z_BLOCK; nCntZ++)
	{
		//インデックス
		int nLineTop = nCntZ * (nLineVtx * 2 + 2);

		//横の頂点の数
		for (int nCntX = 0; nCntX < nLineVtx; nCntX++)
		{//上の段の処理
			pIdx[(nCntX * 2 + 1) + nLineTop] = (WORD)(nCntX + nLineVtx * nCntZ);

			//下の段の処理
			pIdx[nCntX * 2 + nLineTop] = (WORD)(pIdx[(nCntX * 2 + 1) + nLineTop] + nLineVtx);
		}

		//縮退ポリゴンを前後と同一にする処理
		if (nCntZ < MESH_Z_BLOCK)
		{
			pIdx[(nLineVtx * 2) + nLineTop] = (WORD)(MESH_X_BLOCK + (nLineVtx)* nCntZ);
			pIdx[(nLineVtx * 2) + nLineTop + 1] = (WORD)(2 * (nLineVtx)+(nLineVtx)* nCntZ);
		}
	}

	//インデックスバッファのアンロック
	m_pIdxVtxBuff->Unlock();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CMesh::Update()
{
	NorCreate();
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CMesh::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	if (m_pIdxVtxBuff != nullptr)
	{
		m_pIdxVtxBuff->Release();
		m_pIdxVtxBuff = nullptr;
	}

	CObject::Release();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CMesh::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
	CTexture* pTexture = CApplication::GetTexture();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// 計算用マトリックス
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;

	// ワールドマトリックスの初期化
	// 行列初期化関数(第一引数の[行列]を[単位行列]に初期化)
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// 向きの反映
	// 行列回転関数 (第一引数に[ヨー(y)ピッチ(x)ロール(z)]方向の回転行列を作成)
	D3DXMatrixRotationZ(&mtxRot, -m_rot.z);

	// 行列掛け算関数 (第二引数 * 第三引数を第一引数に格納)
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);		// 行列掛け算関数

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファのデータストリームに設定
	pDevice->SetIndices(m_pIdxVtxBuff);

	// メッシュの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESH_VERTEX_NUM, 0, MESH_PRIMITIVE_NUM);

	// テクスチャの設定
	pDevice->SetTexture(0, nullptr);
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CMesh* CMesh::Create(const D3DXVECTOR3 &pos)
{
	int nNumCreate = m_nNumAll;

	CMesh* pCMesh = nullptr;

	pCMesh = new CMesh;

	if (pCMesh != nullptr)
	{
		pCMesh->Init(pos);
	}

	return pCMesh;
}

//=========================================
//座標の設定処理
//=========================================
void CMesh::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;
}

//=========================================
// テクスチャの設定
//=========================================
void CMesh::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//=========================================
//サイズの設定
//=========================================
void CMesh::SetSize(const float size)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	m_scale = size;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size, size, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size, size, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size, -size, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size, -size, 0.0f);

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();
}
//=========================================
//テクスチャの設定
//=========================================
void CMesh::SetAnim(const float Num, const int Pattern)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	float PatternSize = 1.0f / Pattern;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//texの設定
	pVtx[0].tex = D3DXVECTOR2(Num * PatternSize, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(Num * PatternSize + PatternSize, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(Num * PatternSize, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(Num * PatternSize + PatternSize, 1.0f);

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();
}

//=========================================
//オブジェクトの判定処理
//=========================================
bool CMesh::Collision(D3DXVECTOR3 *pos)
{// 当たり判定
	bool bCollison = false;

	// 頂点情報の取得
	VERTEX_3D *pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// インデックスバッファをロック
	WORD *pIdx;
	m_pIdxVtxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// ターゲット情報の宣言
	D3DXVECTOR3 posTarget = *pos;

	for (int nCntPolygon = 0; nCntPolygon < MESH_PRIMITIVE_NUM; nCntPolygon++)
	{
		// 変数宣言
		D3DXVECTOR3 V1 = pVtx[pIdx[nCntPolygon]].pos;
		D3DXVECTOR3 V2 = pVtx[pIdx[nCntPolygon + 1]].pos;
		D3DXVECTOR3 V3 = pVtx[pIdx[nCntPolygon + 2]].pos;

		V1 = WorldCastVtx(V1,m_pos,m_rot);
		V2 = WorldCastVtx(V2, m_pos, m_rot);
		V3 = WorldCastVtx(V3, m_pos, m_rot);

		if (V1 == V2
			|| V1 == V3
			|| V2 == V3)
		{// 縮退ポリゴンの場合
			continue;
		}

		// ポリゴンの辺ベクトル
		D3DXVECTOR3 P1 = V2 - V1;
		D3DXVECTOR3 P2 = V3 - V2;
		D3DXVECTOR3 P3 = V1 - V3;

		// 頂点とターゲットのベクトル
		D3DXVECTOR3 VecA = posTarget - V1;
		D3DXVECTOR3 VecB = posTarget - V2;
		D3DXVECTOR3 VecC = posTarget - V3;

		// 比較演算用の変数の定義と代入
		float fA = (P1.x * VecA.z) - (P1.z * VecA.x);
		float fB = (P2.x * VecB.z) - (P2.z * VecB.x);
		float fC = (P3.x * VecC.z) - (P3.z * VecC.x);

		if ((0.0f < fA && 0.0f < fB && 0.0f < fC)
			|| (0.0f > fA && 0.0f > fB && 0.0f > fC))
		{// 判定の設定
			bCollison = true;

			// 面法線ベクトル
			D3DXVECTOR3 norVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 面法線ベクトル
			D3DXVec3Cross(&norVec, &P1, &P2);

			//正規化
			D3DXVec3Normalize(&norVec, &norVec);

			// 位置の設定
			pos->y = V1.y - ((posTarget.x - V1.x) * norVec.x + (posTarget.z - V1.z) * norVec.z) / norVec.y;

			float randcolr = (rand() % 10) * 1.0f;
			float randcolg = (rand() % 10) * 1.0f;
			float randcolb = (rand() % 10) * 1.0f;
			pVtx[pIdx[nCntPolygon]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[pIdx[nCntPolygon + 1]].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[pIdx[nCntPolygon + 2]].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		}
	}

	// インデックスバッファのアンロック
	m_pIdxVtxBuff->Unlock();

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return bCollison;
}

//=========================================
// メッシュフィールドの法線作制
//=========================================
void CMesh::NorCreate()
{
	// 面法線ベクトルを作成する

	// 頂点情報の取得
	VERTEX_3D *pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// インデックスバッファをロック
	WORD *pIdx;
	m_pIdxVtxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MESH_PRIMITIVE_NUM; nCntPolygon++)
	{
		if (pIdx[nCntPolygon] == pIdx[nCntPolygon + 1] ||
			pIdx[nCntPolygon + 1] == pIdx[nCntPolygon + 2] ||
			pIdx[nCntPolygon + 2] == pIdx[nCntPolygon])
		{
			continue;
		}

		D3DXVECTOR3 V[3] = {};

		V[0] = pVtx[pIdx[nCntPolygon ]].pos;
		V[1] = pVtx[pIdx[nCntPolygon  + 1]].pos;
		V[2] = pVtx[pIdx[nCntPolygon  + 2]].pos;

		V[0] = WorldCastVtx(V[0], m_pos, m_rot);
		V[1] = WorldCastVtx(V[1], m_pos, m_rot);
		V[2] = WorldCastVtx(V[2], m_pos, m_rot);

		// ポリゴンの辺ベクトル
		D3DXVECTOR3 P[3] = {};

		// 辺ベクトルの算出
		P[0] = V[1] - V[0];
		P[1] = V[2] - V[0];

		// 面法線ベクトル
		D3DXVECTOR3 Nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (nCntPolygon % 2 == 0)
		{// 面法線ベクトル
			D3DXVec3Cross(&Nor, &P[0], &P[1]);
		}
		else
		{// 面法線ベクトル
			D3DXVec3Cross(&Nor, &P[1], &P[0]);
		}

		//正規化
		D3DXVec3Normalize(&Nor, &Nor);

		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			// 法線を保存する
			pVtx[pIdx[nCntPolygon + nCnt]].nor += Nor;

			// ベクトルを正規化する
			D3DXVec3Normalize(&pVtx[pIdx[nCntPolygon + nCnt]].nor, &pVtx[pIdx[nCntPolygon + nCnt]].nor);
		}
	}

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();
}