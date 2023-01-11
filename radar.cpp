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
#include "Object2D.h"
#include "radar.h"
#include "camera.h"

//=========================================
//コンストラクタ
//=========================================
CRadar::CRadar(int nPriority)
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

	m_pBackGround = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.25f * 0.5f, SCREEN_HEIGHT * 0.5f * 0.5f,0.0f),0);
	m_pBackGround->SetTexture(CTexture::TEXTURE_FRAME);
	m_pBackGround->SetScale(D3DXVECTOR3(SCREEN_WIDTH * 0.25f * 0.5f, SCREEN_HEIGHT * 0.5f * 0.5f,0.0f));
	m_pBackGround->SetObjectType(OBJECT_RADAR);

	switch (m_type)
	{
	case RADAR_MAP:
		// テクスチャの設定
		CBillboard::SetTexture(CTexture::TEXTURE_RADAR_MAP);
		//サイズの設定
		CBillboard::SetSize(10000.0f);
		// COLOR設定
		CBillboard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case RADAR_PLAYER:
		// テクスチャの設定
		CBillboard::SetTexture(CTexture::TEXTURE_ENEMY_FLY);
		//サイズの設定
		CBillboard::SetSize(200.0f);
		// COLOR設定
		CBillboard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f));
		break;

	case RADAR_ENEMY:
		// テクスチャの設定
		CBillboard::SetTexture(CTexture::TEXTURE_ENEMY_TANK);
		//サイズの設定
		CBillboard::SetSize(200.0f);
		// COLOR設定
		CBillboard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CRadar::Update()
{
	D3DXCOLOR col = GetColor();

	/*if (m_type != RADAR_MAP)
	{*/
		// 目標の座標設定
		D3DXVECTOR3 TargetPos = m_pObject->GetPosition();

		// オブジェクトの設定
		m_pos = D3DXVECTOR3(TargetPos.x, 1090.0f, TargetPos.z);

		// 座標の設定
		SetPosition(m_pos);
	//}

	CBillboard::Update();

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
CRadar* CRadar::Create(const D3DXVECTOR3 &pos, CObject *object, RADAR_TYPE type)
{
	CRadar* pCRadar = nullptr;

	pCRadar = new CRadar(5);

	if (pCRadar != nullptr)
	{
		pCRadar->SetType(type);
		pCRadar->Init(pos);
		pCRadar->SetObject(object);
	}

	return pCRadar;
}