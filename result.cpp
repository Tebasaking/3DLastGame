//=========================================
// game.cpp
// Author: �y���m��
//=========================================
#include "application.h"
#include "result.h"
#include "player3D.h"
#include "inputkeyboard.h"
#include "fade.h"

//=========================================
// �R���X�g���N�^
//=========================================
CResult::CResult()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CResult::~CResult()
{
}

//=========================================
// ������ 
//=========================================
HRESULT CResult::Init(const D3DXVECTOR3 &pos)
{
	// �I�u�W�F�N�g�̐���
	CObject2D *pObject = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 3);

	// �I�u�W�F�N�g�̃T�C�Y�ݒ�
	pObject->SetScale(SCREEN_HEIGHT);

	// �I�u�W�F�N�g�̃e�N�X�`���ݒ�
	pObject->SetTexture(CTexture::TEXTURE_RESULT);

	return S_OK;
}
//=========================================
// �X�V����
//=========================================
void CResult::Update()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// ���[�h�ύX
	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		//���[�h�̐ݒ�
		CFade::SetFade(CApplication::MODE_TITLE);
	}
}