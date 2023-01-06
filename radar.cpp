//=========================================
//
//	バレットの処理(3D [ビルボード])
//	Author:冨所知生
//
//=========================================
#include "application.h"
#include "calculation.h"
#include "texture.h"
#include "explosion.h"
#include "motion_model3D.h"
#include "radar.h"
#include "camera.h"

//=========================================
//コンストラクタ
//=========================================
CRadar::CRadar()
{
	SetObjectType(OBJECT_RADAR);
}

//=========================================
//デストラクタ
//=========================================
CRadar::~CRadar()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CRadar::Init(const D3DXVECTOR3 &pos)
{
	CBillboard::Init(pos);
	m_pos = pos;

	//サイズの設定
	CBillboard::SetSize(1000.0f);

	// テクスチャの設定
	CBillboard::SetTexture(CTexture::TEXTURE_FIRE);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CRadar::Update()
{
	// 目標の座標設定
	D3DXVECTOR3 TargetPos = m_pObject->GetPosition();

	// オブジェクトの設定
	m_pos = D3DXVECTOR3(TargetPos.x, 100.0f, TargetPos.z);

	// 座標の設定
	SetPosition(m_pos);
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CRadar::Uninit()
{
	CBillboard::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CRadar::Draw()
{
	CBillboard::Draw();
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CRadar* CRadar::Create(const D3DXVECTOR3 &pos, CObject *object)
{
	CRadar* pCRadar = nullptr;

	pCRadar = new CRadar;

	if (pCRadar != nullptr)
	{
		pCRadar->Init(pos);
		pCRadar->SetObject(object);
	}

	return pCRadar;
}