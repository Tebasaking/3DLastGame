////=========================================
////
////	3Dオブジェクトの処理
////	Author:冨所知生
////
////=========================================
//#include "model.h"
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
//CModel::CModel()
//{
//	m_pVtxBuff = nullptr;
//	m_texture = CTexture::TEXTURE_NONE;
//	m_pParents = nullptr;
//	m_filename = {};
//
//	m_pMeshModel = nullptr;
//	m_pBuffMatModel = nullptr;
//	m_nNumMatModel = 0;
//	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
//	m_mtx = {};
//	vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
//	vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
//
//	pEffect = NULL;
//	m_hmWVP = NULL;
//	m_hmWIT = NULL;
//	m_hvLightDir = NULL;
//	m_hvCol = NULL;
//	m_hvEyePos = NULL;
//	m_hTechnique = NULL;
//	m_hTexture = NULL;
//}
//
////=========================================
////デストラクタ
////=========================================
//CModel::~CModel()
//{
//
//}
//
////=========================================
////オブジェクトの初期化処理
////=========================================
//HRESULT CModel::Init(const D3DXVECTOR3 &pos)
//{
//	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
//	
//	D3DXCreateTextureFromFile(pDevice, ("Data\\F16_Thuderbirds4.bmp"), &pTex0);
//
//	D3DXCreateEffectFromFile(
//		pDevice, "Effect.fx", NULL, NULL,
//		0, NULL, &pEffect, nullptr);
//
//	m_hTechnique = pEffect->GetTechniqueByName("Diffuse");				//エフェクト
//	m_hTexture = pEffect->GetParameterByName(NULL, "Tex");				//テクスチャ
//	m_hmWVP = pEffect->GetParameterByName(NULL, "mWVP");				//ローカル-射影変換行列
//	m_hmWIT = pEffect->GetParameterByName(NULL, "mWIT");				//ローカル-ワールド変換行列
//	m_hvLightDir = pEffect->GetParameterByName(NULL, "vLightDir");		//ライトの方向
//	m_hvCol = pEffect->GetParameterByName(NULL, "vColor");				//頂点カラー
//	m_hvEyePos = pEffect->GetParameterByName(NULL, "vEyePos");
//	m_pos = pos;
//
//	return S_OK;
//}
//
////=========================================
////オブジェクトの更新処理
////=========================================
//void CModel::Update()
//{
//}
//
////=========================================
////オブジェクトの終了処理
////=========================================
//void CModel::Uninit()
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
//}

