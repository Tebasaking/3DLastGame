//=============================================================================
//
// 3Dモデルクラス(model3D.h)
// Author : 冨所知生
// 概要 : 3Dモデル生成を行う
//
//=============================================================================
#include <stdio.h>
#include <assert.h>

#include "model3D.h"
#include "render.h"
#include "application.h"
#include "game.h"
#include "texture3D.h"
#include "light.h"
#include "camera.h"
#include "camera_player.h"
#include "camera_radar.h"
#include "camera_title.h"

//--------------------------------------------------------------------
// 静的メンバ変数定義
//--------------------------------------------------------------------
CModel3D::MODEL_MATERIAL *CModel3D::m_material = nullptr;		// マテリアル情報
int CModel3D::m_nMaxModel = 0;									// モデル数		

//=============================================================================
// コンストラクタ
//=============================================================================
CModel3D::CModel3D()
{
	m_pParent = nullptr;										// 親モデルの情報
	m_mtxWorld = {};											// ワールドマトリックス
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 大きさ
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				// カラー
	m_quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);		// クォータニオン
	m_nModelID = -1;											// モデルID
	m_bColor = false;											// カラーを使用する
	m_bShadow = true;											// 影の使用状況
	m_bLighting = true;											// ライトの使用状況

	pEffect = NULL;
	m_hmWVP = NULL;
	m_hmWIT = NULL;
	m_hvLightDir = NULL;
	m_hvCol = NULL;
	m_hvEyePos = NULL;
	m_hTechnique = NULL;
	m_hTexture = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModel3D::~CModel3D()
{

}
//=============================================================================
// インスタンス生成
//=============================================================================
CModel3D * CModel3D::Create()
{
	// オブジェクトインスタンス
	CModel3D *pModel3D = nullptr;

	// メモリの解放
	pModel3D = new CModel3D;

	// メモリの確保ができなかった
	assert(pModel3D != nullptr);

	// 数値の初期化
	pModel3D->Init();

	// インスタンスを返す
	return pModel3D;
}

//=============================================================================
// モデル情報の初期化
//=============================================================================
void CModel3D::InitModel()
{
	// レンダラーのゲット
	CRender *pRender = CApplication::GetRender();

	// テクスチャポインタの取得
	CTexture3D *pTexture = CApplication::GetTexture3D();

	// テクスチャ情報の取得
	CTexture3D::TEXTURE *pTextureData = pTexture->GetTextureData();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	// ファイル読み込み
	LoadModel("Data/FILE/data.txt");

	// テクスチャの使用数のゲット
	int nMaxTex = pTexture->GetMaxTexture();

	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{// Xファイルの読み込み
		D3DXLoadMeshFromX(&m_material[nCntModel].aFileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_material[nCntModel].pBuffer,
			NULL,
			&m_material[nCntModel].nNumMat,
			&m_material[nCntModel].pMesh);

		// マテリアルのテクスチャ情報のメモリ確保
		m_material[nCntModel].pNumTex = new int[m_material[nCntModel].nNumMat];
		assert(m_material[nCntModel].pNumTex != nullptr);

		// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_material[nCntModel].pBuffer->GetBufferPointer();

		memset(&m_material[nCntModel].pTexture[0], 0,sizeof(LPDIRECT3DTEXTURE9) * MAX_MATERIAL_TEXTURE_NUM);

		// 各メッシュのマテリアル情報を取得する 
		for (int nCntMat = 0; nCntMat < (int)m_material[nCntModel].nNumMat; nCntMat++)
		{
			// マテリアルのテクスチャ情報の初期化
			m_material[nCntModel].pNumTex[nCntMat] = -1;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				D3DXCreateTextureFromFile(pRender->GetDevice(),
					pMat[nCntMat].pTextureFilename,
					&m_material[nCntModel].pTexture[nCntMat]);
			}
		}

		// 頂点座標の最小値・最大値の算出
		int		nNumVtx;	// 頂点数
		DWORD	sizeFVF;	// 頂点フォーマットのサイズ
		BYTE	*pVtxBuff;	// 頂点バッファへのポインタ

		// 頂点数の取得
		nNumVtx = m_material[nCntModel].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		sizeFVF = D3DXGetFVFVertexSize(m_material[nCntModel].pMesh->GetFVF());

		// 頂点バッファのロック
		m_material[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		// 最も大きな頂点
		D3DXVECTOR3 vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		D3DXVECTOR3 vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x < vtxMin.x)
			{// 比較対象が現在の頂点座標(X)の最小値より小さい
				vtxMin.x = vtx.x;
			}
			if (vtx.y < vtxMin.y)
			{// 比較対象が現在の頂点座標(Y)の最小値より小さい
				vtxMin.y = vtx.y;
			}
			if (vtx.z < vtxMin.z)
			{// 比較対象が現在の頂点座標(Z)の最小値より小さい
				vtxMin.z = vtx.z;
			}

			if (vtx.x > vtxMax.x)
			{// 比較対象が現在の頂点座標(X)の最大値より大きい
				vtxMax.x = vtx.x;
			}
			if (vtx.y > vtxMax.y)
			{// 比較対象が現在の頂点座標(Y)の最大値より大きい
				vtxMax.y = vtx.y;
			}
			if (vtx.z > vtxMax.z)
			{// 比較対象が現在の頂点座標(Z)の最大値より大きい
				vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		// 頂点バッファのアンロック
		m_material[nCntModel].pMesh->UnlockVertexBuffer();

		// 大きさの設定
		m_material[nCntModel].size = vtxMax - vtxMin;
	}
}

