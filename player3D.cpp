//=========================================
//
//	�v���C���[�̏���
//	Author:�y���m��
//
//=========================================
#include "player3D.h"
#include "camera.h"
#include "application.h"
#include "bullet3D.h"
#include "input.h"
#include "inputkeyboard.h"
#include "texture.h"
#include "billboard.h"
#include "Object2D.h"
#include "mouse.h"
#include "game.h"
#include "meshfield.h"
#include "bullet.h"
#include "target.h"
#include "model3D.h"
#include "player_manager.h"
#include "radar.h"
#include "motion.h"
#include "enemy.h"
#include "bullet.h"
#include "parts.h"
#include "debug_proc.h"
#include "sound.h"
#include "explosion.h"

int BulletDelay = 0;

//=========================================
//�R���X�g���N�^
//=========================================
CPlayer3D::CPlayer3D()
{
	SetObjectType(OBJECT_PLAYER);
	m_mode = MODE_MAX;
	m_quaternion = D3DXQUATERNION(1.0f, 1.0f, 1.0f, 1.0f);
	m_Nearest_object = nullptr;
	m_nNumHandParts = 10;
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
	m_size = m_apModel[0]->GetMaterial()->size;

	m_Radar = nullptr;

	// �v���C���[�����[�_�[��ɕ\��������
	m_Radar = CRadar::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetObjectinfo(), CRadar::RADAR_PLAYER);

	//=========================================
	// �l�^���f���̓ǂݍ���
	//=========================================
	m_pRobot = new CModel3D;

	m_pRobot->SetModelID(1);
	m_pRobot->Init();
	m_pRobot->SetPos(pos);
	m_pRobot->SetSize(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	m_pAttack = new CModel3D;
	m_pAttack->Init();
	m_pAttack->SetPos(pos);
	m_pAttack->SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pAttack->SetSize(m_pAttack->GetMaterial()->size);

	//�g�嗦�̐ݒ�
	/*m_apModel[0]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));*/

	//�傫���̐ݒ�
	m_size = m_pRobot->GetMaterial()->size;

	SetSize(m_size);

	SetPosition(pos);

	CMotionModel3D::Init(pos);

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CPlayer3D::Update()
{
	// �Â����W�̐ݒ�
	SetPosOld(m_pos);

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

	// ���b�N�I������
	LockOn();

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
// �T�v : ��ʏ�ōł��߂��G��_��
//=========================================
void CPlayer3D::LockOn()
{
	CObject *object = CObject::GetObjectTop();

	// ��r�p�ő�T�C�Y
	int MAX_SIZE = 0;

	//=========================================
	// ���ݑ��݂���^�[�Q�b�g�̃T�C�Y���r���A
	// ��ԑ傫���^�[�Q�b�g�̃I�u�W�F�N�g���A
	// �ł��߂��I�u�W�F�N�g�Ƃ��ĕۑ�����B
	//=========================================
	while (object)
	{
		if (object != nullptr)
		{
			EObjType ObjType = object->GetObjectType();

			if (ObjType == OBJECT_TARGET)
			{
				CTarget *pTarget = (CTarget*)object;

				if (MAX_SIZE <= pTarget->GetSize())
				{
					// ��r�p�Ɍ��݂̍ő�T�C�Y��ۑ�
					MAX_SIZE = pTarget->GetSize();

					// �ő�T�C�Y�̃I�u�W�F�N�g�̕ۑ�
					m_Nearest_object = pTarget->GetTargetObject();
				}
			}
		}
		object = object->GetObjectNext();
	}
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

	SetQuaternion(m_quaternion);

}

//=========================================
//	��s��Ԃ̃v���C���[�̏���
//	Author : �y���m��
//	�T�v : �v���C���[�̔�s���
//=========================================
void CPlayer3D::UpdateFly()
{
	// �N�H�[�^�j�I�����擾����
	D3DXQUATERNION quaternion = m_apModel[0]->GetQuaternion();

	// �ڕW��pos
	D3DXVECTOR3	m_posDest;
	//�J����
	CCamera *pCamera = CApplication::GetCamera();

	m_pos = m_apModel[0]->GetPos();				// ���W�̎擾
	m_posDest = pCamera->GetPosR();				// �J�����̍��W�̎擾

	D3DXVECTOR3 m_posResult = m_posDest - m_pos;

	m_pos.x += m_posResult.x / 5;
	m_pos.z += m_posResult.z / 5;
	m_pos.y += m_posResult.y / 5;

	m_pos = MtxPos(D3DXVECTOR3(0.0f, -5.0f, 0.0f), quaternion, m_pos);

	int MOVE_SPEED = 5.0f;

	// ��]
	Rotate();

	//���b�N�I������
	LockOn();

	// �p�����䏈��
	//Attitude();

	// �e�̔��ˏ���
	Bullet(m_pos);

	// ���W�̐ݒ�
	m_apModel[0]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	GroundCollison();
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

	m_posDest.y -= 100.0f;

	D3DXVECTOR3 m_posResult = m_posDest - m_pos;
	D3DXVECTOR3 posDiss = m_pos - pCamera->GetPosV();

	m_pos.x += m_posResult.x / 5;
	m_pos.z += m_posResult.z / 5;
	m_pos.y += m_posResult.y / 5;

	// �e�̔��ˏ���
	Bullet(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z));

	// �n�ʂƂ̓����蔻��
	GroundCollison();

	// �W�����v����
	Jump();

	// �U������
	Slash();

	if (m_bMotion)
	{
		m_MotionCnt++;

		if (m_MotionCnt >= 60)
		{
			m_bMotion = false;
			m_MotionCnt = 0;
		}
	}

	// �ړ�����
	Move();

	if (!m_bMotion && !m_bMove)
	{
		CMotion *pMotion = GetMotion();

		pMotion->SetNumMotion(0);
	}

	// ���W�̐ݒ�
	m_pRobot->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));
}

