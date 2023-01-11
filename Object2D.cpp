//=========================================
//
//	2Dオブジェクトの処理
//	Author:冨所知生
//
//=========================================
#include "object2D.h"
#include "application.h"
#include "texture.h"

#define POLYGON_SIZE (400.0f)

//=========================================
//コンストラクタ
//=========================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_texture = CTexture::TEXTURE_NONE;
}

//=========================================
//デストラクタ
//=========================================
CObject2D::~CObject2D()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CObject2D::Init(const D3DXVECTOR3 &pos)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	m_pos = pos;

	m_texture = CTexture::TEXTURE_NONE;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点をアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CObject2D::Update()
{
	//頂点座標更新処理
	VtxUpdate();
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CObject2D::Uninit()
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
void CObject2D::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	CTexture* pTexture = CApplication::GetTexture();

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0,pTexture->GetTexture(m_texture));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//テクスチャの解除
	pDevice->SetTexture(0, NULL);

	//アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CObject2D* CObject2D::Create(const D3DXVECTOR3 &pos,int nPriority)
{
	CObject2D* pObject2D = nullptr;

	pObject2D = new CObject2D(nPriority);
	
	if (pObject2D != nullptr)
	{
		pObject2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pObject2D->Init(pos);
	}

	return pObject2D;
}

//=========================================
//座標の設定処理
//=========================================
void CObject2D::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;

	VtxUpdate();
}

//=========================================
// テクスチャの設定
//=========================================
void CObject2D::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//=========================================
//サイズの設定
//=========================================
void CObject2D::SetScale(const D3DXVECTOR3 Size)
{
	m_scale = Size;

	VtxUpdate();
}

//=========================================
//テクスチャの設定
//=========================================
void CObject2D::SetAnim(const float Num,const int Pattern)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

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

//=============================================================================
// 頂点座標更新処理
//=============================================================================
void CObject2D::VtxUpdate()
{
	if (m_pVtxBuff != nullptr)
	{
		//頂点情報へのポインタ
		VERTEX_2D*pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_scale.x, m_pos.y - m_scale.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_scale.x, m_pos.y - m_scale.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_scale.x, m_pos.y + m_scale.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_scale.x, m_pos.y + m_scale.y, 0.0f);

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
// カラーの設定
//=========================================
void CObject2D::SetColor(const D3DXCOLOR &col)
{
	m_col = col;
	VtxUpdate();
}