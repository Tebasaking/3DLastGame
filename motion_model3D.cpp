//=============================================================================
//
// モーションキャラクター3Dクラス(model3D.h)
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3D生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "motion_model3D.h"
#include "motion.h"
#include "render.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3Dを生成する
//=============================================================================
CMotionModel3D * CMotionModel3D::Create()
{
	// オブジェクトインスタンス
	CMotionModel3D *pMotionModel3D = nullptr;

	// メモリの解放
	pMotionModel3D = new CMotionModel3D;

	// メモリの確保ができなかった
	assert(pMotionModel3D != nullptr);

	// 数値の初期化
	pMotionModel3D->Init(D3DXVECTOR3(0.0f,0.0f,0.0f));

	// インスタンスを返す
	return pMotionModel3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CMotionModel3D::CMotionModel3D(int nPriority) : CObject(nPriority),
m_pMotion(nullptr),		// モーション情報
m_pos(D3DXVECTOR3()),										// 位置
m_posOld(D3DXVECTOR3()),									// 過去位置
m_size(D3DXVECTOR3())										// 大きさ
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CMotionModel3D::~CMotionModel3D()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CMotionModel3D::Init(const D3DXVECTOR3 &pos)
{
	// 変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 過去位置
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 大きさ

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CMotionModel3D::Uninit()
{
	if (m_pMotion != nullptr)
	{// 終了処理
		m_pMotion->Uninit();

		// メモリの解放
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// オブジェクト2Dの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 3D更新を行う
//=============================================================================
void CMotionModel3D::Update()
{
	if (m_pMotion != nullptr)
	{// モーション番号の設定
		m_pMotion->Update();
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 3D描画を行う
//=============================================================================
void CMotionModel3D::Draw()
{
	if (this == nullptr)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxWorld, mtxRot, mtxTrans, mtxScaling;

	// 描画の終了
	pDevice->EndScene();

	D3DXVECTOR3 rot = GetRot();

	if (SUCCEEDED(pDevice->BeginScene()))
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);											// 行列初期化関数

		if (CApplication::GetMode() == CApplication::MODE_TITLE)
		{// タイトルだけサイズを適応する
			m_size = GetSize();
			// サイズの反映
			D3DXMatrixScaling(&mtxScaling, m_size.x, m_size.y, m_size.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScaling);					// 行列掛け算関数
		}

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);			// 行列回転関数
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);						// 行列掛け算関数 

		// クォータニオンの反映
		D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);						// クオータニオンによる行列回転
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);				// 行列移動関数
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);					// 行列掛け算関数

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	}

	pDevice->EndScene();

	if (m_pMotion != nullptr)
	{// パーツの描画設定
		m_pMotion->SetParts(mtxWorld);
	}

	// マトリックスの設定
	SetMtxWorld(mtxWorld);
}

//=============================================================================
// モーションの設定
// Author : 唐﨑結斗
// 概要 : モーションの読み込みを行う
//=============================================================================
void CMotionModel3D::SetMotion(const char * pName)
{
	if (m_pMotion != nullptr)
	{// 終了処理
		m_pMotion->Uninit();

		// メモリの解放
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// モーション情報
	m_pMotion = new CMotion(pName);
	assert(m_pMotion != nullptr);

	// モーションの初期設定
	m_pMotion->SetMotion(0);

	// モーション番号の設定
	m_pMotion->SetNumMotion(0);
}

//=============================================================================
// モーションの設定
// Author : 唐﨑結斗
// 概要 : モーションの読み込みを行い、引数で指定されたモーションに設定する
//=============================================================================
void CMotionModel3D::SetMotion(const char * pName, const int nNumMotion)
{
	if (m_pMotion != nullptr)
	{// 終了処理
		m_pMotion->Uninit();

		// メモリの解放
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// モーション情報
	m_pMotion = new CMotion(pName);
	assert(m_pMotion != nullptr);

	// モーションの初期設定
	m_pMotion->SetMotion(nNumMotion);

	// モーション番号の設定
	m_pMotion->SetNumMotion(nNumMotion);
}
