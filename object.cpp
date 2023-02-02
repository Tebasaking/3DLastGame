//=========================================
//
//	オブジェクトの処理
//	Author:冨所知生
//
//=========================================
#include "object.h"
#include "camera.h"
#include "application.h"
#include <assert.h>

int CObject::m_nNumAll = 0;
CObject *CObject::m_pTop[LAYER_MAX] = { nullptr };
CObject *CObject::m_pCurrent[LAYER_MAX] = { nullptr };
//=========================================
//コンストラクタ
//=========================================
CObject::CObject(int nPriority /*= LAYER_ONE*/)
{
	// オブジェクトを通常モードにする
	m_objmode = MAX_MODE;

	//先頭ポインターが割り振られていない時
	if (m_pTop[nPriority] == nullptr)
	{
		//先頭のポインターを割り振る
		m_pTop[nPriority] = this;
	}
	else
	{
		//一つ前のポインターの情報をCurrentに入れる
		m_pPrev = m_pCurrent[nPriority];
		m_pCurrent[nPriority]->m_pNext = this;
	}

	//カレントに現在のポインターをいれる
	m_pCurrent[nPriority] = this;
	//死亡フラグをfalseにする
	m_DeathFlag = false;

 	m_Prioryty = nPriority;
}

//=========================================
//デストラクタ
//=========================================
CObject::~CObject()
{

}

//=========================================
//全ての更新処理
//=========================================
void CObject::UpdateAll()
{
	for (int nCnt = 0; nCnt < LAYER_MAX; nCnt++)
	{
		if (m_pTop[nCnt] != nullptr)
		{
			{
				CObject *pBox = m_pTop[nCnt];

				while (pBox != nullptr)
				{
					CObject *pObjectNext = pBox->m_pNext;

					if (!pBox->m_DeathFlag)
					{
						// 更新処理
						pBox->Update();
					}
					pBox = pObjectNext;
				}
			}

			{
				CObject *pBox = m_pTop[nCnt];

				while (pBox != nullptr)
				{
					CObject *pObjectNext = pBox->m_pNext;

					if (pBox->m_DeathFlag)
					{
						pBox->DFlagDelete();
					}

					pBox = pObjectNext;
				}
			}
		}
	}
}

//=========================================
//全ての描画処理
//=========================================
void CObject::DrawAll(Object_mode mode)
{
	// カメラをアプリケーションから取得
	CCamera *pCamera = nullptr;

	switch (mode)
	{
	case NORMAL_MODE:
		// カメラをアプリケーションから取得
		pCamera = CApplication::GetCamera();
		break;

	case RADAR_MODE:
		// レーダー用カメラをアプリケーションから取得
		pCamera = CApplication::GetRader();
		break;

	default:
		break;
	}

	// カメラの設定
	pCamera->Set();

	for (int nCnt = 0; nCnt < LAYER_MAX; nCnt++)
	{
		if (m_pTop[nCnt] != nullptr)
		{
			CObject *pBox = m_pTop[nCnt];

			while (pBox != nullptr)
			{
				if (pBox->m_type != OBJECT_RADAR && pCamera->GetObjType() == NORMAL_MODE)
				{
					//描画処理
					pBox->Draw();
				}
				else if (pBox->m_type == OBJECT_RADAR && pCamera->GetObjType() == RADAR_MODE)
				{// レーダーの描画
					pBox->Draw();
				}
				pBox = pBox->m_pNext;
			}
		}
	}
}

//=========================================
//全ての破棄処理
//=========================================
void CObject::UninitAll()
{
	for (int nCnt = 0; nCnt < LAYER_MAX; nCnt++)
	{
		if (m_pTop[nCnt] != nullptr)
		{
			CObject *pDeleteBox = m_pTop[nCnt];

			while (pDeleteBox != nullptr)
			{
				CObject *pSaveBox = pDeleteBox->m_pNext;

				pDeleteBox->Uninit();

				pDeleteBox = pSaveBox;
			}
		}
	}

	for (int nCnt = 0; nCnt < LAYER_MAX; nCnt++)
	{
		if (m_pTop[nCnt] != nullptr)
		{
			CObject *pBox = m_pTop[nCnt];

			while (pBox != nullptr)
			{
				CObject *pObjectNext = pBox->m_pNext;

				if (pBox->m_DeathFlag)
				{
					pBox->DFlagDelete();
				}

				pBox = pObjectNext;
			}
		}
	}
}

//=========================================
// 解放処理
//=========================================
void CObject::Release()
{
	if (!m_DeathFlag)
	{
		m_DeathFlag = true;
	}
}

