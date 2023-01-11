//=========================================
//
//	ビルボードの処理
//	Author:冨所知生
//
//=========================================
#include "billboard.h"
#include "application.h"
#include "texture.h"

#define POLYGON_SIZE (400.0f)

//=========================================
//コンストラクタ
//=========================================
CBillboard::CBillboard(int nPriority)
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_texture = CTexture::TEXTURE_NONE;
}

//=========================================
//デストラクタ
//=========================================
CBillboard::~CBillboard()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CBillboard::Init(const D3DXVECTOR3 &pos)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_3D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,		// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	m_texture = CTexture::TEXTURE_FIRE;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = pos;

	//各頂点の法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//texの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();

	SetSize(10.0f);

	Draw();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CBillboard::Update()
{
	if (m_pVtxBuff != nullptr)
	{
		//頂点情報へのポインタ
		VERTEX_3D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの更新
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CBillboard::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	CObject::Release();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CBillboard::Draw()
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

	// Zテストを使用する
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	// αテストを使用する
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ライトを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 計算用マトリックス
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;

	// ワールドマトリックスの初期化
	// 行列初期化関数(第一引数の[行列]を[単位行列]に初期化)
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// カメラ逆行列を設定
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// 向きの反映
	// 行列回転関数 (第一引数に[ヨー(y)ピッチ(x)ロール(z)]方向の回転行列を作成)
	D3DXMatrixRotationZ(&mtxRot,-m_rot.z);

	// 行列掛け算関数 (第二引数 * 第三引数を第一引数に格納)
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);		// 行列掛け算関数
	
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ビルボード描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//デバイス設定の初期化
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	// ライトを有効	
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// テクスチャの設定
	pDevice->SetTexture(0, nullptr);

	SetMtxWorld(m_mtxWorld);
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CBillboard* CBillboard::Create(const D3DXVECTOR3 &pos)
{
	int nNumCreate = m_nNumAll;

	CBillboard* pCBillboard = nullptr;

	pCBillboard = new CBillboard;

	if (pCBillboard != nullptr)
	{
		pCBillboard->Init(pos);
	}

	return pCBillboard;
}

//=========================================
//座標の設定処理
//=========================================
void CBillboard::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;
}

//=========================================
// テクスチャの設定
//=========================================
void CBillboard::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//=========================================
//サイズの設定
//=========================================
void CBillboard::SetSize(const float size)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	m_scale = size;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_scale, m_scale * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_scale, m_scale * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_scale,-m_scale * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_scale,-m_scale * 0.5f, 0.0f);

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();
}

//=========================================
//サイズの設定
//=========================================
void CBillboard::SetColor(const D3DXCOLOR &col)
{
	m_col = col;
}
//=========================================
//テクスチャの設定
//=========================================
void CBillboard::SetAnim(const float Num, const int Pattern)
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