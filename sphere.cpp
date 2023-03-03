//=========================================
//
//	ビルボードの処理
//	Author:冨所知生
//
//=========================================
#include "application.h"
#include "sphere.h"
#include "texture.h"

//=========================================
//コンストラクタ
//=========================================
CSphere::CSphere()
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 1000.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// テクスチャの設定
	CSphere::SetTexture(CTexture::TEXTURE_SKY);
}

//=========================================
//デストラクタ
//=========================================
CSphere::~CSphere()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CSphere::Init(const D3DXVECTOR3 &pos)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SPHERE_VERTEX_NUM,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,													//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * SPHERE_INDEX_NUM,			//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,													//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pIdxVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = pos;
	float RotZ = 0.0f;
	float Radius = 0.0f;

	for (int nCntZ = 0; nCntZ <= SPHERE_Z_BLOCK; nCntZ++)
	{
		RotZ = (D3DX_PI / SPHERE_Z_BLOCK * 0.55f) * nCntZ;
		Radius = sinf(RotZ) * SPHERE_SIZE;

		for (int nCntX = 0; nCntX <= SPHERE_X_BLOCK; nCntX++)
		{
			//頂点座標の設定（ワールド座標ではなくローカル座標を指定する）
			pVtx[nCntX + (SPHERE_X_BLOCK + 1) * nCntZ].pos = D3DXVECTOR3
			(sinf(nCntX * ((D3DX_PI * 2.0f) / SPHERE_X_BLOCK)) * Radius,				//x
				cosf(RotZ) * SPHERE_SIZE,									//y
				-cosf(nCntX  * ((D3DX_PI * 2.0f) / SPHERE_X_BLOCK)) * Radius);			//z

			//各頂点の法線の設定
			pVtx[nCntX + (SPHERE_X_BLOCK + 1) * nCntZ].nor = pVtx[nCntX + (SPHERE_X_BLOCK + 1) * nCntZ].pos;

			//法線の正規化
			D3DXVec3Normalize(&pVtx[nCntX + (SPHERE_X_BLOCK + 1) * nCntZ].nor, &pVtx[nCntX + (SPHERE_X_BLOCK + 1) * nCntZ].nor);

			//頂点カラーの設定
			pVtx[nCntX + (SPHERE_X_BLOCK + 1) * nCntZ].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャの設定
			pVtx[(nCntX + (SPHERE_X_BLOCK + 1) * nCntZ)].tex = D3DXVECTOR2((float)1.0f / SPHERE_X_BLOCK * nCntX, (float)1.0f / SPHERE_Z_BLOCK * nCntZ);
		}
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxVtxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//縦の頂点の数
	for (int nCntZ = 0; nCntZ < SPHERE_Z_BLOCK; nCntZ++)
	{
		//インデックス
		int nLineTop = nCntZ * (m_LineVtx * 2 + 2);

		//横の頂点の数
		for (int nCntX = 0; nCntX < m_LineVtx; nCntX++)
		{//上の段の処理
			pIdx[(nCntX * 2 + 1) + nLineTop] = (WORD)(nCntX + m_LineVtx * nCntZ);

			//下の段の処理
			pIdx[nCntX * 2 + nLineTop] = (WORD)(pIdx[(nCntX * 2 + 1) + nLineTop] + m_LineVtx);
		}

		//縮退ポリゴンを前後と同一にする処理
		if (nCntZ < SPHERE_Z_BLOCK)
		{
			pIdx[(m_LineVtx * 2) + nLineTop] = (WORD)(SPHERE_X_BLOCK + (m_LineVtx)* nCntZ);
			pIdx[(m_LineVtx * 2) + nLineTop + 1] = (WORD)(2 * (m_LineVtx)+(m_LineVtx)* nCntZ);
		}
	}

	//インデックスバッファのアンロック
	m_pIdxVtxBuff->Unlock();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CSphere::Update()
{

}

//=========================================
//オブジェクトの終了処理
//=========================================
void CSphere::Uninit()
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
void CSphere::Draw()
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

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングの設定

																		// ライトを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// メッシュの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SPHERE_VERTEX_NUM, 0, SPHERE_PRIMITIVE_NUM);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
																		// ライトを有効	
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// テクスチャの設定
	pDevice->SetTexture(0, nullptr);
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CSphere* CSphere::Create(const D3DXVECTOR3 &pos)
{
	int nNumCreate = m_nNumAll;

	CSphere* pCSphere = nullptr;

	pCSphere = new CSphere;

	if (pCSphere != nullptr)
	{
		pCSphere->Init(pos);
	}

	return pCSphere;
}

//=========================================
//座標の設定処理
//=========================================
void CSphere::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;
}

//=========================================
// テクスチャの設定
//=========================================
void CSphere::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//=========================================
//サイズの設定
//=========================================
void CSphere::SetSize(const float size)
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
void CSphere::SetAnim(const float Num, const int Pattern)
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