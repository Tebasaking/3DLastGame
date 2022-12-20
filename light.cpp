#include "main.h"
#include "light.h"
#include "inputkeyboard.h"
#include "application.h"

//グローバル変数宣言
D3DLIGHT9 g_light;		//ライト処理

//=========================================
//ライトの初期化処理
////=========================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	D3DXVECTOR3 vecDir; //ライトの方向ベクトル

	//ライトの構造体をクリアする
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	//ライトの種類を設定
	g_light.Type = D3DLIGHT_DIRECTIONAL;		//平行光源

	//ライトの拡散光を設定
	g_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_light.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	//ライトの方向を設定
	vecDir = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	//正規化する(大きさ1のベクトルにする)
	D3DXVec3Normalize(&vecDir, &vecDir);

	g_light.Direction = vecDir;

	//ライトを設定する
	pDevice->SetLight(0, &g_light);

	//ライトを有効化する
	pDevice->LightEnable(0, TRUE);
}

void UninitLight(void)
{

}

void UpdateLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	if (pKeyboard->GetPress(DIK_LEFT))
	{
		g_light.Direction.x += 0.1f;
	}
	if (pKeyboard->GetPress(DIK_RIGHT))
	{
		g_light.Direction.x -= 0.1f;
	}

	g_light.Direction.x = NormalizeLight(g_light.Direction.x);

	if (pKeyboard->GetPress(DIK_UP))
	{
		g_light.Direction.y -= 0.1f;
	}
	if (pKeyboard->GetPress(DIK_DOWN))
	{
		g_light.Direction.y += 0.1f;
	}

	g_light.Direction.y = NormalizeLight(g_light.Direction.y);

	if (pKeyboard->GetPress(DIK_F))
	{
		g_light.Direction.z -= 0.1f;
	}
	if (pKeyboard->GetPress(DIK_G))
	{
		g_light.Direction.z += 0.1f;
	}

	g_light.Direction.z = NormalizeLight(g_light.Direction.z);

	//ライトを設定する
	pDevice->SetLight(0, &g_light);

}

//=========================================
//ライトの制御
//=========================================
float NormalizeLight(float Direction)
{
	if (Direction > 1.0f)
	{
		Direction = 1.0f;
	}
	else if (Direction < -1.0f)
	{
		Direction = -1.0f;
	}

	return Direction;
}