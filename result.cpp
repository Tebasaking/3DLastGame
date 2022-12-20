//=========================================
// game.cpp
// Author: 冨所知生
//=========================================
#include "application.h"
#include "result.h"
#include "player3D.h"
#include "inputkeyboard.h"
#include "fade.h"

//=========================================
// コンストラクタ
//=========================================
CResult::CResult()
{
}

//=========================================
// デストラクタ
//=========================================
CResult::~CResult()
{
}

//=========================================
// 初期化 
//=========================================
HRESULT CResult::Init(const D3DXVECTOR3 &pos)
{
	// オブジェクトの生成
	CObject2D *pObject = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 3);

	// オブジェクトのサイズ設定
	pObject->SetScale(SCREEN_HEIGHT);

	// オブジェクトのテクスチャ設定
	pObject->SetTexture(CTexture::TEXTURE_RESULT);

	return S_OK;
}
//=========================================
// 更新処理
//=========================================
void CResult::Update()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// モード変更
	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		//モードの設定
		CFade::SetFade(CApplication::MODE_TITLE);
	}
}