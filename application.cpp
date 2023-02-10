//=========================================
// 
// 	�}�l�[�W���\�̏���
// 	Author:�y���m���@
// 
//=========================================
#include "application.h"
#include "render.h"
#include "object2D.h"
#include "billboard.h"
#include "player3D.h"
#include "input.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "Object3D.h"
#include "meshfield.h"
#include "sphere.h"
#include "inputkeyboard.h"
#include "mouse.h"
#include "sound.h"
#include "mode.h"
#include "score.h"
#include "game.h"
#include "title.h"
#include "debug_proc.h"
#include "result.h"
#include "fade.h"
#include "model3D.h"
#include "texture3D.h"
#include "joypad.h"

CDebugProc *CApplication::m_pDebug = nullptr;
CRender *CApplication::m_pRender = nullptr;
CInputKeyboard *CApplication::m_pInputKeyboard = nullptr;
CPlayer *CApplication::m_pPlayer = nullptr;
CEnemy *CApplication::m_pEnemy = nullptr;
CTexture* CApplication::m_pTexture = nullptr;
CMouse* CApplication::m_pMouse = nullptr;
CMode* CApplication::m_pMode = nullptr;
CJoypad *CApplication::m_pJoy = {};	
CGame* CApplication::m_pGame = nullptr;
CTexture3D* CApplication::m_pTexture3D = nullptr;
CSound*	CApplication::m_pSound = nullptr;		//�T�E���h

CCamera* CApplication::m_pRader = nullptr;
CCamera* CApplication::m_pCamera = nullptr;

CSound *pSound = nullptr;
CApplication::MODE CApplication::m_NextMode = MODE_MAX;
CApplication::MODE CApplication::m_mode = MODE_MAX;

LPD3DXEFFECT CApplication::m_pEffect = nullptr;

//=========================================
// �R���X�g���N�^
//=========================================
CApplication::CApplication()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CApplication::~CApplication()
{
}

//=========================================
// ����������
//=========================================
HRESULT CApplication::Init(HINSTANCE hInstance,HWND hWnd)
{
	m_pRender = new CRender;
	m_pTexture = new CTexture;
	m_pCamera = new CCamera;
	m_pRader = new CCamera;
	m_pInputKeyboard = new CInputKeyboard;
	pSound = new CSound;
	m_pMouse = new CMouse;
	m_pDebug = new CDebugProc;
	m_pTexture3D = new CTexture3D;
	m_pJoy = new CJoypad;

	if (FAILED(m_pRender->Init(hWnd, true)))
	{// ���������������s�����ꍇ
		return -1;
	}

	// TEXTURE(3D)�̓ǂݍ���
	m_pTexture3D->Init();

	// ���C�g�̏���������
	InitLight();

	// Model3D�̓ǂݍ���
	CModel3D::InitModel();

	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ����������
	if (FAILED(m_pJoy->Init(1)))
	{
		return E_FAIL;
	}

	//----------------------------
	// �T�E���h�̐����Ə�����
	//----------------------------
	m_pSound = new CSound;
	m_pSound->Init(hWnd);

	// �J�����̏������ݒ�
	if (m_pCamera != nullptr)
	{
		m_pCamera->SetObjMode(CObject::NORMAL_MODE);
		m_pCamera->Init(D3DXVECTOR3(0.0f, 1000.0f, -150.0f));
		m_pCamera->SetViewSize(0, 0, SCREEN_WIDTH , SCREEN_HEIGHT);
	}

	if (m_pRader != nullptr)
	{
		// �J�����̏������ݒ�2
		m_pRader->SetObjMode(CObject::RADAR_MODE);
		m_pRader->Init(D3DXVECTOR3(0.0f, 5000.0f, -150.0f));
		m_pRader->SetViewSize(0, 0, SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f);
	}

	// �Q�[���X�^�[�g���̏����ݒ�
	// ���[�h�̐ݒ�
	SetNextMode(MODE_TITLE);

	// �f�o�b�N�̏���������
	m_pDebug->Init();

	//���[�h�̐ݒ�
	CFade::Create(m_NextMode);

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetRender()->GetDevice();

	D3DXCreateEffectFromFile(
		pDevice, "Effect.fx", NULL, NULL,
		0, NULL, &m_pEffect, nullptr);

	return S_OK;
}

//=========================================
// �X�V����
//=========================================
void CApplication::Update()
{
	// ���͏����̍X�V����
	m_pInputKeyboard->Update();

	// �}�E�X�����̍X�V����
	m_pMouse->Update();

	// ���[�h�̕ύX
	ChangeMode();

	// ���C�g�̍X�V����
	UpdateLight();

	// �J�����̍X�V����
	m_pCamera->Update();

	// ���[�_�[�̍X�V����
	m_pRader->Update();

	// �����_�[�̍X�V����
	m_pRender->Update();

	//// �W���C�p�b�h�̍X�V����
	//m_pJoy->Update();

	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
}

//=========================================
// �`�揈��
//=========================================
void CApplication::Draw()
{
	// �����_�[�̕`�揈��
	m_pRender->Draw();
}

