//=========================================
//
//	バレットの処理(3D [ビルボード])
//	Author:冨所知生
//
//=========================================
#include "bullet.h"
#include "application.h"
#include "calculation.h"
#include "texture.h"
#include "explosion.h"
#include "motion_model3D.h"
#include "camera.h"

//=========================================
//コンストラクタ
//=========================================
CBullet::CBullet()
{
}

//=========================================
//デストラクタ
//=========================================
CBullet::~CBullet()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CBullet::Init(const D3DXVECTOR3 &pos)
{
	CBillboard::Init(pos);
	m_pos = pos;

	//サイズの設定
	CBillboard::SetSize(10.0f);

	// テクスチャの設定
	CBillboard::SetTexture(CTexture::TEXTURE_FIRE);

	// ライフ設定
	SetHP(100);

	CCamera *pCamera = CApplication::GetCamera();
	Camera_Type = pCamera->GetMode();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CBullet::Update()
{
	// 古い座標に保存する
	m_posOld = m_pos;

	// ライフの情報の取得
	int nLife = GetHP();

	// ライフの減少
	nLife = ManageHP(-1);

	// 座標の設定
	SetPosition(m_pos);

	if (nLife <= 0)
	{
		CBullet::Uninit();
	}

	int Size = GetScale();

	CObject *object = m_pTargetObj;

	if (m_pTargetObj != nullptr)
	{
		D3DXVECTOR3 posTarget = object->GetPosition();
		D3DXMATRIX	*TargetMatrix = object->GetMtxWorld();
		D3DXVECTOR3 SizeTarget = object->GetSize();

		switch (Camera_Type)
		{
		case CCamera::TYPE_FREE:
			// 弾の移動
			m_pos = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, 50.0f), m_pos, m_quaternion);
			break;

		case CCamera::TYPE_SHOULDER:
			// 弾の移動
			m_TargetPos = object->GetPosition();
			m_pos += LockOn(TargetMatrix);
			break;
		}

		// 当たり判定
		bool bCollision = Collision(object,false);

		if (bCollision)
		{
			// 弾の終了
			CBullet::Uninit();
			CExplosion::Create(m_pos, m_quaternion);
			object->ManageHP(-1);
		}
	}
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CBullet::Uninit()
{
	CBillboard::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CBullet::Draw()
{
	CBillboard::Draw();
}

//=========================================
// ロックオンの処理
//=========================================
D3DXVECTOR3 CBullet::LockOn(D3DXMATRIX *mtxWorld)
{
	//情報の取得
	D3DXMATRIX *mtx = GetMtxWorld();
	D3DXVECTOR3 BulletPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&BulletPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), mtx);
	D3DXVECTOR3 posTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&posTarget, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), mtxWorld);

	// 敵のいる向き
	D3DXVECTOR3 sub = D3DXVECTOR3(0.0f, 0.0f, 0.f);
	D3DXVECTOR3 distance = posTarget - BulletPos;

	// 三平方で敵の方向を出す
	float fDistanceXZ = sqrtf((distance.x * distance.x) + (distance.z * distance.z));
	float fFellowRotY = atan2f(distance.x, distance.z);
	float fFellowRotX = atan2f(fDistanceXZ, distance.y);

	sub.z = sinf(fFellowRotX) * cosf(fFellowRotY) * 100.0f;
	sub.x = sinf(fFellowRotX) * sinf(fFellowRotY) * 100.0f;
	sub.y = cosf(atan2f(distance.y, distance.y))  * 100.0f;

	return sub;
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CBullet* CBullet::Create(const D3DXVECTOR3 &pos, const D3DXQUATERNION &quaternion, CObject *object)
{
	CBullet* pCBullet = nullptr;

	pCBullet = new CBullet;

	if (pCBullet != nullptr)
	{
		pCBullet->Init(pos);
		pCBullet->m_quaternion = quaternion;
		pCBullet->SetTargetObj(object);
	}

	return pCBullet;
}