//=========================================
// �e�̔��ˏ���
//=========================================
void CPlayer3D::Bullet(D3DXVECTOR3 pos)
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	if (m_Nearest_object && m_mode == (CPlayer3D::PLAYER_MODE)CPlayerManager::GetMode())
	{
		if (!(m_Nearest_object->GetSize().x == 0.0f && m_Nearest_object->GetSize().y == 0.0f))
		{
			if (pKeyboard->GetPress(DIK_L))
			{
				BulletDelay++;

				if (BulletDelay >= 10)
				{
					int BulletSpeed = 50;

					// ��������e�𔭎˂���
					CBullet3D::Create(D3DXVECTOR3(50.0f, 0.0f, 0.0f), m_quaternion, m_Nearest_object, this, 30);
					CBullet3D::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), m_quaternion, m_Nearest_object, this, 30);

					BulletDelay = 0;
				}
			}
		}
	}
}

//=========================================
// �p�����䏈��
//=========================================
void CPlayer3D::Attitude()
{
	//// �}�E�X�̎擾
	//CMouse *pMouse = CApplication::GetMouse();

	////�}�E�X�̈ړ���
	//D3DXVECTOR3 MouseMove = pMouse->GetMouseMove();

	//// ���͏��̎擾
	//static const float MIN_MOUSE_MOVED = 2.0f;		// ���̒l�ȏ㓮�����Ȃ��Ɣ������Ȃ�

	//if (!(!(D3DXVec3Length(&MouseMove) > MIN_MOUSE_MOVED) && !(D3DXVec3Length(&MouseMove) < -MIN_MOUSE_MOVED)))
	//{
	//	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	//	if (hasRightClick)
	//	{
	//		if (pMouse->GetMouseMove().x > 0.0f)
	//		{//�}�E�X���E�����ɍs�����Ƃ��@�̂����ɌX����
	//			m_MouseMove.x -= 0.01f;
	//		}
	//		else if (pMouse->GetMouseMove().x < 0.0f)
	//		{//�}�E�X���������ɍs�����Ƃ��@�̂��E�ɌX����
	//			m_MouseMove.x += 0.01f;
	//		}
	//		else
	//		{
	//			//�}�E�X�𓮂����ĂȂ��Ƃ��ɏ��X�Ƀ}�E�X��߂��Ă�����
	//			if (m_MouseMove.x >= 0.0f)
	//			{
	//				m_MouseMove.x -= 0.01f;
	//			}
	//			else if (m_MouseMove.x <= 0.0f)
	//			{
	//				m_MouseMove.x += 0.01f;
	//			}
	//		}

	//		//�p�x�̐���
	//		if (m_MouseMove.x >= D3DX_PI * 0.35f)
	//		{
	//			m_MouseMove.x = D3DX_PI * 0.35f;
	//		}
	//		//�p�x�̐���
	//		if (m_MouseMove.x <= D3DX_PI * -0.35f)
	//		{
	//			m_MouseMove.x = D3DX_PI * -0.35f;
	//		}

	//		D3DXVECTOR3 rot = GetRotation();

	//		SetRotation(D3DXVECTOR3(rot.x, rot.y, m_MouseMove.x));

	//		////��]
	//		//m_apModel[0]->SetRot(D3DXVECTOR3(rot.x, rot.y, m_MouseMove.x));
	//	}
	//}
	//else
	//{

	//	D3DXVECTOR3 rot = GetRotation();

	//	rot.z += (0.0f - rot.z) * 0.05f;

	//	SetRotation(D3DXVECTOR3(rot.x, rot.y, rot.z));

	//	m_MouseMove.x = rot.z;
	//}
}

