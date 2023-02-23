//=========================================
// game.cpp
// Author: 冨所知生
//=========================================
#include "application.h"
#include "tutorial.h"
#include "player3D.h"
#include "input.h"
#include "fade.h"

//=========================================
// コンストラクタ
//=========================================
CTutorial::CTutorial()
{
}

//=========================================
// デストラクタ
//=========================================
CTutorial::~CTutorial()
{
}

//=========================================
// 初期化 
//=========================================
HRESULT CTutorial::Init(const D3DXVECTOR3 &pos)
{
	// オブジェクトの生成
	CObject2D *pObject = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0);

	// オブジェクトのサイズ設定
	pObject->SetScale(D3DXVECTOR3((float)SCREEN_HEIGHT, (float)SCREEN_HEIGHT * 0.5f, 0.0f));

	// オブジェクトのテクスチャ設定
	pObject->SetTexture(CTexture::TEXTURE_RANKING);

	return S_OK;
}
//=========================================
// 更新処理
//=========================================
void CTutorial::Update()
{
	CInput *pKeyboard = CInput::GetKey();

	// モード変更
	if (pKeyboard->Trigger(DIK_RETURN) || pKeyboard->Trigger(JOYPAD_A))
	{
		//モードの設定
		CFade::SetFade(CApplication::MODE_TITLE);
	}

}

//=========================================
// 終了処理
//=========================================
void CTutorial::Uninit()
{
	Release();
}