//=========================================
//
//	�v���C���[�̏���
//	Author:�y���m��
//
//=========================================
#include "player3D.h"
#include "player_manager.h"
#include "application.h"
#include "inputkeyboard.h"

CPlayerManager::PLAYER_MODE CPlayerManager::m_mode = MODE_FLY;
CPlayer3D *CPlayerManager::m_pFly = nullptr;
CPlayer3D *CPlayerManager::m_pRobot = nullptr;
//=========================================
//�R���X�g���N�^
//=========================================
CPlayerManager::CPlayerManager()
{
	m_mode = MODE_FLY;
}

//=========================================
//�f�X�g���N�^
//=========================================
CPlayerManager::~CPlayerManager()
{
}

//========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CPlayerManager::Init(const D3DXVECTOR3 &pos)
{
	m_pFly = new CPlayer3D;
	m_pRobot = new CPlayer3D;

	// ����������
	m_pFly->Init(pos); 
	m_pRobot->Init(pos);

	// ���[�h�̐ݒ�
	m_pFly->SetMode((CPlayer3D::PLAYER_MODE)MODE_FLY);
	m_pRobot->SetMode((CPlayer3D::PLAYER_MODE)MODE_ROBOT);

	// ���[�V�����̐ݒ�
	m_pFly->SetMotion("data/MOTION/fly_motion.txt");
	m_pRobot->SetMotion("data/MOTION/motion.txt");

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CPlayerManager::Update()
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	
	// �G���^�[�L�[�������ꂽ
	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		// m_mode��FLY�̎���ROBOT�ɁAROBOT�̎���FLY�ɂ���
		m_mode = (m_mode == MODE_FLY) ? MODE_ROBOT : MODE_FLY;
	}
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CPlayerManager::Uninit()
{
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CPlayerManager::Draw()
{
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CPlayerManager* CPlayerManager::Create(const D3DXVECTOR3 &pos)
{
	CPlayerManager* pPlayerManager = nullptr;

	pPlayerManager = new CPlayerManager;

	if (pPlayerManager != nullptr)
	{
		pPlayerManager->Init(pos);
	}

	return pPlayerManager;
}