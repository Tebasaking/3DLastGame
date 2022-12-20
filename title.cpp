//==================================================
//
// title.cpp
// Author: 冨所知生
//
//==================================================
#include "application.h"
#include "title.h"
#include "player3D.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "texture.h"

//==================================================
// コンストラクタ
//==================================================
CTitle::CTitle()
{
}

//==================================================
// デストラクタ
//==================================================
CTitle::~CTitle()
{
}

//=========================================
// 初期化 
//=========================================
HRESULT CTitle::Init(const D3DXVECTOR3 &pos)
{
	// オブジェクトの生成
	CObject2D *pObject = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 3);

	// オブジェクトのサイズ設定
	pObject->SetScale(SCREEN_HEIGHT);

	// オブジェクトのテクスチャ設定
	pObject->SetTexture(CTexture::TEXTURE_TITLE);

	return S_OK;
}

//=========================================
// 更新処理
//=========================================
void CTitle::Update()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// モード変更
	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		//モードの設定
		CFade::SetFade(CApplication::MODE_GAME);
	}
}