//=========================================
// �ߐڍU������
//=========================================
void CPlayer3D::Slash()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	CMotion *pMotion = GetMotion();

	if (pKeyboard->GetTrigger(DIK_Z) && !m_bMotion)
	{
		CSound::PlaySound(CSound::SOUND_SE_MARSHALL_ATTACK);

		// ���[�V�����̐ݒ�
		pMotion->SetNumMotion(2);

		m_bMotion = true;
	}

	if (m_MotionCnt == 30)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (pMotion != nullptr)
		{// ��̃I�u�W�F�N�g�̈ʒu
			CParts *pHand = pMotion->GetParts(m_nNumHandParts);
			D3DXMATRIX mtxParts = pHand->GetMtxWorld();
			D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mtxParts);
		}

		// �U���̓����蔻��
		m_pAttack->SetPos(pos);

		// �I�u�W�F�N�g�̎擾
		CObject *pObj = CObject::GetObjectTop();
		CEnemy *pEnemy = nullptr;

		D3DXVECTOR3 EnemyPos = {};
		D3DXVECTOR3 EnemySize = {};

		//�v���C���[�̍��W���擾
		while (pObj)
		{
			if (pObj != nullptr)
			{
				EObjType ObjType = pObj->GetObjectType();

				if (ObjType == OBJECT_ENEMY)
				{
					pEnemy = dynamic_cast<CEnemy*> (pObj);

					EnemyPos = pEnemy->GetPosition();
					EnemySize = pEnemy->GetSize() * 10.0f;

					D3DXVECTOR3 size = m_pAttack->GetSize() * 5;

					D3DXVECTOR3 SizeTarget = pEnemy->GetSize();

					if ((pos.z - size.z) < (EnemyPos.z + SizeTarget.z)
						&& (pos.z + size.z) > (EnemyPos.z - SizeTarget.z)
						&& (pos.x - size.x) < (EnemyPos.x + SizeTarget.x)
						&& (pos.x + size.x) > (EnemyPos.x - SizeTarget.x)
						&& (pos.y - size.y) < (EnemyPos.y + SizeTarget.y)
						&& (pos.y + size.y) > (EnemyPos.y - SizeTarget.y))
					{// ���f�����ɂ���(XYZ��)
						pEnemy->ManageHP(-10);
						// �e���ڕW�I�u�W�F�N�g�ƏՓ˂�������ł��鏈��
						CExplosion::Create(m_pos, m_quaternion);
						break;
					}
				}
			}
			pObj = pObj->GetObjectNext();
		}
	}
}

//=========================================
// �W�����v����
//=========================================
void CPlayer3D::Jump()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	CCamera *pCamera = CApplication::GetCamera();
	CMotion *pMotion = GetMotion();

	if (pKeyboard->GetTrigger(DIK_SPACE) && !m_bMotion)
	{
		// ���[�V�����̐ݒ�
		pMotion->SetNumMotion(3);

		m_bMotion = true;
		m_bJump = true;

		pCamera->SetUp(true);
	}
}

//=========================================
// �n�ʂƂ̓����蔻��
//=========================================
void CPlayer3D::GroundCollison()
{
	// �I�u�W�F�N�g�̎擾
	CObject *pMesh = CObject::GetObjectTop();
	CMesh *pGround = nullptr;

	//�v���C���[�̍��W���擾
	while (pMesh)
	{
		if (pMesh != nullptr)
		{
			EObjType ObjType = pMesh->GetObjectType();

			if (ObjType == OBJECT_MESH)
			{
				pGround = dynamic_cast<CMesh*> (pMesh);

				if (pGround->GetType() == CMesh::TYPE_GROUND)
				{
					break;
				}
			}
		}
		pMesh = pMesh->GetObjectNext();
	}

	if (pGround != nullptr)
	{
		// ���̓����蔻��
		if (pGround->Collision(&m_pos))
		{
			if (!m_bMotion)
			{
				m_bJump = false;
				m_bCollision = true;
			}
		}
		else
		{
			m_bCollision = false;
		}

		// �����蔻��̐ݒ�
		SetCollision(m_bCollision);
	}
}

//=========================================
// �ړ�
//=========================================
void CPlayer3D::Move()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	CMotion *pMotion = GetMotion();

	if (m_mode != (CPlayer3D::PLAYER_MODE)CPlayerManager::GetMode())
	{
		return;
	}

	if ((pKeyboard->GetPress(DIK_W) || pKeyboard->GetPress(DIK_A) || pKeyboard->GetPress(DIK_D) || pKeyboard->GetPress(DIK_S)) && !m_bMotion && !m_bJump)
	{
		m_StepCnt++;
		m_bMove = true;
	}
	else
	{
		m_bMove_Motion_Check = false;
		m_bMove = false;
	}

	if (m_bMove && !m_bMove_Motion_Check)
	{
		m_bMove_Motion_Check = true;
		pMotion->SetNumMotion(1);
	}

	if (m_StepCnt == 18)
	{
		m_StepCnt = 0;
		CSound::PlaySound(CSound::SOUND_SE_STEP);
	}
}