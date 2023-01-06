//=========================================
//
//	�v���C���[�̏���
//	Author:�y���m��
//
//=========================================
#include "player3D.h"
#include "camera.h"
#include "application.h"
#include "input.h"
#include "inputkeyboard.h"
#include "texture.h"
#include "billboard.h"
#include "Object2D.h"
#include "mouse.h"
#include "game.h"
#include "meshfield.h"
#include "bullet.h"
#include "model3D.h"
#include "player_manager.h"

int BulletDelay = 0;

//=========================================
//�R���X�g���N�^
//=========================================
CPlayer3D::CPlayer3D()
{
	SetObjectType(OBJECT_PLAYER);
	m_mode = MODE_MAX;
}

//=========================================
//�f�X�g���N�^
//=========================================
CPlayer3D::~CPlayer3D()
{
}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CPlayer3D::Init(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	m_apModel[0] = new CModel3D;

	m_apModel[0]->SetModelID(0);
	m_apModel[0]->Init();
	m_apModel[0]->SetPos(pos);

	//�g�嗦�̐ݒ�
	m_apModel[0]->SetSize(D3DXVECTOR3(1.0f,1.0f,1.0f));

	//�傫���̐ݒ�
	m_size = m_apModel[0]->GetSize();

	//=========================================
	// �l�^���f���̓ǂݍ���
	//=========================================
	m_pRobot = new CModel3D;

	m_apModel[0]->SetModelID(1);
	m_apModel[0]->Init();
	m_apModel[0]->SetPos(pos);

	//�g�嗦�̐ݒ�
	m_apModel[0]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//�傫���̐ݒ�
	m_size = m_pRobot->GetSize();

	SetPosition(pos);

	CMotionModel3D::Init(pos);

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CPlayer3D::Update()
{
	switch (m_mode)
	{
	case MODE_FLY:
		// ��s�`�Ԃ̍X�V����
		UpdateFly();
		break;

	case MODE_ROBOT:
		// �l�^�`�Ԃ̍X�V����
		UpdateRob();
		break;
	}
	// ���W�̐ݒ�
	m_apModel[0]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	// ���W�̐ݒ�
	m_pRobot->SetPos(m_pos);

	// ���W�̐ݒ�
	SetPosition(m_pos);

	// ��s���f���̍X�V����
	for (int nCnt = 0; nCnt < 1; nCnt++)
	{
		m_apModel[nCnt]->Update();
	}

	// ���{�b�g���f���̍X�V����
	m_pRobot->Update();

	// ���[�V�����̍X�V����
	CMotionModel3D::Update();
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CPlayer3D::Uninit()
{
	for (int nCnt = 0; nCnt < 1; nCnt++)
	{
		m_apModel[nCnt]->Uninit();
	}

	// ���{�b�g���f���̏I������
	m_pRobot->Uninit();

	// ���[�V�����̏I������
	CMotionModel3D::Uninit();

	//�I�u�W�F�N�g�̉������
	CObject::Release();

}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CPlayer3D::Draw()
{
	// �}�l�[�W���\�̃��[�h�ƈ�v������
	if (m_mode == (CPlayer3D::PLAYER_MODE)CPlayerManager::GetMode())
	{
		PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

		// �v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxRot, mtxTrans, mtxScale;

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_WorldMtx);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);                // �s��ړ��֐�
		D3DXMatrixMultiply(&m_WorldMtx, &m_WorldMtx, &mtxTrans);					// �s��|���Z�֐�

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_WorldMtx);

		switch (m_mode)
		{
		case MODE_FLY:
			// ��s�`�Ԃ̍X�V����
			m_apModel[0]->Draw();
			break;

		case MODE_ROBOT:
			// �l�^�`�Ԃ̍X�V����
			m_pRobot->Draw();
			break;
		}

		// ���[�V�����̕`�揈��
		CMotionModel3D::Draw();
	}
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CPlayer3D* CPlayer3D::Create(const D3DXVECTOR3 &pos)
{
	int nNumCreate = m_nNumAll;

	CPlayer3D* pCPlayer3D = nullptr;

	pCPlayer3D = new CPlayer3D;

	if (pCPlayer3D != nullptr)
	{
		pCPlayer3D->Init(pos);
	}

	return pCPlayer3D;
}

//=========================================
// ���b�N�I������
//=========================================
void CPlayer3D::LockOn()
{
}

//=========================================
// �v���C���[�̉�]����
// Author : �y���m��
// �T�v : �v���C���[�̉�]
//=========================================
void CPlayer3D::Rotate()
{
	//�J�������̎擾
	CCamera *pCamera = CApplication::GetCamera();

	m_quaternion = pCamera->GetQuaternion();

	m_apModel[0]->SetQuaternion(m_quaternion);
	m_pRobot->SetQuaternion(m_quaternion);
}

