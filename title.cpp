//==================================================
//
// title.cpp
// Author: 冨所知生
//
//==================================================
#include "application.h"
#include "title.h"
#include "player3D.h"
#include "enemy.h"
#include "fade.h"
#include "texture.h"
#include "meshfield.h"
#include "sphere.h"
#include "camera.h"
#include "title_model.h"
#include "input.h"

CMesh* CTitle::m_pMesh[3] = {};
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
	m_Select = 1;

	// 海
	m_pMesh[0] = CMesh::Create(D3DXVECTOR3(0.0f, -350.0f, 0.0f), CMesh::TYPE_SEA);
	m_pMesh[2] = CMesh::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), CMesh::TYPE_WAVE);

	CSphere *pSphere = CSphere::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f));

	// 陸
	m_pMesh[1] = CMesh::Create(D3DXVECTOR3(0.0f, -400.0f, 0.0f), CMesh::TYPE_GROUND);

	//CEnemy* pEnemy = CEnemy::Create(D3DXVECTOR3(-1300.0f,250.0f,-800.0f),CEnemy::ENEMY_FLY,0);
	CTitleModel *pTitleModel = CTitleModel::Create(D3DXVECTOR3(-500.0f, 250.0f, -500.0f), CTitleModel::ENEMY_FLY);
	CTitleModel *pTitleModel2 = CTitleModel::Create(D3DXVECTOR3(-300.0f, 50.0f, 500.0f), CTitleModel::ENEMY_FLY);
	CTitleModel *pTitleModel3 = CTitleModel::Create(D3DXVECTOR3(-100.0f, -10.0f,-350.0f), CTitleModel::ENEMY_GROUND);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pObject2D[nCnt] = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 4);
		pObject2D[nCnt]->SetScale(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	}

	pObject2D[0]->SetTexture(CTexture::TEXTURE_TITLE_LOGO);
	pObject2D[1]->SetTexture(CTexture::TEXTURE_TITLE_START);
	pObject2D[2]->SetTexture(CTexture::TEXTURE_TITLE_TUTORIAL);
	pObject2D[3]->SetTexture(CTexture::TEXTURE_TITLE_END);

	return S_OK;
}

//=========================================
// 更新処理
//=========================================
void CTitle::Update()
{
	CInput *pKeyboard = CInput::GetKey();

	float X = SCREEN_WIDTH * 0.5f;
	float Y = SCREEN_HEIGHT * 0.5f;

	if (pKeyboard->Trigger(DIK_UP) || pKeyboard->Trigger(JOYPAD_UP))
	{
		pObject2D[m_Select]->SetScale(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		m_Select--;
	}
	if (pKeyboard->Trigger(DIK_DOWN) || pKeyboard->Trigger(JOYPAD_DOWN))
	{
		pObject2D[m_Select]->SetScale(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		m_Select++;
	}

	if (m_Select == 0)
	{
		m_Select = 3;
	}
	else if (m_Select == 4)
	{
		m_Select = 1;
	}

	if (!m_bSelect)
	{
		m_SelectCnt++;
	}
	else
	{
		m_SelectCnt--;
	}

	pObject2D[m_Select]->SetScale(D3DXVECTOR3(X + m_SelectCnt,Y + m_SelectCnt,0.0f));

	if (m_SelectCnt == 30)
	{
		m_bSelect = true;
	}
	else if (m_SelectCnt == 0)
	{
		m_bSelect = false;
	}

	// モード変更
	if (pKeyboard->Trigger(DIK_RETURN) || pKeyboard->Trigger(JOYPAD_A))
	{
		//モードの設定
		CFade::SetFade(CApplication::MODE_GAME);
	}
}