////=========================================
////オブジェクトの描画処理
////=========================================
//void CModel::Draw()
//{
//	PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
//
//	// 計算用マトリックス
//	D3DXMATRIX mtxRot, mtxTrans, mtxScale;
//	D3DXMATRIX mtxParents;
//
//	// ライトを無効
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&m_mtx);
//
//	// 向きを反映
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
//	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxRot);
//
//	// 位置を反映
//	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);                // 行列移動関数
//	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxTrans);								// 行列掛け算関数
//
//	if (m_pParents != nullptr)
//	{
//		mtxParents = m_pParents->GetMtxWorld();
//		// 行列掛け算関数
//		D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxParents);
//	}
//	//else
//	//{
//	//	// ワールドマトリックスの設定
//	//	pDevice->GetTransform(D3DTS_WORLD, &mtxParents);
//	//}
//
//	// 描画の終了
//	pDevice->EndScene();
//}
//
//void CModel::Draw(D3DXMATRIX mtxParent)
//{
//	PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
//
//	D3DXMATRIX mtxRot, mtxTrans, mtxScale;
//
//	// 現在のマテリアル保存用
//	D3DMATERIAL9 matDef;
//	// マテリアルデータへのポインタ
//	D3DXMATERIAL *pMat;
//	D3DXVECTOR3 scale(5.0f, 5.0f, 5.0f);
//
//	extern D3DLIGHT9 g_light;	// カメラ情報
//
//	D3DXMATRIX m, mT, mR, mView, mProj;
//	D3DXVECTOR4 v, light_pos;
//
//
//	//---------------------------------------------------------
//	// 描画
//	//---------------------------------------------------------
//	if (SUCCEEDED(pDevice->BeginScene()))
//	{
//		CCamera *pCamera = CApplication::GetCamera();
//
//		D3DMATRIX viewMatrix = pCamera->GetView();
//		D3DMATRIX projMatrix = pCamera->GetProjection();
//
//		// ワールド行列
//		D3DXMatrixIdentity(&m_mtx);
//
//		//大きさの設定
//		D3DXMatrixScaling(&mtxScale, m_scale, m_scale, m_scale);   // 行列拡縮関数
//		D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxScale);          // 行列掛け算関数
//
//																// 向きの反映
//		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);		// 行列回転関数
//		D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxRot);		// 行列掛け算関数 
//
//															// クォータニオンの使用した姿勢の設定
//		D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);				// クオータニオンによる行列回転
//		D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxRot);    // 行列掛け算関数(第2引数×第3引数第を１引数に格納)
//
//														//位置を反映
//		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
//		D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxTrans);
//
//		// 行列掛け算関数
//		D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxParent);
//
//		//行列の設定
//		pDevice->SetTransform(D3DTS_WORLD, &m_mtx);
//		pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
//		pDevice->SetTransform(D3DTS_PROJECTION, &projMatrix);
//
//		if (pEffect != NULL)
//		{
//			//-------------------------------------------------
//			// シェーダの設定
//			//-------------------------------------------------
//			pEffect->SetTechnique(m_hTechnique);
//			pEffect->Begin(NULL, 0);
//			pEffect->BeginPass(0);
//			pDevice->SetFVF(FVF_VERTEX_3D);
//
//			//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
//			//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
//
//			D3DXMatrixTranslation(&m, 1.0f, 0.0f, 0.0f);
//
//			D3DXMatrixRotationY(&mR, 0.0f);
//			D3DXMatrixTranslation(&mT, 1.0f, 1.2f, 0.0f);
//
//			//位置を反映
//			D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
//
//			// ローカル-射影変換行列
//			D3DXMatrixInverse(&m, NULL, &m_mtx);
//			D3DXMatrixTranspose(&m, &m);
//			pEffect->SetMatrix(m_hmWIT, &m);
//
//			// ローカル-射影変換行列
//			m = m_mtx * viewMatrix * projMatrix;
//			pEffect->SetMatrix(m_hmWVP, &m);
//
//			// ライトの方向
//			light_pos = D3DXVECTOR4(g_light.Direction.x, g_light.Direction.y, g_light.Direction.z, 0);
//
//			D3DXMatrixInverse(&m, NULL, &m_mtx);
//			D3DXVec4Transform(&v, &light_pos, &m);
//
//			D3DXVec3Normalize((D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v);
//
//			//環境光の大きさ
//			v.w = -0.8f;
//			pEffect->SetVector(m_hvLightDir, &v);
//
//			// 視点
//			m = m_mtx *viewMatrix;
//			D3DXMatrixInverse(&m, NULL, &m);
//
//			//環境光
//			v = D3DXVECTOR4(0, 0, 0, 1);
//
//			//マテリアルデータのポインタを取得する
//			pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();
//
//			D3DMATERIAL9 *pMtrl = &pMat->MatD3D;
//
//			D3DXVec4Transform(&v, &v, &m);
//
//			//視点をシェーダーに渡す
//			pEffect->SetVector(m_hvEyePos, &v);
//
//			for (int nCntMat = 0; nCntMat < (int)m_nNumMatModel; nCntMat++)
//			{
//				//カラーの光に対する反射的なやつの設定(多分)
//				v = D3DXVECTOR4(
//					pMtrl->Diffuse.r,
//					pMtrl->Diffuse.g,
//					pMtrl->Diffuse.b,
//					1.0f);
//
//				// テクスチャの設定
//				pEffect->SetTexture(m_hTexture, pTex0);
//
//				pEffect->SetVector(m_hvCol, &v);
//
//				//モデルパーツの描画
//				m_pMeshModel->DrawSubset(nCntMat);
//				pMtrl++;
//			}
//
//			pEffect->EndPass();
//			pEffect->End();
//		}
//
//		// 描画の終了
//		pDevice->EndScene();
//
//		////*****************************************
//		//// 影の表示
//		////*****************************************
//		////影の処理
//		//DrawShadow();
//	}
//}
//
////=========================================
//// 座標の設定処理
//// 引数 : m_posに代入したい座標の値
////=========================================
//void CModel::SetPosition(const D3DXVECTOR3& pos)
//{ 
//	m_pos = pos;
//}
//
////=========================================
//// 回転の設定取得
//// 引数 : m_rotに代入したい角度の値
////=========================================
//void CModel::SetRot(const D3DXVECTOR3& rot)
//{
//	m_rot = rot;
//}
////=========================================
//// サイズの設定
//// 引数 : サイズの指定
////=========================================
//void CModel::SetScale(const float size)
//{
//	m_scale = size;
//}
//
////=========================================
//// モデルの設定
//// 引数 : ファイル名の入力
////=========================================
//void CModel::SetModel(const char* name)
//{
//	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
//
//	m_filename = name;
//
//	//Xファイルの読み込み
//	D3DXLoadMeshFromX(m_filename,
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
//// モデルの大きさの算出
////=========================================
//D3DXVECTOR3 CModel::GetModelSize()
//{
//	// デバイスへのポインタの取得
//	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
//
//		//Xファイルの読み込み
//		D3DXLoadMeshFromX(m_filename,
//			D3DXMESH_SYSTEMMEM,
//			pDevice,
//			NULL,
//			&m_pBuffMatModel,
//			NULL,
//			&m_nNumMatModel,
//			&m_pMeshModel);
//
//		// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
//		D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();
//
//		int nNumVtx;		// 頂点数
//		DWORD pSizeFVF;		// 頂点フォーマットのサイズ
//		BYTE *pVtxBuff;		// 頂点バッファのポインタ
//
//		// 頂点数の取得
//		nNumVtx = m_pMeshModel->GetNumVertices();
//
//		// 頂点フォーマットのサイズを取得
//		pSizeFVF = D3DXGetFVFVertexSize(m_pMeshModel->GetFVF());
//
//		// 頂点バッファのロック
//		m_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
//
//		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
//		{
//			// 頂点座標の代入
//			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
//
//			// 比較(最小値を求める)x
//			if (vtx.x < vtxMin.x)
//			{
//				vtxMin.x = vtx.x;
//			}
//			// 比較(最小値を求める)y
//			if (vtx.y < vtxMin.y)
//			{
//				vtxMin.y = vtx.y;
//			}
//			// 比較(最小値を求める)z
//			if (vtx.z < vtxMin.z)
//			{
//				vtxMin.z = vtx.z;
//			}
//
//			// 比較(最大値を求める)x
//			if (vtx.x > vtxMax.x)
//			{
//				vtxMax.x = vtx.x;
//			}
//			// 比較(最大値を求める)y
//			if (vtx.y > vtxMax.y)
//			{
//				vtxMax.y = vtx.y;
//			}
//			// 比較(最大値を求める)z
//			if (vtx.z > vtxMax.z)
//			{
//				vtxMax.z = vtx.z;
//			}
//
//			// 頂点フォーマットのサイズ分ポインタを進める
//			pVtxBuff += pSizeFVF;
//		}
//
//		// サイズの計算
//		m_size = vtxMax - vtxMin;
//
//		// 頂点バッファのアンロック
//		m_pMeshModel->UnlockVertexBuffer();
//
//		return m_size;
//}
//
////=========================================
////クォータニオンの設置
////=========================================
//void CModel::SetQuaternion(const D3DXQUATERNION quaternion)
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
//float CModel::NormalizRot(float fRot)
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
//float CModel::LimitedRot(float fRot, float fLimitRot)
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
//void CModel::DrawShadow()
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
//	D3DXMatrixMultiply(&mtxShadow, &m_mtx, &mtxShadow);
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