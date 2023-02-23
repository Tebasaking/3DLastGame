//=========================================
//
//	main.h
//	Author:冨所知生
//
//=========================================
#ifndef _MAIN_H_		//このマクロ定義がされてなかったら
#define _MAIN_H_		//２重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <assert.h>
#include <Windows.h>
#include <tchar.h> // _T
#include <d3dx9.h>


#ifdef _DEBUG
#define CHECK_MEM_LEAK
#endif // _DEBUG:

//普段使用禁止
#ifdef CHECK_MEM_LEAK
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif // CHECK_MEM_LEAK


//前方宣言
class CRender;
class CObject;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点データ
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
};

struct VERTEX_3D
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR	col;	//頂点カラー
	D3DXVECTOR2 tex;	//texture座標
};

// スクリーンの幅
const int SCREEN_WIDTH = 1280;
// スクリーンの高さ
const int SCREEN_HEIGHT = 720;

// 頂点フォーマット
const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		//座標・法線・カラー

#endif