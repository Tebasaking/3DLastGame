//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "application.h"
#include "texture.h"

#include <assert.h>
//==================================================
// 定義
//==================================================
const char* CTexture::s_FileName[] =
{// テクスチャのパス
	"Data\\TEXTURE\\UITEST.png",
	"Data\\TEXTURE\\bullet.png",	// 弾
	"Data\\TEXTURE\\number001.png",						// ナンバー
	"Data\\TEXTURE\\explosion000.png",
	"Data\\TEXTURE\\sky_down.jpg",
	"Data\\TEXTURE\\sea.jpg",
	"Data\\TEXTURE\\ground.jpg",
	"Data\\F16_Thuderbirds4.bmp",
	"Data\\TEXTURE\\targeting.png",
	"Data\\TEXTURE\\たいとる.jpg",
	"Data\\TEXTURE\\りざると.jpg",
	"Data\\TEXTURE\\radarmap.png",
	"Data\\TEXTURE\\Machine.png",
	"Data\\TEXTURE\\enemy_tank.png",
	"Data\\TEXTURE\\enemy_Fly.png",
	"Data\\TEXTURE\\arrow.png",
	"Data\\TEXTURE\\sea000.jpg",
	"Data\\TEXTURE\\effect000.png",
	"Data\\TEXTURE\\smoke.png",
	"Data\\TEXTURE\\TITLE_LOGO.png",
	"Data\\TEXTURE\\TITLE_START.png",
	"Data\\TEXTURE\\TITLE_TUTORIAL.png",
	"Data\\TEXTURE\\TITLE_END.png",
	"Data\\TEXTURE\\Explosion001.png",
	"Data\\TEXTURE\\UI_ALT_SPEED.png",
	"Data\\TEXTURE\\UI_WEAPON.png",
	"Data\\TEXTURE\\GAGE_BOX.png",
	"Data\\TEXTURE\\GAGE.png",
	"Data\\TEXTURE\\UI_MISSILE_ALERT.png",
	"Data\\TEXTURE\\Ranking001.png",
	"Data\\TEXTURE\\GAME_OVER.png",
	"Data\\TEXTURE\\TIME_OVER.png",
	"Data\\TEXTURE\\tutorial_01.png",
	"Data\\TEXTURE\\tutorial_02.png",
	"Data\\TEXTURE\\tutorial_03.png",
	"Data\\TEXTURE\\STOP_PLAYER.png",
	"Data\\TEXTURE\\missile.png",
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CTexture::CTexture() : s_pTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// 全ての読み込み
//--------------------------------------------------
void CTexture::LoadAll()
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// テクスチャの読み込みがされている
			continue;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void CTexture::Load(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// テクスチャの読み込みがされている
		return;
	}

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		s_FileName[inTexture],
		&s_pTexture[inTexture]);
}

//--------------------------------------------------
// 全ての解放
//--------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// テクスチャの解放
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//--------------------------------------------------
// 解放
//--------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != NULL)
	{// テクスチャの解放
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = NULL;
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == TEXTURE_NONE)
	{// テクスチャを使用しない
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	// 読み込み
	Load(inTexture);

	return s_pTexture[inTexture];
}
