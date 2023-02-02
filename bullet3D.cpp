//=========================================
//
//	�o���b�g3D�̏���
//	Author:�y���m��
//
//=========================================
#include "bullet3D.h"
#include "model3D.h"
#include "explosion.h"
#include "game.h"
#include "meshfield.h"
#include "application.h"
#include "enemy.h"
#include "debug_proc.h"
#include "camera.h"
#include "calculation.h"

//=========================================
//�R���X�g���N�^
//=========================================
CBullet3D::CBullet3D()
{
	SetObjectType(OBJECT_PLAYER);
}

//=========================================
//�f�X�g���N�^
//=========================================
CBullet3D::~CBullet3D()
{
}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CBullet3D::Init(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	m_MissileSpeed = 0;

	m_pModel = new CModel3D;

	// �Ƃ肠����������L�΂�
	m_length = 10000.0f;

	m_MissileCnt = 0;

	// Rot�̐ݒ�===============================================

	D3DXVECTOR3 BulletPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	BulletPos = GetPosition();
	m_posTarget = m_TargetObj->GetPosition();

	D3DXVECTOR3 FellowRot = AtanRot(m_posTarget, BulletPos);

	m_pModel->SetRot(D3DXVECTOR3(FellowRot.x, FellowRot.y, 0.0f));

	//==========================================================

	//�~�T�C���A���[�g�̐���====================================l

	if (m_TargetObj->GetObjectType() == OBJECT_PLAYER)
	{// �_��ꂽ�^�[�Q�b�g���v���C���[�������ꍇ�̂ݐ�������
		m_pAlert = nullptr;

		m_pAlert = CAlert::Create(m_pos,this);

		m_pAlert->SetObject(this);
	}

	//==========================================================

	// ������
	m_pModel->Init();

	m_pModel->SetModelID(13);

	//�g�嗦�̐ݒ�
	m_pModel->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CBullet3D::Update()
{
	// �Â����W�̐ݒ�
	SetPosOld(m_pos);

	// �Â����W�Ɍ��݂̍��W��������
	m_posOld = m_pos;

	// �e�̈ړ�����
	BulletMove();

	CDebugProc::Print("pos : %.1f\n",m_pos.y);

	// �����蔻��
	if (Collision(m_TargetObj, false))
	{
		// �e���ڕW�I�u�W�F�N�g�ƏՓ˂�������ł��鏈��
		CExplosion::Create(m_TargetObj->GetPosition(), m_quaternion);
		Uninit();
		m_TargetObj->ManageHP(-5);
		CDebugProc::Print("����������I");

		if (m_TargetObj->GetObjectType() == OBJECT_ENEMY)
		{// �G�l�~�[�̃X�e�[�g��ω�����
			CEnemy* pEnemy = (CEnemy*)m_TargetObj;

			pEnemy->SetState(CEnemy::ENEMY_WARNNING);
		}
	}
	
	// �O���E���h�̎擾
	CMesh *pGround = CGame::GetGround();

	if (pGround != nullptr && m_TargetObj != nullptr)
	{
		// ���̓����蔻��
		if (pGround->Collision(&m_pos))
		{
			Uninit();
			CExplosion::Create(m_pos, m_quaternion);
		}
	}

	//=========================================
	// �~�T�C���̎���
	//=========================================
	m_MissileCnt++;

	if (m_MissileCnt >= 300)
	{
		Uninit();
	}
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CBullet3D::Uninit()
{
	if (m_pAlert != nullptr)
	{
		m_pAlert->Uninit();
	}

	// ���f���̏I������
	m_pModel->Uninit();

	//�I�u�W�F�N�g�̉������
	CObject::Release();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CBullet3D::Draw()
{
	// �f�o�C�X�̎擾
	PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	// �J�����O�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_WorldMtx);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);                // �s��ړ��֐�
	D3DXMatrixMultiply(&m_WorldMtx, &m_WorldMtx, &mtxTrans);					// �s��|���Z�֐�

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_WorldMtx);

	// ���f���̕`�揈��
	m_pModel->Draw();
}

//=====================================================================
// �I�u�W�F�N�g�̃N���G�C�g
// ���� : �ˎ肩��̋����A�N�H�[�^�j�I���A�ڕW�A�ˎ�A�U���̃^�C�~���O
//=====================================================================
CBullet3D* CBullet3D::Create(const D3DXVECTOR3 &pos ,const D3DXQUATERNION &quaternion, CObject *object, CObject *Shooter, int val)
{
	int nNumCreate = m_nNumAll;

	CBullet3D* pCBullet3D = nullptr;
	CObject *pShooter = Shooter;

	pCBullet3D = new CBullet3D;

	if (pCBullet3D != nullptr)
	{
		pCBullet3D->m_TargetObj = object;
		pCBullet3D->Init(pCBullet3D->MtxPos(pos,quaternion, pShooter->GetPosition()));
		pCBullet3D->SetRot(pShooter->GetRot());
		pCBullet3D->SetQue(pShooter->GetQuaternion());
		pCBullet3D->SetSearchValue(val);
	}

	return pCBullet3D;
}

//=========================================
// ���b�N�I���̏���
//=========================================
D3DXVECTOR3 CBullet3D::LockOn()
{
	// �U���J�E���g�̉��Z
	m_SearchCnt++;

	//���̎擾
	D3DXVECTOR3 BulletPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	BulletPos = GetPosition();

	if (m_SearchCnt >= m_SearchValue)
	{// �T�[�`�J�E���g�����̍��W�𒴂�����
		m_posTarget = m_TargetObj->GetPosition();
		m_SearchCnt = 0;
	}

	// �G�̂������
	D3DXVECTOR3 sub = D3DXVECTOR3(0.0f, 0.0f, 0.f);
	D3DXVECTOR3 distance = m_posTarget - BulletPos;

	m_FllowRot = AtanRot(m_posTarget, BulletPos);

	m_length = D3DXVec3Length(&(BulletPos - m_posTarget));

	if (m_MissileSpeed >= m_length)
	{// �~�T�C���X�s�[�h��length�ȏ�̎�
		//m_MissileSpeed = 5.0f;
		m_SearchValue = 30;
	}

	sub.z = sinf(m_FllowRot.x) * cosf(m_FllowRot.y) * m_MissileSpeed;
	sub.x = sinf(m_FllowRot.x) * sinf(m_FllowRot.y) * m_MissileSpeed;
	sub.y = cosf(m_FllowRot.x)  * m_MissileSpeed;

	CDebugProc::Print("BulletPos.y %.1f\n", BulletPos.y);
	CDebugProc::Print("distance.y %.1f\n",distance.y);

	return sub;
}

//=========================================
// �e�̈ړ��̐���
//=========================================
void CBullet3D::BulletMove()
{
	if (m_TargetObj != nullptr)
	{
		// ���X�Ƀ~�T�C���̃X�s�[�h�𑬂�����
		if (m_MissileSpeed <= 0.3f)
		{
			m_MissileSpeed += 0.1f;
		}
		else
		{
			m_MissileSpeed += 1.0f;
		}

		// �ڕW�̃}�g���b�N�X���擾
		D3DXMATRIX	*TargetMatrix = m_TargetObj->GetMtxWorld();

		// �ڕW���W
		m_TargetPos = m_TargetObj->GetPosition();

		m_FrontMoveCnt++;

		if (m_FrontMoveCnt <= 60)
		{
			D3DXVECTOR3 MoveRot;
			D3DXVECTOR3 Pos;

			if (m_quaternion != D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f))
			{
				// �N�H�[�^�j�I�����g�p�����ړ��̓K��
				Pos = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, m_MissileSpeed), m_pos, m_quaternion);
			}
			else
			{
				MoveRot = m_ShooterRot;

				// �ړ���
				Pos = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, m_MissileSpeed), m_pos, MoveRot);
			}

			// �p�x�̐ݒ�
			m_FllowRot = AtanRot(Pos, m_pos);

			// ���݋@�̂̐��ʕ���
			D3DXVECTOR3 VecFront = Pos - m_pos;

			D3DXVec3Normalize(&VecFront, &VecFront);

			// �ړ���
			m_move = VecFront * m_MissileSpeed;
		}
		else
		{
			if (m_MissileSpeed <= m_length && m_MissileSpeed <= 100)
			{// �ڕW��pos�̋�����Speed�ȏ�Ȃ�LockOn����
			 // ���b�N�I���ŏo�����l�����Z����
				D3DXVECTOR3 Result;
				D3DXVECTOR3 Sub = LockOn();
				D3DXVECTOR3 EnemyPos = m_TargetObj->GetPosition();

				m_length = D3DXVec3Length(&(m_pos - EnemyPos));
				
				if (m_length <= 500.0f)
				{// ���̋����ɂȂ������A�m���ɓ�����悤�ɂ���(���̒Ǐ]���Ɠ�����Ȃ�����)
					// �G�l�~�[�ւ̕����x�N�g��
					D3DXVECTOR3 VecEnemy = EnemyPos - m_pos;

					// ���K��
					D3DXVec3Normalize(&VecEnemy, &VecEnemy);

					m_move = VecEnemy * m_MissileSpeed;
				}
				else
				{
					Result = Sub - m_move;
					m_move += Result * 0.03f;
					m_pos += m_move;
				}
			}
		}

		// �ړ��ʂ����Z����
		m_pos += m_move;

		m_pModel->SetRot(D3DXVECTOR3(m_FllowRot.x, m_FllowRot.y, 0.0f));

		// �d��
		m_pos.y -= 3.0f;

		// ���W�̐ݒ�
		SetPosition(m_pos);

		// ���f���̍��W��ݒ肷��
		m_pModel->SetPos(m_pos);

		// ���f���̍X�V����
		m_pModel->Update();
	}
}