//=========================================
//	��s��Ԃ̃v���C���[�̏���
//	Author : �y���m��
//	�T�v : �v���C���[�̔�s���
//=========================================
void CPlayer3D::UpdateFly()
{
	// �ڕW��pos
	D3DXVECTOR3	m_posDest;
	//�J����
	CCamera *pCamera = CApplication::GetCamera();

	m_pos = m_apModel[0]->GetPos();				// ���W�̎擾
	m_posDest = pCamera->GetPosR();				// �J�����̍��W�̎擾

	m_posDest.y -= 20.0f;
	D3DXVECTOR3 m_posResult = m_posDest - m_pos;
	D3DXVECTOR3 posDiss = m_pos - pCamera->GetPosV();

	m_pos.x += m_posResult.x / 5;
	m_pos.z += m_posResult.z / 5;
	m_pos.y += m_posResult.y / 5;

	int MOVE_SPEED = 5.0f;

	// ��]
	Rotate();

	//���b�N�I������
	LockOn();

	// �p�����䏈��
	Attitude();

	// �e�̔��ˏ���
	Bullet(m_pos);

	// ���W�̐ݒ�
	m_apModel[0]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	// �O���E���h�̎擾
	CMesh *pGround = CGame::GetGround();

	if (pGround != nullptr)
	{
		// ���̓����蔻��
		bool bCollision = pGround->Collision(&m_pos);

		// �����蔻��̐ݒ�
		SetCollision(bCollision);
	}
}

//=========================================
// �l�^�`�Ԃ̍X�V����
//=========================================
void CPlayer3D::UpdateRob()
{
	// �v���C���[�̉�]����
	Rotate();

	// �ڕW��pos
	D3DXVECTOR3	m_posDest;

	CCamera *pCamera = CApplication::GetCamera();

	m_pos = m_apModel[0]->GetPos();				// ���W�̎擾
	m_posDest = pCamera->GetPosR();				// �J�����̍��W�̎擾

	m_posDest.y -= 30.0f;
	D3DXVECTOR3 m_posResult = m_posDest - m_pos;
	D3DXVECTOR3 posDiss = m_pos - pCamera->GetPosV();

	m_pos.x += m_posResult.x / 5;
	m_pos.z += m_posResult.z / 5;
	m_pos.y += m_posResult.y / 5;

	// �O���E���h�̎擾
	CMesh *pGround = CGame::GetGround();

	if (pGround != nullptr)
	{
		// ���̓����蔻��
		bool bCollision = pGround->Collision(&m_pos);

		// �����蔻��̐ݒ�
		SetCollision(bCollision);
	}

	// �e�̔��ˏ���
	Bullet(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z));

	// ���W�̐ݒ�
	m_pRobot->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
	//m_posDest.y -= 10.0f;
	//m_posDest.z += 100.0f;

	/*D3DXVECTOR3 m_posResult = m_posDest - m_pos;

	m_pos.x += m_posResult.x / 5;
	m_pos.z += m_posResult.z / 5;
	m_pos.y += m_posResult.y / 5;*/
}

//=========================================
// �e�̔��ˏ���
//=========================================
void CPlayer3D::Bullet(D3DXVECTOR3 pos)
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �}�l�[�W���\�̃��[�h�ƈ�v������
	if (m_mode == (CPlayer3D::PLAYER_MODE)CPlayerManager::GetMode())
	{
		//�e�̔���
		if (pKeyboard->GetPress(DIK_SPACE))
		{
			BulletDelay++;

			//�o���b�g�̔��˃��[�g
			if (BulletDelay >= 10)
			{
				int BulletSpeed = 50;

				// �J�������̎擾
				CCamera *pCamera = CApplication::GetCamera();

				// �l�^�̎��v���C���[�̌������ς��Ȃ��̂ŃJ�����̃N�H�[�^�j�I��������
				CBullet::Create(pos, pCamera->GetQuaternion());

				BulletDelay = 0;
			}
		}
	}
}

//=========================================
// �p�����䏈��
//=========================================
void CPlayer3D::Attitude()
{
	// �}�E�X�̎擾
	CMouse *pMouse = CApplication::GetMouse();

	//�}�E�X�̈ړ���
	D3DXVECTOR3 MouseMove = pMouse->GetMouseMove();

	bool hasLeftClick = pMouse->GetPress(CMouse::MOUSE_KEY_LEFT);

	if (hasLeftClick)
	{
		if (pMouse->GetMouseMove().x > 0.0f)
		{//�}�E�X���E�����ɍs�����Ƃ��@�̂����ɌX����
			m_MouseMove.x -= 0.01f;
		}
		else if (pMouse->GetMouseMove().x < 0.0f)
		{//�}�E�X���������ɍs�����Ƃ��@�̂��E�ɌX����
			m_MouseMove.x += 0.01f;
		}
		else
		{
			//�}�E�X�𓮂����ĂȂ��Ƃ��ɏ��X�Ƀ}�E�X��߂��Ă�����
			if (m_MouseMove.x >= 0.0f)
			{
				m_MouseMove.x -= 0.01f;
			}
			else if (m_MouseMove.x <= 0.0f)
			{
				m_MouseMove.x += 0.01f;
			}
		}

		//�p�x�̐���
		if (m_MouseMove.x >= D3DX_PI * 0.35f)
		{
			m_MouseMove.x = D3DX_PI * 0.35f;
		}
		//�p�x�̐���
		if (m_MouseMove.x <= D3DX_PI * -0.35f)
		{
			m_MouseMove.x = D3DX_PI * -0.35f;
		}

		//D3DXVECTOR3 rot = GetRotation();

		////��]
		//SetRotation(D3DXVECTOR3(rot.x, rot.y, m_MouseMove.x));
	}
}