//=========================================
// 死亡フラグを処理する処理
//=========================================
void CObject::DFlagDelete()
{
	//先頭のポインターだったら先頭に次のポインターを代入
	if (m_pTop[m_Prioryty] == this)
	{
		m_pTop[m_Prioryty] = m_pTop[m_Prioryty]->m_pNext;
	}
	//最後のポインターだったら一つ前のポインターを代入
	else if (m_pCurrent[m_Prioryty] == this)
	{
		m_pCurrent[m_Prioryty] = m_pCurrent[m_Prioryty]->m_pPrev;
	}
	// 中間のポインターだったら次のポインターに一つ前を、一つ前のポインターに次のポインターを。
	if (m_pPrev != nullptr)
	{// 前のオブジェクトの前のオブジェクトに自分の次のオブジェクトを設定
		m_pPrev->m_pNext = m_pNext;
	}
	if (m_pNext != nullptr)
	{// 自分の次のオブジェクトに自分の前のオブジェクトを設定
		m_pNext->m_pPrev = m_pPrev;
	}

	delete this;
}

//==================================================================================
// 視野の当たり判定
// 引数: 目標の座標 、 扇の中心 、 範囲(角度) 、長さ , 方向(角度)
//==================================================================================
bool CObject::SearchEye(D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2, float rangeRadian, float length, float FanRadian)
{
	D3DXVECTOR3 Vec1To2xz = Vec1 - Vec2;
	Vec1To2xz.y = 0.0f;

	// ベクトルの長さの算出
	float vec_length = sqrtf((Vec1To2xz.x * Vec1To2xz.x) + (Vec1To2xz.z * Vec1To2xz.z));

	// ベクトルと扇の長さの比較
	if (length < vec_length)
	{
		// 当たっていない
		return false;
	}

	//扇の方向ベクトルを求める
	D3DXVECTOR3 Fan_dir = D3DXVECTOR3(cosf(FanRadian + D3DX_PI * 0.5f), 0.0f, sinf(FanRadian - D3DX_PI * 0.5f));

	Vec1To2xz /= vec_length;

	// 内積計算
	float dot = Vec1To2xz.x * Fan_dir.x + Vec1To2xz.z * Fan_dir.z;

	// 扇の範囲をcosにする
	float fan_cos = cosf(/*D3DXToRadian(*/rangeRadian / 4.0f/*)*/);

	if (dot > 0)
	{
		if (fan_cos > dot)
		{
			// 当たってない
			return false;
		}
	}

	////=========================================
	////xyの計算
	////=========================================
	//D3DXVECTOR3 Vec1To2xy = { Vec1.x - Vec2.x,Vec1.y - Vec2.y , 0.0f };

	//// ベクトルの長さの算出
	//vec_length = sqrtf((Vec1To2xy.x * Vec1To2xy.x) + (Vec1To2xy.y * Vec1To2xy.y));

	//// ベクトルと扇の長さの比較
	//if (length < vec_length)
	//{
	//	// 当たっていない
	//	return false;
	//}

	////扇の方向ベクトルを求める
	//direction_rad = ((FanDegree) * (D3DX_PI / 180.0f));
	//fan_dir = D3DXVECTOR3(cosf(direction_rad), sinf(direction_rad), 0.0f);

	//D3DXVec3Normalize(&Vec1To2xy, &Vec1To2xy);

	//// 内積計算
	//dot = Vec1To2xy.x * fan_dir.x + Vec1To2xy.y * fan_dir.y;

	//// 扇の範囲をcosにする
	//fan_cos = cosf((rangeDegree / 2.0f) * (D3DX_PI / 180.0f));

	//if (fan_cos > dot)
	//{
	//	// 当たってない
	//	return false;
	//}

	return true;
}

//=========================================
// 座標の設定
//=========================================
void CObject::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;
}

//=========================================
// オブジェクトのタイプを分類する
//=========================================
CObject *CObject::GetObjectType(CObject::EObjType type)
{
	CObject *pObject = CObject::GetObjectTop();

	while (0)
	{

	}

	return pObject;
}

//=========================================
// HPを管理する
//=========================================
int CObject::ManageHP(int Value)
{
	m_HP += Value;

	return m_HP;
}

