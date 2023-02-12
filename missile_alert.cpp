//=========================================
//
//	ミサイルアラートの処理
//	Author:冨所知生
//
//=========================================
#include "missile_alert.h"
#include "camera.h"
#include "application.h"
#include "debug_proc.h"
#include "bullet3D.h"

D3DXVECTOR3 CAlert::m_TargetPos = {};
//=========================================
//コンストラクタ
//=========================================
CAlert::CAlert()
{

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
	//初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	CObject2D::SetTexture(CTexture::TEXTURE_ARROW);

	//サイズの設定
	CObject2D::SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// タイプ設定
	//SetObjectType(OBJECT_TARGET);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CAlert::Update()
{
	D3DXVECTOR3 BulletPos;			// エネミーの変換前座標
	D3DXVECTOR3 PlayerPos;			// プレイヤーの座標

	m_Size = 50.0f;

	//プレイヤーの座標を取得
	if (pObject != nullptr)
	{
		// 目標の座標をスクリーン座標に変換する
		BulletPos = pObject->GetPosition();
		m_TargetPos = CApplication::WorldToScreen(BulletPos);
	}

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

				break;
			}
		}
		object = object->GetObjectNext();
	}

	// TargetPosの制限
	if (m_TargetPos.x >= 0.0f && m_TargetPos.x <= SCREEN_WIDTH &&
		m_TargetPos.y >= 0.0f && m_TargetPos.y <= SCREEN_HEIGHT &&
		m_TargetPos.z >= 0.0f)
	{
		//m_Size = 0.0f;
	}
	else
	{
		m_Size = 50.0f;
	}

	//サイズの設定
	CObject2D::SetScale(D3DXVECTOR3(m_Size, m_Size, 0.0f));

	// アラートのX制限の始点
	float ALERT_SCREEN_WIDTH_BEGIN = 300.0f;
	// アラートのY制限の始点
	float ALERT_SCREEN_HEIGHT_BEGIN = 500;

	// アラートのX制限の終点
	float ALERT_SCREEN_WIDTH_END = SCREEN_WIDTH - 300.0f;
	// アラートのY制限の終点
	float ALERT_SCREEN_HEIGHT_END = SCREEN_HEIGHT - 200.0f;

	m_TargetPos.y *= 0.1f;
	m_TargetPos.y += 500.0f;

	// アラート始点制限
	if (m_TargetPos.x >= ALERT_SCREEN_WIDTH_END)
	{
		m_TargetPos.x = ALERT_SCREEN_WIDTH_END - m_Size;
	}
	if (m_TargetPos.y <= ALERT_SCREEN_HEIGHT_BEGIN)
	{
		m_TargetPos.y = ALERT_SCREEN_HEIGHT_BEGIN + m_Size;
	}

	// 終点制限
	if (m_TargetPos.x <= ALERT_SCREEN_WIDTH_BEGIN)
	{
		m_TargetPos.x = ALERT_SCREEN_WIDTH_BEGIN + m_Size;
	}
	if (m_TargetPos.y >= ALERT_SCREEN_HEIGHT_END)
	{
		m_TargetPos.y = ALERT_SCREEN_HEIGHT_END - m_Size;
	}

	// TargetPosとPlayerPosから角度を求める
	D3DXVECTOR3 rot = AtanRot(m_TargetPos, PlayerPos);

	// スクリーン座標に設定する
	SetPosition(m_TargetPos);

	SetRotation(rot);

	// 例外処理
	CBullet3D *pBullet = (CBullet3D*)pObject;

	if (pBullet != nullptr)
	{
		if (pBullet->GetSpeed() >= 100)
		{
			SetScale(D3DXVECTOR3(0.0f,0.0f,0.0f));
		}
	}

	EObjType obj = GetObjectType();
	CAlert *pTagert = this;

	CDebugProc::Print("missileの座標 x %.1f y ,%.1f,z %.1f \n", GetTest().x, GetTest().y, GetTest().z);
	// カメラの視点
	CDebugProc::Print("ターゲットのサイズ %f \n", m_Size);
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
CAlert* CAlert::Create(const D3DXVECTOR3 &pos, CObject *object)
{
	CAlert* pCAlert = nullptr;

	pCAlert = new CAlert;

	if (pCAlert != nullptr)
	{
		pCAlert->Init(pos);
	}

	// ターゲットを設定するオブジェクトを取得
	pCAlert->SetObject(object);

	return pCAlert;
}