//=========================================
// �I������
//=========================================
void CApplication::Uninit()
{
	// ���͏����̏I������
	m_pInputKeyboard->Uninit();

	// �}�E�X�����̏I������
	m_pMouse->Uninit();

	// �����_�[�̏I������
	m_pRender->Uninit();

	// �e�N�X�`���̏I������
	m_pTexture3D->Uninit();

	// ���f���̏I������
	CModel3D::UninitModel();

	// �e�N�X�`���̏I������
	m_pTexture->ReleaseAll();

	// ���C�g�̏I������
	UninitLight();

	// �J�����̏I������
	m_pCamera->Uninit();

	// ���[�_�[�̏I������
	m_pRader->Uninit();

	// �f�o�b�N�̏I������
	m_pDebug->Uninit();

	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	if (m_pRender != nullptr)
	{
		delete m_pRender;
		m_pRender = nullptr;
	}
	if (m_pTexture != nullptr)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	if (m_pCamera != nullptr)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	if (m_pRader != nullptr)
	{
		delete m_pRader;
		m_pRader = nullptr;
	}
	if (m_pInputKeyboard != nullptr)
	{
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}
	if (m_pJoy != nullptr)
	{// �I������
		m_pJoy->Uninit();

		delete m_pJoy;
		m_pJoy = nullptr;
	}
}

//=========================================
// �����_�[�̏��̎擾
//=========================================
CRender *CApplication::GetRender()
{
	return m_pRender;
}

//=========================================
// �v���C���[�̏��̎擾
//=========================================
CPlayer *CApplication::GetPlayer()
{
	return m_pPlayer;
}

//=========================================
// �v���C���[�̏��̎擾
//=========================================
CEnemy *CApplication::GetEnemy()
{
	return m_pEnemy;
}

//=========================================
// �e�N�X�`���̏��̎擾
//=========================================
CTexture* CApplication::GetTexture()
{
	return m_pTexture;
}

//=========================================
// �L�[�{�[�h�̏��̎擾
//=========================================
CInputKeyboard* CApplication::GetInputKeyboard()
{
	return m_pInputKeyboard;
}

//=========================================
// ���[���h���W���X�N���[�����W�ɕϊ�����֐�
//=========================================
D3DXVECTOR3 CApplication::WorldToScreen(D3DXVECTOR3 pos)
{
	// �r���[�s��Ǝˉe�s��̎擾
	D3DXMATRIX mtx;

	D3DXMatrixTranslation(&mtx, pos.x, pos.y, pos.z);

	CCamera *pCamera = GetCamera();

	D3DXMATRIX view = pCamera->GetView();
	D3DXMATRIX proj = pCamera->GetProjection();

	mtx = mtx * view * proj;

	D3DXVECTOR3 pOut(mtx._41, mtx._42, mtx._43);

	// z�Ŋ�����-1��1�͈̔͂Ɏ��߂�
	pOut.x /= pOut.z, pOut.y /= pOut.z/*, pOut.z /= pOut.z*/;

	// �r���[�|�[�g�s��i�X�N���[���s��j�̍쐬
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);

	viewport._11 = SCREEN_WIDTH / 2.0f;
	viewport._22 = -SCREEN_HEIGHT / 2.0f;
	viewport._41 = SCREEN_WIDTH / 2.0f;
	viewport._42 = SCREEN_HEIGHT / 2.0f;

	D3DXVec3TransformCoord(&pOut, &pOut, &viewport);

	return pOut;
}

//==================================================
// �Q�[�����[�h�̎��̃��[�h�̐ݒ�
//==================================================
void CApplication::SetNextMode(MODE mode)
{
	m_NextMode = mode;
	m_mode = mode;
}

//=========================================
// �Q�[�����[�h�̕ύX
//=========================================
void CApplication::ChangeMode()
{
	if (m_NextMode == MODE_MAX)
	{
		return;
	}

	// �I�u�W�F�N�g�S�Ă̏I������
	CObject::UninitAll();

	//���[�h�̐ݒ�
	CFade::Create(m_NextMode);

	switch (m_NextMode)
	{
	case MODE_TITLE:
		// �J�����̏���������
		m_pCamera->Init(D3DXVECTOR3(0.0f, 0.0f, -150.0f));

		m_pMode = new CTitle;
		break;

	case MODE_GAME:
		// �J�����̏���������
		m_pCamera->Init(D3DXVECTOR3(0.0f, 1000.0f, -150.0f));
		m_pRader->Init(D3DXVECTOR3(0.0f, 5000.0f, -150.0f));
		m_pMode = new CGame;
		break;

	case MODE_RESULT:
		// �J�����̏���������
		m_pCamera->Init(D3DXVECTOR3(0.0f, 1000.0f, -150.0f));
		m_pMode = new CResult;
		break;

	default:
		break;
	}

	// ���̃��[�h�̏�����
	m_NextMode = MODE_MAX;

	if (m_pMode != nullptr)
	{
		m_pMode->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}

