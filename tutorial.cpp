//==================================================
//
// title.cpp
// Author: 冨所知生
//
//==================================================
#include "application.h"
#include "tutorial.h"
#include "player3D.h"
#include "enemy.h"
#include "fade.h"
#include "texture.h"
#include "meshfield.h"
#include "sphere.h"
#include "camera.h"
#include "title_model.h"
#include "input.h"

//==================================================
// コンストラクタ
//==================================================
CTutorial::CTutorial()
{
}

//==================================================
// デストラクタ
//==================================================
CTutorial::~CTutorial()
{
}

//=========================================
// 初期化 
//=========================================
HRESULT CTutorial::Init(const D3DXVECTOR3 &pos)
{
	m_Select = 0;

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		pObject2D[nCnt] = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 4);
		pObject2D[nCnt]->SetScale(D3DXVECTOR3(0.0f,0.0f, 0.0f));
	}
	pObject2D[0]->SetScale(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	pObject2D[0]->SetTexture(CTexture::TEXTURE_TUTORIAL_000);
	pObject2D[1]->SetTexture(CTexture::TEXTURE_TUTORIAL_001);
	pObject2D[2]->SetTexture(CTexture::TEXTURE_TUTORIAL_002);

	return S_OK;
}

//=========================================
// 更新処理
//=========================================
void CTutorial::Update()
{
	CInput *pKeyboard = CInput::GetKey();

	float X = SCREEN_WIDTH * 0.5f;
	float Y = SCREEN_HEIGHT * 0.5f;


	if (pKeyboard->Trigger(JOYPAD_A) || pKeyboard->Trigger(DIK_RETURN))
	{
		pObject2D[m_Select]->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_Select++;
		if (m_Select != 3)
		{
			pObject2D[m_Select]->SetScale(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		}
	}

	else if (m_Select == 3)
	{
		//モードの設定
		CFade::SetFade(CApplication::MODE_TITLE);

		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			pObject2D[nCnt]->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
}