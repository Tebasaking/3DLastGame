////=========================================
////
////	3Dオブジェクトの処理
////	Author:冨所知生
////
////=========================================
//#include "object3D.h"
//#include "application.h"
//#include "inputkeyboard.h"
//#include "texture.h"
//#include "camera.h"
//#include "input.h"
//#include "light.h"
//#define POLYGON_SIZE (400.0f)
//
////=========================================
////コンストラクタ
////=========================================
//CObject3D::CObject3D()
//{
//	m_pVtxBuff = nullptr;
//	m_texture = CTexture::TEXTURE_NONE;
//
//	m_pMeshModel = nullptr;
//	m_pBuffMatModel = nullptr;
//	m_nNumMatModel = 0;
//	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_quaternion = D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f);
//	m_mtxWorldModel = {};
//
//	pEffect = NULL;
//}
//
////=========================================
////デストラクタ
////=========================================
//CObject3D::~CObject3D()
//{
//
//}
//
////=========================================
////オブジェクトの初期化処理
////=========================================
//HRESULT CObject3D::Init(const D3DXVECTOR3 &pos)
//{
//	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
//
//	// テクスチャ生成
//	D3DXCreateTextureFromFile(pDevice, ("data\\F16_Thuderbirds4.bmp"), &pTex0);
//
//	return S_OK;
//}
//
////=========================================
////オブジェクトの更新処理
////=========================================
//void CObject3D::Update()
//{
//}
//
////=========================================
////オブジェクトの終了処理
////=========================================
//void CObject3D::Uninit()
//{
//	//メッシュの破棄
//	if (m_pMeshModel != NULL)
//	{
//		m_pMeshModel->Release();
//		m_pMeshModel = NULL;
//	}
//
//	//マテリアルの破棄
//	if (m_pBuffMatModel != NULL)
//	{
//		m_pBuffMatModel->Release();
//		m_pBuffMatModel = NULL;
//	}
//
//	CObject::Release();
//}
//
////=========================================
////オブジェクトの描画処理
////=========================================
//void CObject3D::Draw()
//{
//	PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
//
//	// 計算用マトリックス
//	D3DXMATRIX mtxRot, mtxTrans, mtxScale;
//	// 現在のマテリアル保存用
//	D3DMATERIAL9 matDef;
//	// マテリアルデータへのポインタ
//	D3DXMATERIAL *pMat;
//	D3DXVECTOR3 scale(5.0f, 5.0f, 5.0f);
//
//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングの設定
//
//	//// ライトを無効
//	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&m_mtxWorldModel);
//
//	// 行列拡縮関数
//	//D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
//	//// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
//	//D3DXMatrixMultiply(&m_mtxWorldModel, &m_mtxWorldModel, &mtxScale);
//
//	// 向きを反映
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
//	D3DXMatrixMultiply(&m_mtxWorldModel, &m_mtxWorldModel, &mtxRot);
//
//	// 位置を反映
//	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);                // 行列移動関数
//	D3DXMatrixMultiply(&m_mtxWorldModel, &m_mtxWorldModel, &mtxTrans);          // 行列掛け算関数
//
//	//*****************************************
//	// 影の表示
//	//*****************************************
//	//影の処理
//	DrawShadow();
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldModel);
//
//	//DWORD d;
//	//pDevice->GetRenderState(D3DRS_AMBIENT, &d);
//	//pDevice->SetRenderState(D3DRS_AMBIENT, 0x88888888);
//
//	// 現在のマテリアルを保持
//	pDevice->GetMaterial(&matDef);
//
//	// マテリアルデータへのポインタを取得
//	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();
//
//	for (int i = 0; i < (int)m_nNumMatModel; i++)
//	{
//		pMat[i].MatD3D.Ambient = pMat[i].MatD3D.Diffuse;
//
//			// 引数を色に設定
////		pMat[i].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
////		pMat[i].MatD3D.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		//pMat[i].MatD3D.Power = 5.0f;
//
//		// マテリアルの設定
//		pDevice->SetMaterial(&pMat[i].MatD3D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, pTex0);
//
//		// モデルパーツの描画
//		m_pMeshModel->DrawSubset(i);
//	}
//
//	// 保存していたマテリアルを戻す
//	pDevice->SetMaterial(&matDef);
//
//	// テクスチャの設定
//	pDevice->SetTexture(0, NULL);
//
//	// ライトを無効
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
//}
//
////=========================================
//// オブジェクトのクリエイト
//// 引数 : オブジェクトを生成したい座標
////=========================================
//CObject3D* CObject3D::Create(const D3DXVECTOR3 &pos)
//{
//	//int nNumCreate = m_nNumAll;
//
//	CObject3D* pObject3D = nullptr;
//
//	pObject3D = new CObject3D;
//
//	if (pObject3D != nullptr)
//	{
//		pObject3D->Init(pos);
//	}
//
//	return pObject3D;
//}
//
////=========================================
//// 座標の設定処理
//// 引数 : m_posに代入したい座標の値
////=========================================
//void CObject3D::SetPosition(const D3DXVECTOR3& pos)
//{
//	m_pos = pos;
//}
//
////=========================================
//// 回転の設定取得
//// 引数 : m_rotに代入したい角度の値
////=========================================
//void CObject3D::SetRot(const D3DXVECTOR3& rot)
//{
//	m_rot = rot;
//}
//
////=========================================
//// テクスチャの設定
//// 引数 : テクスチャのファイル名の指定
////=========================================
//void CObject3D::SetTexture(CTexture::TEXTURE texture)
//{
//	m_texture = texture;
//}
//
////=========================================
//// サイズの設定
//// 引数 : サイズの指定
////=========================================
//void CObject3D::SetSize(const float size)
//{
//	m_size = size;
//}
//
////=========================================
//// モデルの設定
//// 引数 : ファイル名の入力
////=========================================
//void CObject3D::SetModel(const char* name)
//{
//	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
//
//	//Xファイルの読み込み
//	D3DXLoadMeshFromX(name,
//		D3DXMESH_SYSTEMMEM,
//		pDevice,
//		NULL,
//		&m_pBuffMatModel,
//		NULL,
//		&m_nNumMatModel,
//		&m_pMeshModel);
//}
//
////=========================================
////クォータニオンの設置
////=========================================
//void CObject3D::SetQuaternion(const D3DXQUATERNION quaternion)
//{
//	// クオータニオンを適用
//	m_quaternion = quaternion;
//
//	// クオータニオンのノーマライズ
//	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
//}
//
////=========================================
//// 角度の正規化処理
//// 引数 : 正規化したい角度
////=========================================
//float CObject3D::NormalizRot(float fRot)
//{
//	if (fRot < -D3DX_PI)
//	{
//		fRot += D3DX_PI * 2;
//	}
//	else if (fRot > D3DX_PI)
//	{
//		fRot -= D3DX_PI * 2;
//	}
//	return fRot;
//}
//
////=========================================
//// 角度の上限設定
//// 引数 : 制限を設けたい角度、限界角度
////=========================================
//float CObject3D::LimitedRot(float fRot, float fLimitRot)
//{
//	if (fRot < D3DX_PI * -fLimitRot)
//	{
//		fRot = D3DX_PI * -fLimitRot;
//	}
//	else if (fRot > D3DX_PI * fLimitRot)
//	{
//		fRot = D3DX_PI * fLimitRot;
//	}
//	return fRot;
//}
//
////=========================================
//// 影の描画処理
//// 概要 : Drawで行う落影の処理
////=========================================
//void CObject3D::DrawShadow()
//{
//	extern D3DLIGHT9 g_light;	// カメラ情報
//
//	PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
//
//	// 現在のマテリアル保存用
//	D3DMATERIAL9 matDef;
//
//	// マテリアルデータへのポインタ
//	D3DXMATERIAL *pMat;
//
//	// 変数宣言
//	D3DXMATRIX		mtxShadow = {};										// シャドウマトリックス
//	D3DXPLANE		planeField = {};									// 平面化用変数
//	D3DXVECTOR4		vecLight = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);		// ライト方向の逆ベクトル
//	D3DXVECTOR3		posShadow = D3DXVECTOR3(0.0f, -13.0f, 0.0f);			// 影の位置
//	D3DXVECTOR3		norShadow = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 影の法線
//	D3DXVECTOR3		light = g_light.Direction;
//	D3DXVECTOR3		lightVec = light * -1;								// ライト方向
//
//	// シャドウマトリックスの初期化
//	D3DXMatrixIdentity(&mtxShadow);    // 行列初期化関数
//
//	// ライト方向の逆ベクトルの設定
//	vecLight = D3DXVECTOR4(lightVec.x, lightVec.y, lightVec.z, 0.0f);
//
//	// 平面化用変数の設定
//	D3DXPlaneFromPointNormal(&planeField, &posShadow, &norShadow);
//
//	// シャドウマトリックスの設定
//	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);
//
//	// 行列掛け算関数
//	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorldModel, &mtxShadow);
//
//	// シャドウマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);
//
//	// 現在のマテリアルを保持
//	pDevice->GetMaterial(&matDef);
//
//	for (int nCntMat = 0; nCntMat < (int)m_nNumMatModel; nCntMat++)
//	{
//		//マテリアルデータのポインタを取得する
//		pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();
//
//		// マテリアル情報の設定
//		D3DMATERIAL9  matD3D = pMat->MatD3D;
//
//		// 引数を色に設定
//		matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
//		matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
//
//		// マテリアルの設定
//		pDevice->SetMaterial(&matD3D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, nullptr);
//
//		// モデルパーツの描画
//		m_pMeshModel->DrawSubset(nCntMat);
//	}
//
//	// 保持していたマテリアルを戻す
//	pDevice->SetMaterial(&matDef);
//}