//=============================================================================
// モデル情報の終了
//=============================================================================
void CModel3D::UninitModel()
{
	for (int nCnt = 0; nCnt < m_nMaxModel; nCnt++)
	{
		// メッシュの破棄
		if (m_material[nCnt].pMesh != nullptr)
		{
			m_material[nCnt].pMesh->Release();
			m_material[nCnt].pMesh = nullptr;
		}

		// マテリアルの破棄
		if (m_material[nCnt].pBuffer != nullptr)
		{
			m_material[nCnt].pBuffer->Release();
			m_material[nCnt].pBuffer = nullptr;
		}

		// 各メッシュのマテリアル情報を取得する 
		for (int nCntMat = 0; nCntMat < (int)m_material[nCnt].nNumMat; nCntMat++)
		{
			if (m_material[nCnt].pTexture[nCntMat] != nullptr)
			{
				m_material[nCnt].pTexture[nCntMat]->Release();
				m_material[nCnt].pTexture[nCntMat] = nullptr;
			}
		}

		// メモリの解放
		delete[] m_material[nCnt].pNumTex;
		m_material[nCnt].pNumTex = nullptr;
	}

	// メモリの解放
	delete[] m_material;
	m_material = nullptr;
}

//=============================================================================
// Xファイルの読み込み
//=============================================================================
void CModel3D::LoadModel(const char *pFileName)
{
	// 変数宣言
	char aStr[128];
	int nCntModel = 0;

	// ファイルの読み込み
	FILE *pFile = fopen(pFileName, "r");

	if (pFile != nullptr)
	{
		while (fscanf(pFile, "%s", &aStr[0]) != EOF)
		{// "EOF"を読み込むまで 
			if (strncmp(&aStr[0], "#", 1) == 0)
			{// 一列読み込む
				fgets(&aStr[0], sizeof(aStr), pFile);
			}

			if (strstr(&aStr[0], "MAX_TYPE") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nMaxModel);
				m_material = new CModel3D::MODEL_MATERIAL[m_nMaxModel];
				assert(m_material != nullptr);
			}

			if (strstr(&aStr[0], "MODEL_FILENAME") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%s", &m_material[nCntModel].aFileName[0]);
				nCntModel++;
			}
		}
	}
	else
	{
		assert(false);
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel3D::Init()
{
	// メンバ変数の初期化
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);					// 大きさ
	m_nModelID = -1;										// モデルID

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// エフェクトファイルの取得
	pEffect = CApplication::GetShader();

	m_hTechnique = pEffect->GetTechniqueByName("Diffuse");				//エフェクト
	m_hTexture = pEffect->GetParameterByName(NULL, "Tex");				//テクスチャ
	m_hmWVP = pEffect->GetParameterByName(NULL, "mWVP");				//ローカル-射影変換行列
	m_hmWIT = pEffect->GetParameterByName(NULL, "mWIT");				//ローカル-ワールド変換行列
	m_hvLightDir = pEffect->GetParameterByName(NULL, "vLightDir");		//ライトの方向
	m_hvCol = pEffect->GetParameterByName(NULL, "vColor");				//頂点カラー
	m_hvEyePos = pEffect->GetParameterByName(NULL, "vEyePos");

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CModel3D::Uninit()
{

}

//=============================================================================
// 更新
//=============================================================================
void CModel3D::Update()
{
	
}

//=============================================================================
// 描画
//=============================================================================
void CModel3D::Draw()
{
	if (m_nModelID >= 0
		&& m_nModelID < m_nMaxModel)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

		D3DMATRIX viewMatrix;
		D3DMATRIX projMatrix;

		pDevice->GetTransform(D3DTS_VIEW, &viewMatrix);
		pDevice->GetTransform(D3DTS_PROJECTION, &projMatrix);

		// 親のワールドマトリックス
		D3DXMATRIX mtxParent = {};

		// 計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans, mtxScaling;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);											// 行列初期化関数

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);			// 行列回転関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);						// 行列掛け算関数 

		// クォータニオンの反映
		D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);						// クオータニオンによる行列回転
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);				// 行列移動関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);					// 行列掛け算関数

		if (m_pParent != nullptr)
		{	// 親のワールド座標を取得
			mtxParent = m_pParent->GetMtxWorld();

			// 行列掛け算関数
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
		pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
		pDevice->SetTransform(D3DTS_PROJECTION, &projMatrix);

		// マテリアル描画
		DrawMaterial();
	}
}

