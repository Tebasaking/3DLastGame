//=========================================
//
//	レンダー(ポリゴンの生成)処理
//	Author:冨所知生
//
//=========================================
#include <stdio.h>
#include "render.h"
#include "object.h"
#include "camera.h"
#include "application.h"
#include "debug.h"
#include "input.h"
#include "inputkeyboard.h"
#include "model3D.h"

#define POLIGON_MAX (2)

//=========================================
//コンストラクタ
//=========================================
CRender::CRender()
{

}

//=========================================
//デストラクタ
//=========================================
CRender::~CRender()
{

}

//=========================================
//レンダーの初期化処理
//=========================================
HRESULT CRender::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	m_Wire = false;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if ((FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画と頂点処理をCPUで行なう
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))))
	{
		// 生成失敗
		return E_FAIL;
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// アルファブレンドの設定
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// アルファブレンドの設定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// アルファブレンドの設定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャの大きさを変えても綺麗に貼る
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャの大きさを変えても綺麗に貼る
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャを繰り返して貼る
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャを繰り返して貼る

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// ポリゴンとテクスチャのアルファ値を混ぜる。
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// ポリゴンとテクスチャのアルファ値を混ぜる。テクスチャ指定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ポリゴンとテクスチャのアルファ値を混ぜる。ポリゴン指定

//#ifdef _DEBUG
//	// デバッグ情報表示用フォントの生成
//	D3DXCreateFont(m_pD3DDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
//		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
//#endif

	return S_OK;
}

//=========================================
//レンダーの更新処理
//=========================================
void CRender::Update()
{
	//更新処理
	CObject::UpdateAll();

	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	//ワイヤーフレーム
	if (pKeyboard->GetTrigger(DIK_F1))
	{
		m_Wire = !m_Wire;
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, m_Wire ? D3DFILL_WIREFRAME : D3DFILL_FORCE_DWORD);
	}
}

//=========================================
//レンダーの終了処理
//=========================================
void CRender::Uninit()
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif // _DEBUG

	// デバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=========================================
//レンダーの描画処理
//=========================================
void CRender::Draw()
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
#ifdef _DEBUG
		// FPS表示
		CDebugProc::Draw();
#endif // _DEBUG

		CCamera *m_pCamera = CApplication::GetCamera();

		//カメラの描画処理
		m_pCamera->Set();

		//描画処理
		CObject::DrawAll();

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void CRender::DrawFPS()
{
	//RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	//TCHAR str[256];

	//D3DXVECTOR3 TPos = CTarget::GetTargetPos();

	////wsprintf(str, _T("エネミーのスクリーン座標 : (%.0f , %f , %f )\n"), TPos.x, TPos.y, TPos.z);

	//// カメラの視点
	//sprintf(&str[0], "  エネミーのスクリーン座標 : (%.0f , %f , %f )\n", TPos.x, TPos.y, TPos.z);

	//// テキスト描画
	//m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
#endif // _DEBUG
