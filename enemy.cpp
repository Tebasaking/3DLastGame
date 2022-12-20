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
	SetHP(5);
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

	//�g�嗦�̐ݒ�
	m_apModel[0]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//�傫���̐ݒ�
	SetSize(m_apModel[0]->GetSize());

	m_scale = 1.0f;

	m_pos = pos;
	
	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CEnemy::Update()
{
	m_pos = m_apModel[0]->GetPos();
	m_rot = m_apModel[0]->GetRot();

	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerRot;

	Death();

	//�v���C���[�̍��W���擾
	while(0)
	{
		CObject *object = CObject::GetObjectTop();

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
	}

	//=========================================
	//�ҋ@��Ԃ̃G�l�~�[�̍s������
	//=========================================
	if (m_state == ENEMY_IDOL)
	{
		m_rot.y += 0.01f;

		if (SearchEye(PlayerPos, m_pos, D3DX_PI / 2.0f, 50.0f, m_rot.y))
		{
			m_state = ENEMY_WARNNING;
		}
	}

	//=========================================
	// �퓬��Ԃ̃G�l�~�[�̍s������
	//=========================================
	if (m_state == ENEMY_WARNNING)
	{
		// �v���C���[�ƃG�l�~�[�̋���
		D3DXVECTOR3 posDiss = PlayerPos - m_pos;
		D3DXVECTOR3 rotDest = D3DXVECTOR3
		(atan2f(posDiss.x, posDiss.z),
			atan2f(posDiss.y, sqrtf((posDiss.x * posDiss.x) + (posDiss.z * posDiss.z))),
			0.0f);

		//�G�l�~�[�̊p�x���߁A�v���C���[�̕���������
		m_rot.y += m_apModel[0]->NormalizeRot(rotDest.x - m_rot.y) * 0.01f;
		m_rot.z = m_apModel[0]->LimitedRot(-m_rot.y, 0.25f);
		m_rot.x = 0.1f * (-posDiss.y * 0.3f);
	}

	float EnemySpeed = 3.0f;

	D3DXVECTOR3 move = D3DXVECTOR3(sinf(m_rot.y) * EnemySpeed,
		-(m_rot.x * EnemySpeed),
		cosf(m_rot.y) * EnemySpeed);

	//�G�l�~�[�̈ړ�
	m_pos += move;

	m_apModel[0]->SetRot(m_rot);
	// �G�l�~�[�̃��f���̍��W�ݒ�
	m_apModel[0]->SetPos(m_pos);

	// �G�l�~�[�̍��W�ݒ�
	SetPosition(m_pos);
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CEnemy::Uninit()
{
	m_apModel[0]->Uninit();
	CObject::Release();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CEnemy::Draw()
{
	// ������
	m_apModel[0]->Draw();
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