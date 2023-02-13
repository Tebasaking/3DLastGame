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
#include "inputkeyboard.h"
#include "fade.h"
#include "texture.h"
#include "meshfield.h"
#include "sphere.h"
#include "camera.h"

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
	// 海
	m_pMesh[0] = CMesh::Create(D3DXVECTOR3(0.0f, -350.0f, 0.0f), CMesh::TYPE_SEA);
	m_pMesh[2] = CMesh::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), CMesh::TYPE_WAVE);

	CSphere *pSphere = CSphere::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f));

	// 陸
	m_pMesh[1] = CMesh::Create(D3DXVECTOR3(0.0f, -400.0f, 0.0f), CMesh::TYPE_GROUND);

	CPlayer3D *pPlayer = CPlayer3D::Create(D3DXVECTOR3(0.0f, 1000.0f, 0.0f));

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