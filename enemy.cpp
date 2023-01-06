//=========================================
//
//	�G�l�~�[�̏���
//	Author:�y���m��
//
//=========================================
#include "enemy.h"
#include "application.h"
#include "input.h"
#include "inputkeyboard.h"
#include "texture.h"
#include "game.h"
#include "model.h"
#include "model3D.h"
#include "target.h"
#include "debug_proc.h"
#include "meshfield.h"
#include "radar.h"

//=========================================
//�O���[�o���ϐ�
//=========================================
int nEnemyNumber = 0;

//=========================================
//�R���X�g���N�^
//=========================================
CEnemy::CEnemy()
{
	SetObjectType(OBJECT_ENEMY);
	m_state = ENEMY_IDOL;
	SetHP(100);
	m_type = ENEMY_MAX;
}

//=========================================
//�f�X�g���N�^
//=========================================
CEnemy::~CEnemy()
{
}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CEnemy::Init(const D3DXVECTOR3 &pos)
{
	m_apModel[0] = new CModel3D;

	m_apModel[0]->SetModelID(3);
	m_apModel[0]->Init();
	m_apModel[0]->SetPos(pos);

	m_pos = pos;

	//�g�嗦�̐ݒ�
	m_apModel[0]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// ���W�̐ݒ�
	SetPosition(pos);

	//�傫���̐ݒ�
	SetSize(m_apModel[0]->GetSize());

	m_scale = 1.0f;
	
	// ���[�V�����̏���������
	CMotionModel3D::Init(pos);

	m_Target = nullptr;

	//�^�[�Q�b�g(�G�̋����\��)�̐ݒ�
	m_Target = CTarget::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f),this);

	m_Target->SetObject(this);

	m_Radar = nullptr;

	// �G�l�~�[�����[�_�[��ɕ\��������
	m_Radar = CRadar::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetObjectinfo());

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CEnemy::Update()
{
	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerRot;

	// ���S����
	Death();

	// ���W�̎擾
	m_pos = GetPosition();

	// �I�u�W�F�N�g�̎擾
	CObject *object = CObject::GetObjectTop();

	//�v���C���[�̍��W���擾
	while(object)
	{
		if (object != nullptr)
		{
			EObjType ObjType = object->GetObjectType();

			if (ObjType == OBJECT_PLAYER)
			{
				PlayerPos = object->GetPosition();
				PlayerRot = object->GetRot();
				break;
			}
		}
		object = object->GetObjectNext();
	}

	//=========================================
	//�ҋ@��Ԃ̃G�l�~�[�̍s������
	//=========================================
	/*if (m_state == ENEMY_IDOL)
	{*/
	float EnemySpeed = 3.0f;
	D3DXVECTOR3 move;

	if(m_type == ENEMY_FLY)
	{ 
		// �ړ���
		move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
			-(m_rot.x * EnemySpeed),
			cosf(m_rot.y) * EnemySpeed);
	}

	if (m_type == ENEMY_GROUND)
	{
		// �ړ���
		move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
			-10.0f,
			cosf(m_rot.y) * EnemySpeed);
	}

	m_rot.y += 0.01f;

	//=========================================
	//// �퓬��Ԃ̃G�l�~�[�̍s������
	////=========================================
	//if (m_state == ENEMY_WARNNING)
	//{
	//	// �v���C���[�ƃG�l�~�[�̋���
	//	D3DXVECTOR3 posDiss = PlayerPos - pos;
	//	D3DXVECTOR3 rotDest = D3DXVECTOR3
	//	(atan2f(posDiss.x, posDiss.z),
	//		atan2f(posDiss.y, sqrtf((posDiss.x * posDiss.x) + (posDiss.z * posDiss.z))),
	//		0.0f);

	//	//�G�l�~�[�̊p�x���߁A�v���C���[�̕���������
	//	rot.y += m_apModel[0]->NormalizeRot(rotDest.x - rot.y) * 0.01f;
	//	rot.z = m_apModel[0]->LimitedRot(-rot.y, 0.25f);
	//	rot.x = 0.1f * (-posDiss.y * 0.3f);
	//}

	//�G�l�~�[�̈ړ�
	m_pos += move;

	// �O���E���h�̎擾
	CMesh *pGround = CGame::GetGround();

	if (pGround != nullptr)
	{
		// ���̓����蔻��
		bool bCollision = pGround->Collision(&m_pos);
	}

	// �G�l�~�[�̉�]�̐ݒ�
	m_apModel[0]->SetRot(m_rot);

	// ���W�̐ݒ�
	SetPosition(m_pos);

	// ��]�̐ݒ�
	SetRotation(m_rot);

	// ���[�V�����̍X�V����
	CMotionModel3D::Update();

	// �J�����̎��_
	CDebugProc::Print("�G�l�~�[�̃X�N���[�����W : (%.3f , %.3f , %.3f )\n", m_pos.x, m_pos.y, m_pos.z);
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CEnemy::Uninit()
{
	// ���f���̉��
	m_apModel[0]->Uninit();

	// ���[�V�����̏I������
	CMotionModel3D::Uninit();

	CObject::Release();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CEnemy::Draw()
{
	PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;
	D3DXMATRIX mtxParents;

	// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;
	// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXVECTOR3 scale(5.0f, 5.0f, 5.0f);

	// �J�����O�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_WorldMtx);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);                // �s��ړ��֐�
	D3DXMatrixMultiply(&m_WorldMtx, &m_WorldMtx, &mtxTrans);					// �s��|���Z�֐�

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_WorldMtx);

	// ������
	m_apModel[0]->Draw();

	// ���[�V�����̕`�揈��
	CMotionModel3D::Draw();

	SetMtxWorld(m_WorldMtx);
}

//=========================================
// ���S����
//=========================================
void CEnemy::Death()
{
	int nLife = GetHP();

	if (nLife <= 0)
	{
		// �G�l�~�[�̑���
		((CGame*)CApplication::GetModeObject())->DeleteEnemy(this);

		// ������
		CEnemy::Uninit();
	}
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CEnemy* CEnemy::Create(const D3DXVECTOR3 &pos)
{
	CEnemy* pCEnemy = nullptr;

	pCEnemy = new CEnemy;

	if (pCEnemy != nullptr)
	{
		pCEnemy->Init(pos);
		
		// �G�l�~�[�ԍ��̎w��
		pCEnemy->SetNumber(nEnemyNumber);
		// �G�l�~�[�ԍ��̉��Z
		nEnemyNumber++;
	}

	return pCEnemy;
}