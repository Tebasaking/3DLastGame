//=========================================
//
//	3Dエフェクトの処理(3D [ビルボード])
//	Author:冨所知生
//
//=========================================
#include "explosion.h"
#include "application.h"
#include "texture.h"
#include "calculation.h"
#include "explosion.h"
#include "missile_locus.h"
#include "sound.h"
#include "effect.h"

//=========================================
//コンストラクタ
//=========================================
CEffect::CEffect()
{
	SetObjectType(OBJECT_EFFECT);
	nAnimCount = 0;
	nAnimNum = 0;
}

//=========================================
//デストラクタ
//=========================================
CEffect::~CEffect()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CEffect::Init(const D3DXVECTOR3 &pos)
{
	CBillboard::Init(pos);

	// 座標
	m_pos = pos;

	m_RandAlfa = (float)(rand() % 10 + 1) * 0.01f;

	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// Zバッファの設定
	SetZFunc(D3DCMP_ALWAYS);

	// αテストの透過率の設定
	SetAlphaValue(50);
	
	//サイズの設定
	CBillboard::SetSize(D3DXVECTOR3(40.0f,40.0f,0.0f));

	// テクスチャの設定
	CBillboard::SetTexture(m_tex);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CEffect::Update()
{
	// 位置の取得
	//D3DXVECTOR3 pos = GetPosition();
	D3DXCOLOR col = GetColor();

	//// 色をランダムで下げていく
	//m_col = GetColor();
	col.a -= 0.01f;

	SetColor(col);

	D3DXVECTOR3 size = GetSize();
	size.x += -0.75f;
	size.y += -0.75f;
	size.z += -0.75f;
	SetSize(size);

	// 位置の更新
	//SetPosition(pos);

	// ライフの減少
	DecreaseLife();

	if (col.a <= 0.0f || m_nLife == 0 || size.x <= 0.0f)
	{
		Uninit();
	}
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CEffect::Uninit()
{
	CBillboard::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CEffect::Draw()
{
	// レンダーの取得
	CRender *pRender = CApplication::GetRender();
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	switch (m_renderMode)
	{
	case CEffect::MODE_NORMAL:
		break;

	case CEffect::MODE_ADD:
		// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	case CEffect::MODE_SUB:
		// αブレンディングを減算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	default:
		break;
	}

	CBillboard::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CEffect* CEffect::Create(const D3DXVECTOR3 &pos, int ANIM_SPEED, int ANIM_MAX, CTexture::TEXTURE tex)
{
	int nNumCreate = m_nNumAll;

	CEffect* pEffect = nullptr;

	pEffect = new CEffect;

	if (pEffect != nullptr)
	{
		pEffect->m_tex = tex;
		pEffect->SetTexture(tex);
		pEffect->Init(pos);
		pEffect->m_ANIM_MAX = ANIM_MAX;
		pEffect->m_ANIM_SPEED = ANIM_SPEED;
	}

	return pEffect;
}

//=============================================================================
// ライフの減少処理
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CEffect::DecreaseLife()
{
	if (m_nLife > 0)
	{
		D3DXVECTOR3 size = GetSize();
		D3DXCOLOR color = GetColor();

		// ライフの減少
		m_nLife--;
		SetSize(size - m_subSize);
		SetColor(D3DXCOLOR(color.r, color.g, color.b, color.a - m_fSubAlpha));

		if (m_nLife <= 0)
		{
			m_nLife = 0;
		}
	}
}
