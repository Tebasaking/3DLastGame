//=========================================
// game.cpp
// Author: 冨所知生
//=========================================
#include "application.h"
#include "game_over.h"
#include "player3D.h"
#include "input.h"
#include "fade.h"
#include "ranking.h"
#include "sound.h"

//=========================================
// コンストラクタ
//=========================================
CGameOver::CGameOver()
{
}

//=========================================
// デストラクタ
//=========================================
CGameOver::~CGameOver()
{
}

//=========================================
// 初期化 
//=========================================
HRESULT CGameOver::Init(const D3DXVECTOR3 &pos)
{
	// オブジェクトの生成
	CObject2D *pObject = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0);

	// オブジェクトのサイズ設定
	pObject->SetScale(D3DXVECTOR3((float)SCREEN_HEIGHT, (float)SCREEN_HEIGHT * 0.5f, 0.0f));

	// オブジェクトのテクスチャ設定
	pObject->SetTexture(CTexture::TEXTURE_GAME_OVER);

	//サウンド生成
	CSound::PlaySound(CSound::SOUND_LABEL_BGM_LOSE);

	return S_OK;
}

//=========================================
// 更新処理
//=========================================
void CGameOver::Update()
{
	CInput *pKeyboard = CInput::GetKey();

	// モード変更
	if (pKeyboard->Trigger(DIK_RETURN) || pKeyboard->Trigger(JOYPAD_A))
	{
		//モードの設定
		CFade::SetFade(CApplication::MODE_RESULT);
	}

}

//=========================================
// 終了処理
//=========================================
void CGameOver::Uninit()
{
	// BGMの終了処理
	CSound::StopSound();

	// 解放処理
	Release();
}