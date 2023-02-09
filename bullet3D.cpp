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
#include "sound.h"
#include "enemy.h"
#include "debug_proc.h"
#include "camera.h"
#include "calculation.h"
#include "missile_locus.h"

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

	//�~�T�C���A���[�g�̐���====================================

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

	// �O�Ղ𐶐�����^�C�~���O
	m_LocusCnt++;

	if (m_LocusCnt % 1 == 0)
	{
		// �~�T�C���̋O�Ղ̐���
		CMissile_Locus::Create(m_pos, m_TargetObj);
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
		CSound::PlaySound(CSound::SOUND_SE_MISSILE);
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
		//m_MissileSpeed = 30.0f;
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
		
		D3DXVECTOR3 MoveRot;
		D3DXVECTOR3 Pos;

		if (m_FrontMoveCnt <= 30)
		{
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
			D3DXVECTOR3 EnemyVec = m_TargetObj->GetPosition() - GetPosition();

			// �x�N�g�����Z�o
			D3DXVECTOR3 MoveVec = {};

			D3DXVec3Normalize(&MoveVec, &m_move);
			D3DXVec3Normalize(&EnemyVec, &EnemyVec);

			// �i�s�����x�N�g������o�������ݒn���猩���G�l�~�[�ւ̊p�x
			float AdvanceRot = acos((MoveVec.x * EnemyVec.x) + (MoveVec.y * EnemyVec.z) + (MoveVec.z * EnemyVec.z));

			if (AdvanceRot <= D3DX_PI * 0.25f)
			{
				AdvanceRot = AdvanceRot * (180 * D3DX_PI);

				D3DXVECTOR3 Vec = EnemyVec / AdvanceRot;

				// 1�t���[���ɓ����p�x��ݒ肷��
				float OneRadian = (1 * (180 / D3DX_PI));

				D3DXVec3Normalize(&Vec, &Vec);

				D3DXVECTOR3 A = Vec;

				D3DXVECTOR3 AB = Vec *  OneRadian;
				D3DXVec3Normalize(&AB, &AB);

				//m_move = AB * m_MissileSpeed;

				m_move = AB * m_MissileSpeed;

				// �p�x�̐ݒ�
				m_FllowRot = AtanRot(m_pos + m_move, m_pos);
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

//=========================================
//�@�~�T�C���̒Ǐ]����
//=========================================
void CBullet3D::Follow()
{
	if (m_TargetObj != nullptr)
	{
		D3DXVECTOR3 EnemyVec = m_TargetObj->GetPosition() - GetPosition();

		// �x�N�g�����Z�o
		D3DXVECTOR3 MoveVec = {};

		D3DXVec3Normalize(&MoveVec, &m_move);
		D3DXVec3Normalize(&EnemyVec, &EnemyVec);

		float AdvanceRot = acos((MoveVec.x * EnemyVec.x) + (MoveVec.y * EnemyVec.z) + (MoveVec.z * EnemyVec.z));
	}
}