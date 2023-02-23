//=========================================
// game.cpp
// Author: 冨所知生
//=========================================
#include "application.h"
#include "result.h"
#include "player3D.h"
#include "input.h"
#include "fade.h"
#include "ranking.h"

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
	//// オブジェクトの生成
	//CObject2D *pObject = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 3);

	//// オブジェクトのサイズ設定
	//pObject->SetScale(D3DXVECTOR3((float)SCREEN_HEIGHT,(float)SCREEN_HEIGHT,0.0f));

	//// オブジェクトのテクスチャ設定
	//pObject->SetTexture(CTexture::TEXTURE_RESULT);

	//ランキング生成
	m_pRanking = CRanking::Create();

	return S_OK;
}
//=========================================
// 更新処理
//=========================================
void CResult::Update()
{
	CInput *pKeyboard = CInput::GetKey();

	// モード変更
	if (pKeyboard->Trigger(DIK_RETURN) || pKeyboard->Trigger(JOYPAD_A))
	{
		//モードの設定
		CFade::SetFade(CApplication::MODE_TITLE);
	}
	//---------------------
	// ランキングの更新
	//---------------------
	if (m_pRanking != nullptr)
	{//ランキングがnullじゃないなら 
	 //更新
		m_pRanking->Update();
	}

}

//=========================================
// 終了処理
//=========================================
void CResult::Uninit()
{
	//---------------------
	// ランキング終了
	//---------------------
	if (m_pRanking != nullptr)
	{
		//終了
		m_pRanking->Uninit();

		//消去
		delete m_pRanking;
		m_pRanking = nullptr;
	}

	Release();
}