//=========================================
//
//	ターゲットの処理
//	Author:冨所知生
//
//=========================================
#include "target.h"
#include "camera.h"
#include "application.h"
#include "debug_proc.h"

D3DXVECTOR3 CTarget::m_TargetPos = {};
//=========================================
//コンストラクタ
//=========================================
CTarget::CTarget()
{

}

//=========================================
//デストラクタ
//=========================================
CTarget::~CTarget()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CTarget::Init(const D3DXVECTOR3 &pos)
{
	//初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	CObject2D::SetTexture(CTexture::TEXTURE_TARGET);

	//サイズの設定
	CObject2D::SetScale(D3DXVECTOR3(0.0f,0.0f,0.0f));

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CTarget::Update()
{
	D3DXVECTOR3 EnemyPos;			// エネミーの変換前座標
	D3DXVECTOR3 PlayerPos;			// プレイヤーの座標

	CCamera *pCamera = CApplication::GetCamera();

	//プレイヤーの座標を取得
	if (pObject != nullptr)
	{
		// 目標の座標をスクリーン座標に変換する
		EnemyPos = pObject->GetPosition();
		m_TargetPos = CApplication::WorldToScreen(EnemyPos);
	}

	if (m_TargetPos.x >= 0.0f && m_TargetPos.x <= SCREEN_WIDTH &&
		m_TargetPos.y >= 0.0f && m_TargetPos.y <= SCREEN_HEIGHT &&
		m_TargetPos.z >= 0.0f)
	{
		CObject *object = CObject::GetObjectTop();

		//プレイヤーの座標を取得
		while (object)
		{
			if (object != nullptr)
			{
				EObjType ObjType = object->GetObjectType();

				if (ObjType == OBJECT_PLAYER)
				{
					PlayerPos = object->GetPosition();

					if (PlayerPos.x > 0.0f || PlayerPos.x < 0.0f)
					{
						int nCnt = 0;
					}
					break;
				}
			}
			object = object->GetObjectNext();
		}

		//プレイヤーとエネミーの距離
		D3DXVECTOR3 PtoE_Diss = PlayerPos - EnemyPos;
		float PtoE_Distance = sqrtf((PtoE_Diss.y * PtoE_Diss.y) + (PtoE_Diss.x * PtoE_Diss.x) + (PtoE_Diss.z * PtoE_Diss.z));

		//距離がマイナスだったらプラスにする処理
		//※これのせいでプレイヤーの後ろにエネミーが行った時多分ターゲットが大きくなるべきなんだけど小さくなる
		if (PtoE_Distance <= 0)
		{
			PtoE_Distance *= -1;
		}

		float Conversion = ((1.0f / PtoE_Distance));

		//距離に応じてサイズを決める
		float Size = 10000.0f * (Conversion);

		//最大サイズ
		if (Size >= 200.0f)
		{
			Size = 200.0f;
		}
		//最小サイズ
		else if (Size <= 0)
		{
			Size = 1.0f;
		}

		//サイズの設定
		CObject2D::SetScale(D3DXVECTOR3(Size,Size,0.0f));

		//スクリーン座標に設定する
		SetPosition(m_TargetPos);
	}
	else
	{
		//サイズの設定
		CObject2D::SetScale(D3DXVECTOR3(0.0f,0.0f,0.0f));
	}
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CTarget::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CTarget::Draw()
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
CTarget* CTarget::Create(const D3DXVECTOR3 &pos, CObject *object)
{
	int nNumCreate = m_nNumAll;

	CTarget* pCTarget = nullptr;

	pCTarget = new CTarget;

	if (pCTarget != nullptr)
	{
		pCTarget->Init(pos);
	}
	
	pCTarget->SetObject(object);

	return pCTarget;
}