//=============================================================================
//
// パーティクルクラス(particle.h)
// Author : 唐﨑結斗
// 概要 : パーティクル生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <time.h>
#include "radar.h"
#include "particle.h"
#include "effect.h"
#include "render.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : スコアを生成する
//=============================================================================
CParticle *CParticle::Create()
{
	// オブジェクトインスタンス
	CParticle *pParticle = nullptr;

	pParticle = new CParticle;

	// メモリの確保ができなかった
	assert(pParticle != nullptr);

	// 数値の初期化
	pParticle->Init(D3DXVECTOR3(0.0f,0.0f,0.0f));

	// 疑似乱数の初期化
	srand((unsigned int)time(NULL));

	// インスタンスを返す
	return pParticle;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CParticle::CParticle(int nPriority) : CObject(nPriority)
{
	SetObjectType(OBJECT_EFFECT);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
	m_popRange = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 生成範囲
	m_moveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動方向
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_fSpeed = 0.0f;									// 速度
	m_nLife = 0;										// 寿命
	m_nMaxEffect = 0;									// 位置フレーム出すエフェクト数
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CParticle::Init(const D3DXVECTOR3 &pos)
{
	// 位置の設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 向きの設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 大きさの設定
	m_size = D3DXVECTOR3(30.0f, 30.0f, 0.0f);

	// 位置フレーム出すエフェクト数
	m_nMaxEffect = 1;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CParticle::Uninit()
{
	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CParticle::Update()
{
	m_nLife--;

	SetEffect();

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CParticle::Draw()
{

}

//=============================================================================
// エフェクトの生成
// Author : 唐﨑結斗
// 概要 : エフェクトの生成
//=============================================================================
void CParticle::SetEffect()
{
	// 変数宣言
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	D3DXVECTOR3 moveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動方向
	float fSize = 0;								// 大きさ
	float fSpeed = 0.0f;									// 速度
	int nEffectLife = 0;									// 寿命
	m_size = GetSize();
	m_pos = GetPosition();
	m_quaternion = GetQuaternion();

	for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
	{// 数値の算出
		pos.x = m_pos.x + (float)(-(int)(m_popRange.x * 0.5f) + rand() % ((int)(m_popRange.x) + 1));
		pos.y = m_pos.y + (float)(-(int)(m_popRange.y) + rand() % ((int)(m_popRange.y) + 1));
		pos.z = m_pos.z + (float)(-(int)(m_popRange.z * 0.5f) + rand() % ((int)(m_popRange.z) + 1));
		
		fSize = (float)(rand() % ((int)(sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y)) * 50.0f) + 1)) * 0.01f;
		
		moveVec.x = (((int)(m_moveVec.x * 100.0f) * 0.5f) - (rand() % ((int)(m_moveVec.x * 100.0f) + 1))) * 0.01f;
		moveVec.y = (((int)(m_moveVec.y * 100.0f) * 0.5f) - (rand() % ((int)(m_moveVec.y * 100.0f) + 1))) * 0.01f;
		moveVec.x = NormalizeRot(moveVec.x);
		moveVec.y = NormalizeRot(moveVec.y);
		nEffectLife = rand() % (m_nEffectLife + 1) + 1;
		fSpeed = FloatRandam(m_fSpeed,1.0f);

		CEffect *pEffect = CEffect::Create(pos,0,0,CTexture::TEXTURE_PARTIClE);
		pEffect->SetPosition(pos);
		pEffect->SetSize(D3DXVECTOR3(fSize, fSize, 0.0f));
		pEffect->SetMoveVec(moveVec);
		pEffect->SetLife(nEffectLife);
		pEffect->SetSpeed(fSpeed);
		pEffect->SetRenderMode(CEffect::MODE_ADD);
		pEffect->SetColor(m_color);
		pEffect->SetTex(CTexture::TEXTURE_PARTIClE);
		pEffect->SetQuaternion(m_quaternion);
	}
}

//---------------------------------------------------------------------------
// 小数点のランダム
//---------------------------------------------------------------------------
float CParticle::FloatRandam(float fMax, float fMin)
{
	return ((rand() / (float)RAND_MAX) * (fMax - fMin)) + fMin;
}
