//=========================================
//
//	ミサイルアラートの処理
//	Author:冨所知生
//
//=========================================
#include "missile_alert.h"
#include "missile_alert_manager.h"
#include "camera.h"
#include "application.h"
#include "debug_proc.h"
#include "bullet3D.h"
#include "game.h"
#include "playerUI.h"

D3DXVECTOR3 CAlert::m_TargetPos = {};
//=========================================
//コンストラクタ
//=========================================
CAlert::CAlert()
{
	SetObjectType(OBJECT_ALERT);
}

//=========================================
//デストラクタ
//=========================================
CAlert::~CAlert()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CAlert::Init(const D3DXVECTOR3 &pos)
{
	m_bCheck = false;

	//初期化処理
	CObject2D::Init(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f,0.0f));

	// テクスチャの設定
	CObject2D::SetTexture(CTexture::TEXTURE_UI_ALERT);

	//サイズの設定
	CObject2D::SetScale(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	// タイプ設定
	//SetObjectType(OBJECT_TARGET);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CAlert::Update()
{
	// 全てのアラートからアラートが起動しているものがあった時、bCheckをtrueにする。
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		CObject *object = CObject::GetObjectTop(nCnt);

		//プレイヤーの座標を取得
		while (object)
		{
			if (object != nullptr)
			{
				EObjType ObjType = object->GetObjectType();

				if (ObjType == OBJECT_ALERT_MANAGAER)
				{
					CMissileAlertManager *pAlert = dynamic_cast<CMissileAlertManager*> (object);

					if (pAlert->GetAlert())
					{
						m_bCheck = true;
						break;
					}
					else
					{
						m_bCheck = false;
					}
				}
			}
			object = object->GetObjectNext();
		}

		if (m_bCheck)
		{
			break;
		}
	}

	if (m_bCheck)
	{
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
	//CDebugProc::Print("missileの座標 x %.1f y ,%.1f,z %.1f \n", GetTest().x, GetTest().y, GetTest().z);
	// カメラの視点
	//CDebugProc::Print("ターゲットのサイズ %f \n", m_Size);
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CAlert::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CAlert::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

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

	{
		// 2Dオブジェクトの描画処理
		CObject2D::Draw();
	}

	// Zテストの終了
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// αテストの終了
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CAlert* CAlert::Create(const D3DXVECTOR3 &pos)
{
	CAlert* pCAlert = nullptr;

	pCAlert = new CAlert;

	if (pCAlert != nullptr)
	{
		pCAlert->Init(pos);
	}

	return pCAlert;
}