//=============================================================================
// 描画
//=============================================================================
void CModel3D::Draw(D3DXMATRIX mtxParent)
{
	if (m_nModelID >= 0
		&& m_nModelID < m_nMaxModel)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

		D3DMATRIX viewMatrix;
		D3DMATRIX projMatrix;

		pDevice->GetTransform(D3DTS_VIEW, &viewMatrix);
		pDevice->GetTransform(D3DTS_PROJECTION, &projMatrix);

		// 計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans, mtxScaling;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);											// 行列初期化関数

		// サイズの反映
		D3DXMatrixScaling(&mtxScaling, m_size.x, m_size.y, m_size.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);					// 行列掛け算関数

		// クォータニオンの反映
		D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);						// クオータニオンによる行列回転
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);			// 行列回転関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);						// 行列掛け算関数 

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);				// 行列移動関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);					// 行列掛け算関数

		// 行列掛け算関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		//if (m_bShadow)
		//{// 影の生成
		//	Shadow();
		//}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
		pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
		pDevice->SetTransform(D3DTS_PROJECTION, &projMatrix);

		// マテリアル描画
		DrawMaterial();
	}
}

//=============================================================================
// マテリアル描画
//=============================================================================
void CModel3D::DrawMaterial()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;
	// マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	CCamera *pCamera = nullptr;

	switch (CApplication::GetRender()->GetCheck())
	{
	case false:
		switch (CApplication::GetMode())
		{
		case CApplication::MODE_TITLE:
			pCamera = CApplication::GetTitleCamera();
			break;

		case CApplication::MODE_GAME:
			pCamera = CApplication::GetCamera();
			break;

		case CApplication::MODE_RESULT:
			pCamera = CApplication::GetTitleCamera();
			break;
		}
		break;
	case true:
		pCamera = CApplication::GetRader();
		break;
	}

	D3DMATRIX viewMatrix = pCamera->GetView();
	D3DMATRIX projMatrix = pCamera->GetProjection();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	extern D3DLIGHT9 g_light;	// カメラ情報

	D3DXMATRIX m, mT, mR, mView, mProj;
	D3DXVECTOR4 v, light_pos;

	//==================================================================================
	if (pEffect != NULL)
	{
		// テクスチャポインタの取得
		CTexture3D *pTexture = CApplication::GetTexture3D();

		//-------------------------------------------------
		// シェーダの設定
		//-------------------------------------------------
		pEffect->SetTechnique(m_hTechnique);
		pEffect->Begin(NULL, 0);
		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

		D3DXMatrixTranslation(&m, 1.0f, 0.0f, 0.0f);

		D3DXMatrixRotationY(&mR, 0.0f);
		D3DXMatrixTranslation(&mT, 1.0f, 1.2f, 0.0f);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		// ローカル-射影変換行列
		D3DXMatrixInverse(&m, NULL, &m_mtxWorld);
		D3DXMatrixTranspose(&m, &m);
		pEffect->SetMatrix(m_hmWIT, &m);

		// ローカル-射影変換行列
		m = m_mtxWorld * viewMatrix * projMatrix;
		pEffect->SetMatrix(m_hmWVP, &m);

		// ライトの方向
		light_pos = D3DXVECTOR4(g_light.Direction.x, g_light.Direction.y, g_light.Direction.z, 0);

		D3DXMatrixInverse(&m, NULL, &m_mtxWorld);
		D3DXVec4Transform(&v, &-light_pos, &m);

		D3DXVec3Normalize((D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v);

		//環境光の大きさ
		v.w = -0.8f;
		pEffect->SetVector(m_hvLightDir, &v);

		// 視点
		m = m_mtxWorld *viewMatrix;
		D3DXMatrixInverse(&m, NULL, &m);

		//環境光
		v = D3DXVECTOR4(0, 0, 0, 1);

		//マテリアルデータのポインタを取得する
		pMat = (D3DXMATERIAL*)m_material[m_nModelID].pBuffer->GetBufferPointer();

		D3DMATERIAL9 *pMtrl = &pMat->MatD3D;

		D3DXVec4Transform(&v, &v, &m);

		//視点をシェーダーに渡す
		pEffect->SetVector(m_hvEyePos, &v);

		for (int nCntMat = 0; nCntMat < (int)m_material[m_nModelID].nNumMat; nCntMat++)
		{
			// モデルの色の設定♪ 
			v = D3DXVECTOR4(
				1.0f,
				1.0f,
				1.0f,
				1.0f);

			if (m_material[m_nModelID].pTexture[nCntMat] != nullptr)
			{// テクスチャの適応
				pTex0 = m_material[m_nModelID].pTexture[nCntMat];
			}

			pEffect->SetVector(m_hvCol, &v);

			// テクスチャの設定
			pEffect->SetTexture(m_hTexture, pTex0);

			pEffect->BeginPass(0);

			//モデルパーツの描画
			m_material[m_nModelID].pMesh->DrawSubset(nCntMat);
			pMtrl++;

			pEffect->EndPass();
		}

		pEffect->End();
	}

	//=========================================

	// 現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// ライトを有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//==================================================================================
}

