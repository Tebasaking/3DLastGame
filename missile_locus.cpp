//=========================================
//
//	爆発の処理(3D [ビルボード])
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

const int CMissile_Locus::ANIM_SPEED = 1;
const int CMissile_Locus::ANIM_MAX = 6;
//=========================================
//コンストラクタ
//=========================================
CMissile_Locus::CMissile_Locus()
{
	nAnimCount = 0;
	nAnimNum = 0;
}

//=========================================
//デストラクタ
//=========================================
CMissile_Locus::~CMissile_Locus()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CMissile_Locus::Init(const D3DXVECTOR3 &pos)
{
	CBillboard::Init(pos);

	// 座標
	m_pos = pos;

	m_RandAlfa = (float)(rand() % 10 + 1) * 0.01f;

	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	
	//サイズの設定
	CBillboard::SetSize(D3DXVECTOR3(20.0f,20.0f,0.0f));

	// テクスチャの設定
	CBillboard::SetTexture(CTexture::TEXTURE_SMOKE);

	// 体力の設定
	SetHP(100);

	// Zバッファの設定
	SetZFunc(D3DCMP_LESS);

	// アルファ量の設定
	SetAlphaValue(100);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CMissile_Locus::Update()
{
	int nLife = GetHP();

	nLife = ManageHP(-1);

	SetPosition(m_pos);
	
	m_col = GetColor();
	m_col.a -= m_RandAlfa;

	SetColor(m_col);

	D3DXVECTOR3 Scale = GetSize();

	Scale.x += 1.0f;
	Scale.y += 1.0f;

	//サイズの設定
	CBillboard::SetSize(Scale);

	int Size = (float)GetScale();

	nAnimCount++;

	CBillboard::SetAnim((float)nAnimNum, ANIM_MAX,nAnimNumY,6);

	// アニメーション処理
	if (nAnimCount >= ANIM_SPEED)
	{
		nAnimNum++;
		nAnimCount = 0;

		if (nAnimNum >= ANIM_MAX)
		{
			nAnimNumY++;
			nAnimNum = 0;

			if (nAnimNumY >= 6)
			{
				nAnimNumY = 0;
			}
			//CMissile_Locus::Uninit();
		}
	}

	if (m_col.a <= 0)
	{
		Uninit();
	}
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CMissile_Locus::Uninit()
{
	CBillboard::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CMissile_Locus::Draw()
{
	CBillboard::Draw();
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CMissile_Locus* CMissile_Locus::Create(const D3DXVECTOR3 &pos, CObject *object)
{
	int nNumCreate = m_nNumAll;

	CMissile_Locus* pLocus = nullptr;

	pLocus = new CMissile_Locus;

	if (pLocus != nullptr)
	{
		pLocus->Init(pos);
		//CSound::PlaySound(CSound::SOUND_SE_EXPLOSION);
	}

	return pLocus;
}