//=========================================
// �N�H�[�^�j�I����Rot�ɕϊ�����
// Author : �y���m��
// �T�v : �N�H�[�^�j�I����Rot�ɕς���
//=========================================
D3DXVECTOR3 CBullet3D::QuaternionChange()
{
	// �N�H�[�^�j�I�����擾����
	D3DXVECTOR3 ChangeRot;

	float z1 = 2 * (m_quaternion.x * m_quaternion.y + m_quaternion.z * m_quaternion.w);
	float z2 = m_quaternion.x * m_quaternion.x - m_quaternion.y * m_quaternion.y - m_quaternion.z * m_quaternion.z - m_quaternion.w * m_quaternion.w;
	float z3 = z1 / z2;
	ChangeRot.z = atanf(z3);

	float y1 = 2 * (m_quaternion.x * m_quaternion.z - m_quaternion.y * m_quaternion.w);
	ChangeRot.y = atanf(y1);

	float x1 = 2 * (m_quaternion.x * m_quaternion.w + m_quaternion.y * m_quaternion.z);
	float x2 = m_quaternion.x * m_quaternion.x + m_quaternion.y * m_quaternion.y - m_quaternion.z * m_quaternion.z - m_quaternion.w * m_quaternion.w;
	float x3 = x1 / x2;
	ChangeRot.x = -atanf(x3);

	return ChangeRot;
}