//===========================================================================
// オブジェクトの当たり判定
// 引数 : ターゲットのオブジェクト情報、押し返すか返さないか
//===========================================================================
bool CObject::Collision(CObject *Target, bool bExtrude)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 size = GetSize();
	
	D3DXVECTOR3 posTarget = Target->GetPosition();
	D3DXVECTOR3 SizeTarget = Target->GetSize();

	// 当たった
	if ((pos.z - size.z) < (posTarget.z + SizeTarget.z)
		&& (pos.z + size.z) > (posTarget.z - SizeTarget.z)
		&& (pos.x - size.x) < (posTarget.x + SizeTarget.x)
		&& (pos.x + size.x) > (posTarget.x - SizeTarget.x)
		&& (pos.y - size.y) < (posTarget.y + SizeTarget.y)
		&& (pos.y + size.y) > (posTarget.y - SizeTarget.y))
	{
		bCollision = true;
	}

	// 生まれてしまう僅かなズレを解決する値
	float fCollision = 1.0f;

	if ((pos.z - size.z) < (posTarget.z + SizeTarget.z)
		&& (pos.z + size.z) > (posTarget.z - SizeTarget.z)
		&& (pos.x - size.x) < (posTarget.x + SizeTarget.x)
		&& (pos.x + size.x) > (posTarget.x - SizeTarget.x))
	{// モデル内にいる(XZ軸)
		if ((posOld.y + size.y) <= (posTarget.y - SizeTarget.y)
			&& (pos.y + size.y) > (posTarget.y - SizeTarget.y))
		{
			if (bExtrude)
			{
				pos.y = posTarget.y - SizeTarget.y - size.y - fCollision;
			}
		}
		if ((posOld.y - size.y) >= (posTarget.y + SizeTarget.y)
			&& (pos.y - size.y) < (posTarget.y + SizeTarget.y))
		{
			if (bExtrude)
			{
				pos.y = posTarget.y + SizeTarget.y + size.y + fCollision;
			}
		}
	}

	if ((pos.y - size.y) < (posTarget.y + SizeTarget.y)
		&& (pos.y + size.y) > (posTarget.y - SizeTarget.y))
	{
		if ((pos.z - size.z) < (posTarget.z + SizeTarget.z)
			&& (pos.z + size.z) > (posTarget.z - SizeTarget.z))
		{// モデル内にいる(Z軸)
			if ((posOld.x + size.z) <= (posTarget.x - SizeTarget.x)
				&& (pos.x + size.z) > (posTarget.x - SizeTarget.x))
			{
				if (bExtrude)
				{
					pos.x = posTarget.x - SizeTarget.x - size.z - fCollision;
				}
			}
			if ((posOld.x - size.z) >= (posTarget.x + SizeTarget.x)
				&& (pos.x - size.z) < (posTarget.x + SizeTarget.x))
			{
				if (bExtrude)
				{
					pos.x = posTarget.x + SizeTarget.x + size.z + fCollision;
				}
			}
		}
		if ((pos.x - size.x) < (posTarget.x + SizeTarget.x)
			&& (pos.x + size.x) > (posTarget.x - SizeTarget.x))
		{// モデル内にいる(X軸)
			if ((posOld.z + size.z) <= (posTarget.z - SizeTarget.z)
				&& (pos.z + size.z) > (posTarget.z - SizeTarget.z))
			{
				if (bExtrude)
				{
					pos.z = posTarget.z - SizeTarget.z - size.z - fCollision;
				}
			}
			if ((posOld.z - size.z) >= (posTarget.z + SizeTarget.z)
				&& (pos.z - size.z) < (posTarget.z + SizeTarget.z))
			{
				if (bExtrude)
				{
					pos.z = posTarget.z + SizeTarget.z + size.z + fCollision;
				}
			}
		}
	}

	SetPosition(pos);

	return bCollision;
}

//=========================================
// 2点から角度を求める
//=========================================
D3DXVECTOR3 CObject::AtanRot(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	D3DXVECTOR3 distance = pos1 - pos2;
	D3DXVECTOR3 AtanRot = {};

	// 三平方で敵の方向を出す
	AtanRot.z = sqrtf((distance.x * distance.x) + (distance.z * distance.z));
	AtanRot.y = atan2f(distance.x, distance.z);
	AtanRot.x = atan2f(AtanRot.z, distance.y);

	return AtanRot;
}

//=========================================
// 角度の正規化処理
// 引数 : 正規化したい角度
//=========================================
D3DXVECTOR3 CObject::NormalizeRotXYZ(D3DXVECTOR3 Rot)
{
	D3DXVECTOR3 NormRot;

	NormRot.x = NormalizeRot(Rot.x);
	NormRot.y = NormalizeRot(Rot.y);
	NormRot.z = NormalizeRot(Rot.z);

	return NormRot;
}

float CObject::NormalizeRot(float fRot)
{
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}
	else if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2;
	}
	return fRot;
}

//===========================================================
// 行列変換する処理
// 概要 : 受け取ったposとmtxで行列計算する処理
// 主な使用 : 座標を受け取った時
// 引数 : ずらす座標の大きさ、クォータニオン、目標座標
//============================================================
D3DXVECTOR3 CObject::MtxPos(D3DXVECTOR3 pos, D3DXQUATERNION qua, D3DXVECTOR3 TargetPos)
{
	// もしかして必要？ワールドマトリックス
	D3DXMATRIX WorldMtx;

	// 計算用マトリックス
	D3DXMATRIX mtxBullet, mtxTrans, mtxQua;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMtx);

	// 弾の座標を書き換える
	D3DXMatrixTranslation(&mtxBullet, pos.x, pos.y, pos.z);			// 行列移動関数
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &mtxBullet);			// 行列掛け算関数

																	// クォータニオン(回転)の反映
	D3DXMatrixRotationQuaternion(&mtxQua, &qua);					// クオータニオンによる行列回転
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &mtxQua);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

																	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, TargetPos.x, TargetPos.y, TargetPos.z);		// 行列移動関数
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &mtxTrans);							// 行列掛け算関数

																					// 行列から座標を取り出す
	D3DXVECTOR3 MtxPos = D3DXVECTOR3(WorldMtx._41, WorldMtx._42, WorldMtx._43);

	return MtxPos;
}