//=========================================
// game.cpp
// Author: �y���m��
//=========================================
#include "application.h"
#include "tutorial.h"
#include "player3D.h"
#include "input.h"
#include "fade.h"

//=========================================
// �R���X�g���N�^
//=========================================
CTutorial::CTutorial()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CTutorial::~CTutorial()
{
}

//=========================================
// ������ 
//=========================================
HRESULT CTutorial::Init(const D3DXVECTOR3 &pos)
{
	// �I�u�W�F�N�g�̐���
	CObject2D *pObject = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0);

	// �I�u�W�F�N�g�̃T�C�Y�ݒ�
	pObject->SetScale(D3DXVECTOR3((float)SCREEN_HEIGHT, (float)SCREEN_HEIGHT * 0.5f, 0.0f));

	// �I�u�W�F�N�g�̃e�N�X�`���ݒ�
	pObject->SetTexture(CTexture::TEXTURE_RANKING);

	return S_OK;
}
//=========================================
// �X�V����
//=========================================
void CTutorial::Update()
{
	CInput *pKeyboard = CInput::GetKey();

	// ���[�h�ύX
	if (pKeyboard->Trigger(DIK_RETURN) || pKeyboard->Trigger(JOYPAD_A))
	{
		//���[�h�̐ݒ�
		CFade::SetFade(CApplication::MODE_TITLE);
	}

}

//=========================================
// �I������
//=========================================
void CTutorial::Uninit()
{
	Release();
}