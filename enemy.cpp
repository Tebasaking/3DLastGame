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
#include "bullet3D.h"

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
	SetHP(10);
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

	// �Â����W�̐ݒ�
	SetPosOld(m_pos);
	
	// �T�C�Y�̐ݒ�
	m_size = m_apModel[0]->GetMaterial()->size;

	//�傫���̐ݒ�
	SetSize(m_size);

	m_scale = 1.0f;

	// ���[�V�����̏���������
	CMotionModel3D::Init(pos);

	m_Target = nullptr;

	//�^�[�Q�b�g(�G�̋����\��)�̐ݒ�
	m_Target = CTarget::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f),this);

	m_Target->SetObject(this);

	m_Radar = nullptr;

	// �G�l�~�[�����[�_�[��ɕ\��������
	m_Radar = CRadar::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetObjectinfo(),CRadar::RADAR_ENEMY);

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CEnemy::Update()
{
	// �Â����W�̐ݒ�
	SetPosOld(m_pos);

	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerRot;

	// �O���E���h�̎擾
	CMesh *pGround = CGame::GetGround();

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

	if (SearchEye(PlayerPos, m_pos, D3DX_PI * 0.5f, 100.0f, m_rot.y))
	{
		m_state = ENEMY_WARNNING;
	}

	// �ړ��ʗp
	D3DXVECTOR3 move;

	float EnemySpeed = 3.0f;

	//=========================================
	// �G�l�~�[�̌��݂̏��
	//=========================================
	//=========================================
	// ��퓬��Ԃ̓G�̍s������
	//=========================================
	if (m_state == ENEMY_IDOL)
	{
		m_rot.y += 0.01f;

		if (m_type == ENEMY_FLY)
		{
			// �ړ���
			move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
				-(m_rot.x * EnemySpeed),
				cosf(m_rot.y) * EnemySpeed);
		}

		if (m_type == ENEMY_GROUND)
		{
			D3DXVECTOR3 CollisionCheck = m_pos;

			if (!pGround->Collision(&CollisionCheck))
			{
				move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
					-10.0f,
					cosf(m_rot.y) * EnemySpeed);
			}
			else
			{
				move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
					-0.0f,
					cosf(m_rot.y) * EnemySpeed);
			}
		}
	}

	//=========================================
	// �퓬��Ԃ̃G�l�~�[�̍s������
	//=========================================
	if (m_state == ENEMY_WARNNING)
	{
		//���̎擾
		D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 posTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		
		Pos = GetPosition();
		posTarget = PlayerPos;

		// �G�̂������
		D3DXVECTOR3 sub = D3DXVECTOR3(0.0f, 0.0f, 0.f);
		// �^�[�Q�b�g�ƌ��ݒn�̋���
		D3DXVECTOR3 distance = posTarget - Pos;

		D3DXVECTOR3 FellowRot = AtanRot(posTarget, Pos);
	
		sub.z = sinf(FellowRot.x) * cosf(FellowRot.y) * EnemySpeed;
		sub.x = sinf(FellowRot.x) * sinf(FellowRot.y) * EnemySpeed;
		sub.y = cosf(atan2f(Pos.y, distance.y))  * EnemySpeed;

		D3DXVECTOR3 rotDest = (D3DXVECTOR3(FellowRot.x - D3DX_PI * 0.5f, FellowRot.y, m_rot.z));
		D3DXVECTOR3 rotResult = rotDest - m_rot;

		rotResult = NormalizeRotXYZ(rotResult);
		
		// ��]�̉��Z
		m_rot += rotResult * 0.01f;

		m_rot = NormalizeRotXYZ(m_rot);

		move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
			-0.0f,
			cosf(m_rot.y) * EnemySpeed);

		// �A�^�b�N�J�E���g��i�߂�
		m_AttackCount++;

		// �A�^�b�N�J�E���g��300�ȏ�̎�
		if (object != nullptr && m_AttackCount >= 300)
		{
			// �v���C���[�ɑ΂��ă~�T�C���𔭎˂���
			Bullet(object);
		}
	}

	// �ړ��ʂ����Z����
	m_pos += move;

	if (pGround != nullptr)
	{
		if (m_type == ENEMY_FLY)
		{
			D3DXVECTOR3 GroundPos = m_pos;
			GroundPos.y -= 100.0f;
			// ���̓����蔻��
			pGround->Collision(&GroundPos);
			m_pos.y = GroundPos.y + 100.0f;
		}
		else if (m_type == ENEMY_GROUND)
		{
			D3DXVECTOR3 GroundPos = m_pos;
			GroundPos.y -= 100.0f;

			// ���̓����蔻��
			if (pGround->Collision(&GroundPos))
			{
				m_pos.y = GroundPos.y;
			}
		}
	}

	// ���W�̐ݒ�
	SetPosition(m_pos);

	// �G�l�~�[�̓����蔻��
	EnemyCollision();

	// �G�l�~�[�̉�]�̐ݒ�
	m_apModel[0]->SetRot(m_rot);

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

	// �I�u�W�F�N�g�̉��
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

		CGame::Add(100);

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

//=========================================
// �G�l�~�[�̒e���ˏ���
//=========================================
void CEnemy::Bullet(CObject *obj)
{
	// ��������e�𔭎˂���
	CBullet3D::Create(D3DXVECTOR3(50.0f, 50.0f, 0.0f), m_quaternion, obj, this,60);
	CBullet3D::Create(D3DXVECTOR3(-50.0f, 50.0f, 0.0f), m_quaternion, obj, this,60);

	m_AttackCount = 0;
}

//=========================================
// �G�l�~�[���m�̓����蔻��
//=========================================
void CEnemy::EnemyCollision()
{
	// �I�u�W�F�N�g�̎擾
	CObject *object = CObject::GetObjectTop();

	//�v���C���[�̍��W���擾
	while (object)
	{
		if (object != nullptr)
		{
			EObjType ObjType = object->GetObjectType();

			if (ObjType == OBJECT_ENEMY && object != this)
			{
				SetCollision(Collision(object, true));
			}
		}
		object = object->GetObjectNext();
	}
}