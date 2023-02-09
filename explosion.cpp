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
#include "sound.h"

const int CExplosion::ANIM_SPEED = 10;
const int CExplosion::ANIM_MAX = 8;
//=========================================
//コンストラクタ
//=========================================
CExplosion::CExplosion()
{
	nAnimCount = 0;
	nAnimNum = 0;
}

//=========================================
//デストラクタ
//=========================================
CExplosion::~CExplosion()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CExplosion::Init(const D3DXVECTOR3 &pos)
{
	CBillboard::Init(pos);

	// 座標
	m_pos = pos;

	//サイズの設定
	CBillboard::SetSize(D3DXVECTOR3(150.0f,150.0f,0.0f));

	// テクスチャの設定
	CBillboard::SetTexture(CTexture::TEXTURE_EXPLOSION);

	// 体力の設定
	SetHP(100);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CExplosion::Update()
{
	int nLife = GetHP();

	nLife = ManageHP(-1);

	SetPosition(m_pos);

	if (nLife <= 0)
	{
		CExplosion::Uninit();
	}

	int Size = (float)GetScale();

	nAnimCount++;

	CBillboard::SetAnim((float)nAnimNum, ANIM_MAX);

	// アニメーション処理
	if (nAnimCount >= ANIM_SPEED)
	{
		nAnimNum++;
		nAnimCount = 0;

		if (nAnimNum >= ANIM_MAX)
		{
			nAnimNum = 0;
			CExplosion::Uninit();
		}
	}
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CExplosion::Uninit()
{
	CBillboard::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CExplosion::Draw()
{
	CBillboard::Draw();
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CExplosion* CExplosion::Create(const D3DXVECTOR3 &pos, const D3DXQUATERNION &quaternion)
{
	int nNumCreate = m_nNumAll;

	CExplosion* pExplosion = nullptr;

	pExplosion = new CExplosion;

	if (pExplosion != nullptr)
	{
		pExplosion->Init(pos);
		pExplosion->m_quaternion = quaternion;
		CSound::PlaySound(CSound::SOUND_SE_EXPLOSION);
	}

	return pExplosion;
}