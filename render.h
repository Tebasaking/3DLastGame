//=========================================
//
//	render.h
//	Author:冨所知生
//
//=========================================
#ifndef _RENDER_H_		//このマクロ定義がされてなかったら
#define _RENDER_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//=========================================
//クラス宣言
//=========================================
class CRender
{
public:
	CRender();			//コンストラクタ
	~CRender();			//デストラクタ

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();
	void DrawFPS();
	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; };

private:
	// Direct3Dオブジェクト
	LPDIRECT3D9 m_pD3D = nullptr;
	// Deviceオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice = nullptr;
	//頂点バッファの設定
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuffPolygon = nullptr;
	//回転
	D3DXVECTOR3 m_rot = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	bool m_bCheck = false;
	bool m_Wire = false;
	float m_fWidth = 250.0f, fHeight = 250.0f;

	D3DXVECTOR3 m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0);

#ifdef _DEBUG
	// フォント
	LPD3DXFONT m_pFont = nullptr;
#endif // _DEBUG

#ifdef _DEBUG
	// FPSカウンタ
	int m_nCountFPS;
#endif // _DEBUG
};

#endif