//=============================================================================
// 位置のセッター
//=============================================================================
void CModel3D::SetPos(const D3DXVECTOR3 &pos)
{
	// 位置の設定
	m_pos = pos;
}

//=============================================================================
// 向きのセッター
//=============================================================================
void CModel3D::SetRot(const D3DXVECTOR3 &rot)
{
	// 位置の設定
	m_rot = rot;
}

//=============================================================================
// 大きさのセッター
//=============================================================================
void CModel3D::SetSize(const D3DXVECTOR3 & size)
{
	// 大きさの設定
	m_size = size;
}

//=============================================================================
// カラーの設定
//=============================================================================
void CModel3D::SetColor(const D3DXCOLOR color)
{
	// 色の設定
	m_color = color;
	m_bColor = true;
}

//=============================================================================
// 影の生成
//=============================================================================
void CModel3D::Shadow()
{
	// 変数宣言
	D3DXMATRIX		mtxShadow = {};													// シャドウマトリックス
	D3DXPLANE		planeField = {};												// 平面化用変数
	D3DXVECTOR4		vecLight = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);					// ライト方向の逆ベクトル
	D3DXVECTOR3		posShadow = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						// 影の位置
	D3DXVECTOR3		norShadow = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						// 影の法線
	extern D3DLIGHT9 g_light;	// カメラ情報

	D3DXVECTOR3		light = g_light.Direction;
	D3DXVECTOR3		lightVec = light * -1;		// ライト方向

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);	// 行列初期化関数

	// ライト方向の逆ベクトルの設定
	vecLight = D3DXVECTOR4(lightVec.x, lightVec.y, lightVec.z, 0.0f);

	// 平面化用変数の設定
	D3DXPlaneFromPointNormal(&planeField, &posShadow, &norShadow);

	// シャドウマトリックスの設定
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	// 行列掛け算関数
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// シャドウマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// 現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	if (m_material[m_nModelID].pBuffer != nullptr)
	{// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_material[m_nModelID].pBuffer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_material[m_nModelID].nNumMat; nCntMat++)
		{// マテリアル情報の設定
			D3DMATERIAL9  matD3D = pMat[nCntMat].MatD3D;

			// 引数を色に設定
			matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			// マテリアルの設定
			pDevice->SetMaterial(&matD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, nullptr);

			// モデルパーツの描画
			m_material[m_nModelID].pMesh->DrawSubset(nCntMat);
		}
	}

	// Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// 角度の正規化処理
// 引数 : 正規化したい角度
//=========================================
float CModel3D::NormalizeRot(float fRot)
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

//=========================================
// 角度の上限設定
// 引数 : 制限を設けたい角度、限界角度
//=========================================
float CModel3D::LimitedRot(float fRot, float fLimitRot)
{
	if (fRot < D3DX_PI * -fLimitRot)
	{
		fRot = D3DX_PI * -fLimitRot;
	}
	else if (fRot > D3DX_PI * fLimitRot)
	{
		fRot = D3DX_PI * fLimitRot;
	}
	return fRot;
}
