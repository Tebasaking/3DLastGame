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
CBillboard::CBillboard(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetObjectType(OBJECT_EFFECT);
	m_texture = CTexture::TEXTURE_NONE;
	m_zFunc = D3DCMP_LESS;							// Zテストの優先度
	m_nAlphaValue = 0;								// アルファテストの透過率
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

	//texの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();

	SetSize(D3DXVECTOR3(10.0f,10.0f,0.0f));

	Draw();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CBillboard::Update()
{
	SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
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
//	m_texture = CTexture::TEXTURE_NONE;

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
	CTexture* pTexture = CApplication::GetTexture();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	
	// Zテストを使用する
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZFUNC, m_zFunc);

	// αテストを使用する
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlphaValue);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ライトを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxView;

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

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// ライトを有効	
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストの終了
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// テクスチャの設定
	pDevice->SetTexture(0, nullptr);

	SetMtxWorld(m_mtxWorld);

	// 元に戻す
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CBillboard* CBillboard::Create(const D3DXVECTOR3 &pos)
{
	int nNumCreate = m_nNumAll;

	CBillboard* pCBillboard = nullptr;

	pCBillboard = new CBillboard(LAYER_FIVE);

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
void CBillboard::SetSize(const D3DXVECTOR3 size)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	m_scale = size;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_scale.x, m_scale.y * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_scale.x, m_scale.y * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_scale.x,-m_scale.y * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_scale.x,-m_scale.y * 0.5f, 0.0f);

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();
}

//=========================================
//サイズの設定
//=========================================
void CBillboard::SetColor(const D3DXCOLOR &col)
{
	if (m_pVtxBuff == nullptr)
	{
		return;
	}

	// 色の設定
	m_col = col;

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの更新
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();
}
//=========================================
//テクスチャの設定
//=========================================
void CBillboard::SetAnim(const float NumX, const int PatternX, const int NumY, const int PatternY)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	float PatternSizeX = 1.0f / PatternX;
	float PatternSizeY = 1.0f / PatternY;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//texの設定
	pVtx[0].tex = D3DXVECTOR2(NumX * PatternSizeX, NumY * PatternSizeY);
	pVtx[1].tex = D3DXVECTOR2(NumX * PatternSizeX + PatternSizeX, NumY * PatternSizeY);
	pVtx[2].tex = D3DXVECTOR2(NumX * PatternSizeX, NumY * PatternSizeY + PatternSizeY);
	pVtx[3].tex = D3DXVECTOR2(NumX * PatternSizeX + PatternSizeX, NumY * PatternSizeY + PatternSizeY);

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();
}