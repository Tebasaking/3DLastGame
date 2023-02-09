//==================================================
//
// title.cpp
// Author: �y���m��
//
//==================================================
#include "application.h"
#include "title.h"
#include "player3D.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "texture.h"

//==================================================
// �R���X�g���N�^
//==================================================
CTitle::CTitle()
{
}

//==================================================
// �f�X�g���N�^
//==================================================
CTitle::~CTitle()
{
}

//=========================================
// ������ 
//=========================================
HRESULT CTitle::Init(const D3DXVECTOR3 &pos)
{
	// �I�u�W�F�N�g�̐���
	CObject2D *pObject = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 3);

	// �I�u�W�F�N�g�̃T�C�Y�ݒ�
	pObject->SetScale(D3DXVECTOR3((float)SCREEN_HEIGHT, (float)SCREEN_HEIGHT,0.0f));
	pObject->SetColor(D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f));

	// �I�u�W�F�N�g�̃e�N�X�`���ݒ�
	pObject->SetTexture(CTexture::TEXTURE_TITLE);

	return S_OK;
}

//=========================================
// �X�V����
//=========================================
void CTitle::Update()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// ���[�h�ύX
	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		//���[�h�̐ݒ�
		CFade::SetFade(CApplication::MODE_GAME);
	}
}