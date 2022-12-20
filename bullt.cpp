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

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CBullet::Update()
{
	// ライフの情報の取得
	int nLife = GetHP();

	// ライフの減少
	nLife = ManageHP(-1);

	// 弾の移動テスト
	m_pos = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, 50.0f), m_pos, m_quaternion);

	// 座標の設定
	SetPosition(m_pos);

	if (nLife <= 0)
	{
		CBullet::Uninit();
	}

	int Size = GetScale();

	CObject *object = CObject::GetObjectTop();


	//=========================================
	// 弾とエネミーの当たり判定
	//=========================================
	while(object)
	{
		if (object != nullptr)
		{
			EObjType ObjType = object->GetObjectType();
			D3DXVECTOR3 size = GetSize();

			if (ObjType == OBJECT_ENEMY)
			{
				D3DXVECTOR3 posTarget = object->GetPosition();
				D3DXVECTOR3 SizeTarget = object->GetSize();

				bool bCollision = Collision(m_pos, m_posOld, posTarget, size, SizeTarget, false);

				if (bCollision)
				{
					// 弾の終了
					CBullet::Uninit();
					CExplosion::Create(m_pos,m_quaternion);
					object->ManageHP(-1);
				}
			}
		}
		
		object = object->GetObjectNext();
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
//オブジェクトのクリエイト
//=========================================
CBullet* CBullet::Create(const D3DXVECTOR3 &pos, const D3DXQUATERNION &quaternion)
{

	CBullet* pCBullet = nullptr;

	pCBullet = new CBullet;

	if (pCBullet != nullptr)
	{
		pCBullet->Init(pos);
		pCBullet->m_quaternion = quaternion;
	}

	return pCBullet;
}