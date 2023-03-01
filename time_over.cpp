//=========================================
// game.cpp
// Author: 冨所知生
//=========================================
#include "application.h"
#include "time_over.h"
#include "player3D.h"
#include "input.h"
#include "fade.h"
#include "ranking.h"
#include "sound.h"

//=========================================
// コンストラクタ
//=========================================
CTimeOver::CTimeOver()
{
}

//=========================================
// デストラクタ
//=========================================
CTimeOver::~CTimeOver()
{
}

//=========================================
// 初期化 
//=========================================
HRESULT CTimeOver::Init(const D3DXVECTOR3 &pos)
{
	//サウンド生成
	CSound::PlaySound(CSound::SOUND_LABEL_BGM_LOSE);

	// オブジェクトの生成
	CObject2D *pObject = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0);

	// オブジェクトのサイズ設定
	pObject->SetScale(D3DXVECTOR3((float)SCREEN_HEIGHT, (float)SCREEN_HEIGHT * 0.5f, 0.0f));

	// オブジェクトのテクスチャ設定
	pObject->SetTexture(CTexture::TEXTURE_TIME_OVER);

	return S_OK;
}
//=========================================
// 更新処理
//=========================================
void CTimeOver::Update()
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
void CTimeOver::Uninit()
{
	Release();

	CSound::StopSound();
}