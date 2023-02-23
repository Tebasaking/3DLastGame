//=========================================
//
//	プレイヤーの処理
//	Author:冨所知生
//
//=========================================
#include "missile_alert_manager.h"
#include "application.h"
#include "bullet3D.h"

//=========================================
//コンストラクタ
//=========================================
CMissileAlertManager::CMissileAlertManager()
{
	SetObjectType(OBJECT_ALERT_MANAGAER);
}

//=========================================
//デストラクタ
//=========================================
CMissileAlertManager::~CMissileAlertManager()
{
}

//=========================================
// 更新処理
//=========================================
void CMissileAlertManager::Update()
{
	// 例外処理
	CBullet3D *pBullet = (CBullet3D*)pObject;

	if (pBullet != nullptr)
	{
		if (pBullet->GetAlert())
		{
			m_bAlert = false;
		}
		else
		{
			m_bAlert = true;
		}
	}
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CMissileAlertManager* CMissileAlertManager::Create(CObject *Obj)
{
	CMissileAlertManager* pAlertManager = nullptr;

	pAlertManager = new CMissileAlertManager;

	if (pAlertManager != nullptr)
	{
		pAlertManager->Init(D3DXVECTOR3(0.0f,0.0f,0.0f));
		pAlertManager->SetObject(Obj);
	}

	return pAlertManager;
}