//**************************************************************************************************
//
// �J��������(camera.h)
// Auther�F�������l
// Author : ���c�C�l
// Author : �y���m��
// �T�v : �J�����̐ݒ菈��
//
//**************************************************************************************************

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "camera.h"
#include "application.h"
#include "render.h"
#include "input.h"
#include "mouse.h"
#include "inputkeyboard.h"
#include "move.h"
#include "calculation.h"
#include "player_manager.h"
#include "debug_proc.h"
#include "game.h"
#include "meshfield.h"
#include "object.h"
#include "joypad.h"
#include "player3D.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
const float CCamera::CAMERA_NEAR = 30.0f;		// �j�A
const float CCamera::CAMERA_FUR = 100000000.0f;	// �t�@�[

//=============================================================================
// �R���X�g���N�^
// Author	: �������l
// �T�v		: �C���X�^���X�������ɍs������
//=============================================================================
CCamera::CCamera() :
	m_posV(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// ���_
	m_posR(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// �����_
	m_vecU(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// ������x�N�g��
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// ����
	m_rotMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),			 	// �ړ�����
	m_quaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f)),	// �N�I�[�^�j�I��
	m_viewType(TYPE_CLAIRVOYANCE),							// ���e���@�̎��
	m_event(EVENT_NORMAL),
	m_fDistance(0.0f),										// ���_���璍���_�܂ł̋���
	m_nCntFly(0.0f),
	m_fRotMove(0.0f)										// �ړ�����
{
	m_mtxWorld = {};	// ���[���h�}�g���b�N�X
	m_mtxProj = {};		// �v���W�F�N�V�����}�g���b�N�X
	m_mtxView = {};		// �r���[�}�g���b�N�X
	m_Objectmode = (CObject::Object_mode)3;
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�ƒ����_�̊Ԃ̋������Z�o����
//=============================================================================
HRESULT CCamera::Init()
{
	switch (m_Objectmode)
	{
	case CObject::NORMAL_MODE:
		m_posV = D3DXVECTOR3(0.0f, 1000.0f, -150.0f);
		m_posR = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �Œ�
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_viewport.MinZ = 0.0f;
		m_viewport.MaxZ = 1.0f;
		break;

	case CObject::RADAR_MODE:
		m_posV = D3DXVECTOR3(0.0f, 5000.0f, -150.0f);
		m_posR = D3DXVECTOR3(0.0f, 5000.0f, 0.0f);
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �Œ�
		m_rot = D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f);
		m_viewport.MinZ = 0.0f;
		m_viewport.MaxZ = 1.0f;
		m_viewType = TYPE_PARALLEL;

		break;
	}

	// ���_�ƒ����_�̋���
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	// �N�H�[�^�j�I���̓K��
	D3DXQuaternionRotationYawPitchRoll(&m_quaternion, m_rot.y, m_rot.x, m_rot.z);

	// �������̊m��
	m_pRoll = new CMove;
	assert(m_pRoll != nullptr);
	m_pRoll->SetMoving(0.015f, 10.0f, 0.0f, 0.1f);

	m_mode = TYPE_FREE;

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �I��
//=============================================================================
void CCamera::Uninit(void)
{
	if (m_pRoll != nullptr)
	{// �I������
	 // �������̉��
		delete m_pRoll;
		m_pRoll = nullptr;
	}
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V
//=============================================================================
void CCamera::Update(void)
{
	if (m_event == EVENT_NORMAL)
	{
		// �J�����̃��[�h���Ƃ̏���
		switch (m_Objectmode)
		{
		case CObject::NORMAL_MODE:
			UpdateNormal();
			break;

		case CObject::RADAR_MODE:
			UpdateRadar();

			break;
		}

		D3DXVECTOR3 Result = m_Dest - m_rotMove;
		m_rotMove += Result * 0.25f;
	}
	else
	{
		FlightEvent();
	}

	CDebugProc::Print("�J�����̍��W : (%f,%f,%f) \n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("�J�����̉�] : (%f,%f,%f,%f) \n", m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
}

//=============================================================================
// �J�����̐ݒ�
// Author : �������l
// �T�v : �r���[�}�g���b�N�X�̐ݒ�
//=============================================================================
void CCamera::Set()
{// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);			// �s�񏉊����֐�

	D3DXMATRIX mtxRot, mtxTrans;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_posV.x, m_posV.y, m_posV.z);		// �s��ړ��֐�
	D3DXMatrixInverse(&mtxTrans, NULL, &mtxTrans);						// �t�s��Ɍv�Z
	D3DXMatrixMultiply(&m_mtxView, &m_mtxView, &mtxTrans);				// �s��|���Z�֐�

	// �����̔��f
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);		// �N�I�[�^�j�I���ɂ��s���]
	D3DXMatrixInverse(&mtxRot, NULL, &mtxRot);					// �t�s��Ɍv�Z
	D3DXMatrixMultiply(&m_mtxView, &m_mtxView, &mtxRot);		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �r���[�|�[�g�̓K��
	pDevice->SetViewport(&m_viewport);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProj);			// �s�񏉊����֐�

	switch (m_viewType)
	{
	case TYPE_CLAIRVOYANCE:
		// �v���W�F�N�V�����}�g���b�N�X�̍쐬(�������e)
		D3DXMatrixPerspectiveFovLH(&m_mtxProj,				// �v���W�F�N�V�����}�g���b�N�X
			D3DXToRadian(60.0f),							// ����p
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// �A�X�y�N�g��
			CAMERA_NEAR,									// �j�A
			CAMERA_FUR);									// �t�@�[
		break;

	case TYPE_PARALLEL:
		// �v���W�F�N�V�����}�g���b�N�X�̍쐬(���s���e)
		D3DXMatrixOrthoLH(&m_mtxProj,						// �v���W�F�N�V�����}�g���b�N�X
			(float)SCREEN_WIDTH * 5,							// ��
			(float)SCREEN_HEIGHT * 5,							// ����
			CAMERA_NEAR,									// �j�A
			CAMERA_FUR);									// �t�@�[
		break;

	default:
		assert(false);
		break;
	}

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);
}

//=============================================================================
// ��]�̌v�Z
// Author : �������l
// Author : YudaKaito
// �T�v : 
//=============================================================================
void CCamera::Rotate()
{
	// ���͏��̎擾
	static const float MIN_MOUSE_MOVED = 2.0f;		// ���̒l�ȏ㓮�����Ȃ��Ɣ������Ȃ�

	if (!(D3DXVec3Length(&m_rotMove) > MIN_MOUSE_MOVED) && !(D3DXVec3Length(&m_rotMove) < -MIN_MOUSE_MOVED))
	{
		return;
	}

	// �f�b�h�]�[���̐ݒ�
	if (m_rotMove.x >= -MIN_MOUSE_MOVED && m_rotMove.x <= MIN_MOUSE_MOVED)
	{
		m_rotMove.x = 0.0f;
	}
	if (m_rotMove.y >= -MIN_MOUSE_MOVED && m_rotMove.y <= MIN_MOUSE_MOVED)
	{
		m_rotMove.y = 0.0f;
	}

	/* ���w�肵�������ȏ�œ������Ă遫 */

	static const float ROTATE_MOUSE_MOVED = 0.45f;	// ��]���x

	// �ړ������̎Z�o
	D3DXVECTOR3 rollDir = (m_rotMove * (D3DX_PI / 180.0f) * ROTATE_MOUSE_MOVED);

	//m_pRoll->Moving(rollDir);

	if (m_mode == TYPE_SHOULDER && Limit_Used_Mouse())
	{
		rollDir.x = 0.0f;
	}

	// �}�E�X�̃x�N�g�����擾
	m_axisVec.y = rollDir.x;
	m_axisVec.x = -rollDir.y;

	D3DXVECTOR3 inverseVec = m_axisVec;

	D3DXVec3Normalize(&inverseVec, &inverseVec);

	m_VecGet = inverseVec;

	// X���̉�]
	{
		if (inverseVec.y != 0.0f)
		{
			if (m_Objectmode != CObject::RADAR_MODE)
			{
				// �N�I�[�^�j�I���ɂ��s���]
				D3DXMATRIX mtxRot, mtxVec;
				D3DXMatrixTranslation(&mtxVec, inverseVec.x, inverseVec.y, inverseVec.z);		// �s��ړ��֐�

				D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);							// �N�I�[�^�j�I������]�s��ɕϊ�

				D3DXVECTOR3 axis;
				D3DXVECTOR3 vecX;

				if (inverseVec.y >= 0.0f)
				{
					vecX = D3DXVECTOR3(1, 0, 0);
				}
				else if (inverseVec.y <= 0.0f)
				{
					vecX = D3DXVECTOR3(-1, 0, 0);
				}

				D3DXVec3TransformCoord(&axis, &vecX, &mtxRot);

				// �N�I�[�^�j�I���̌v�Z
				D3DXQUATERNION quaternion;
				//D3DXQuaternionRotationAxis(&quaternion, &axis, D3DXVec3Length(&m_pRoll->GetMove()));	// ��]���Ɖ�]�p�x���w��

				D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// ��]���Ɖ�]�p�x���w��

				// �N�I�[�^�j�I���̃m�[�}���C�Y
				D3DXQuaternionNormalize(&quaternion, &quaternion);

				// �N�I�[�^�j�I����K�p
				m_quaternion *= quaternion;
			}
			else
			{
				if (m_mode == TYPE_SHOULDER)
				{
					D3DXVECTOR3 axis;

					if (inverseVec.x < 0.0f)
					{
						D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
						D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, -inverseVec.x, 0.0f);
						D3DXVec3Cross(&axis, &xAxis, &zAxis);	// �O�ςŉ�]�����Z�o�B
					}
					else
					{
						D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
						D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, inverseVec.x, 0.0f);
						D3DXVec3Cross(&axis, &xAxis, &zAxis);	// �O�ςŉ�]�����Z�o�B
					}

					if (inverseVec.x != 0.0f)
					{
						// �N�I�[�^�j�I���̌v�Z
						D3DXQUATERNION quaternion;

						D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// ��]���Ɖ�]�p�x���w��

						// �N�I�[�^�j�I���̃m�[�}���C�Y
						D3DXQuaternionNormalize(&quaternion, &quaternion);

						// �N�I�[�^�j�I����K�p
						m_quaternion *= quaternion;
					}
				}
			}
		}
	}

	switch (m_mode)
	{
	case TYPE_SHOULDER:
		// Y���̉�]
	{
		D3DXVECTOR3 axis;

		if (inverseVec.x < 0.0f)
		{
			D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
			D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, 0.0f, -inverseVec.x);
			D3DXVec3Cross(&axis, &xAxis, &zAxis);	// �O�ςŉ�]�����Z�o�B
		}
		else
		{
			D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
			D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, 0.0f, inverseVec.x);
			D3DXVec3Cross(&axis, &xAxis, &zAxis);	// �O�ςŉ�]�����Z�o�B
		}

		if (inverseVec.x != 0.0f)
		{
			// �N�I�[�^�j�I���̌v�Z
			D3DXQUATERNION quaternion;

			D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// ��]���Ɖ�]�p�x���w��

			// �N�I�[�^�j�I���̃m�[�}���C�Y
			D3DXQuaternionNormalize(&quaternion, &quaternion);

			// �N�I�[�^�j�I����K�p
			m_quaternion *= quaternion;
		}
	}
	break;

	case TYPE_FREE:
		// Z���̉�]
	{
		if (m_Objectmode != CObject::RADAR_MODE)
		{
			D3DXVECTOR3 axis;

			if (inverseVec.x < 0.0f)
			{
				D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
				D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, inverseVec.x, 0.0f);
				D3DXVec3Cross(&axis, &xAxis, &zAxis);	// �O�ςŉ�]�����Z�o�B
			}
			else
			{
				D3DXVECTOR3 xAxis = D3DXVECTOR3(inverseVec.x, 0.0f, 0.0f);
				D3DXVECTOR3 zAxis = D3DXVECTOR3(0.0f, -inverseVec.x, 0.0f);
				D3DXVec3Cross(&axis, &xAxis, &zAxis);	// �O�ςŉ�]�����Z�o�B
			}

			if (inverseVec.x != 0.0f)
			{
				// �N�I�[�^�j�I���̌v�Z
				D3DXQUATERNION quaternion;

				D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// ��]���Ɖ�]�p�x���w��

				// �N�I�[�^�j�I���̃m�[�}���C�Y
				D3DXQuaternionNormalize(&quaternion, &quaternion);

				// �N�I�[�^�j�I����K�p
				m_quaternion *= quaternion;
			}
		}
	}
	break;
	}

	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
}

//=============================================================================
// �����_�̉�]
// Author : �������l
// Author : YudaKaito
// �T�v : 
//=============================================================================
void CCamera::RPosRotate()
{
	m_posR = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, m_fDistance), m_posV, m_quaternion);
}

//=============================================================================
// ���_�̉�]
// Author : �������l
// Author : YudaKaito
// �T�v : 
//=============================================================================
void CCamera::VPosRotate()
{
	m_posV = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, -m_fDistance), m_posR, m_quaternion);
}

//=============================================================================
// �J�����̎��R�ړ�
// Author : �������l
// Author : YudaKaito
// Author : �y���m��
// �T�v : 
//=============================================================================
void CCamera::FreeMove(void)
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�ړ��L�[�������ꂽ
	if (pKeyboard->GetPress(DIK_W))
	{
		MOVE_SPEED += 0.1f;
		CAMERA_MOVE_SPEED += 0.1f;
	}
	else
	{// �v���C���[�����삵�Ă��Ȃ��Ƃ���������
		MOVE_SPEED -= 0.01f;
		CAMERA_MOVE_SPEED -= 0.01f;
	}

	if (MOVE_SPEED >= 10.0f)
	{
		MOVE_SPEED = 10.0f;
	}
	if (CAMERA_MOVE_SPEED >= 10.0f)
	{
		CAMERA_MOVE_SPEED = 10.0f;
	}
	if (CAMERA_MOVE_SPEED <= 5.0f)
	{
		CAMERA_MOVE_SPEED = 5.0f;
	}
	if (MOVE_SPEED <= 5.0f)
	{
		MOVE_SPEED = 5.0f;
	}
		//��ɑO�i��������
		m_fDistance -= MOVE_SPEED;
		VPosRotate();
		m_fDistance += MOVE_SPEED;
		RPosRotate();

	D3DXVec3Normalize(&move, &move);

	if (D3DXVec3Length(&move) != 0.0f)
	{
		// ���_�ʒu�̍X�V
		m_posV = m_posV + move * CAMERA_MOVE_SPEED;
		m_posR = m_posR + move * CAMERA_MOVE_SPEED;
	}

	// �O���E���h�̎擾
	CMesh *pGround = CGame::GetGround();

	if (pGround != nullptr)
	{// ���̓����蔻��
		CPlayer3D *pPlayer = CPlayerManager::GetPlayer();

		if (pPlayer->GetCollision())
		{
			m_posV.y = pPlayer->GetPosition().y + 50.0f;

			pGround->Collision(&m_posV);
			m_posR.y = pPlayer->GetPosition().y;
		}
	}
}

//=========================================
//�@�J�����̌��z���ړ�
//	Author : �y���m��
//=========================================
void CCamera::ShoulderMove()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (m_bUp)
	{// �㏸����
		Up();
	}

	/* �ړ��L�[�������ꂽ*/
	if (pKeyboard->GetPress(DIK_W))
	{
		m_fDistance -= MOVE_SPEED;
		VPosRotate();
		m_fDistance += MOVE_SPEED;
		RPosRotate();
	}

	if (pKeyboard->GetPress(DIK_A))
	{// �ړ��L�[�������ꂽ
		D3DXVECTOR3 Pythagoras = D3DXVECTOR3(m_posV.z - m_posR.z, 0.0f, m_posV.x - m_posR.x);

		move.x += Pythagoras.x;
		move.z += -Pythagoras.z;
	}
	if (pKeyboard->GetPress(DIK_S))
	{// �ړ��L�[�������ꂽ
		m_fDistance += MOVE_SPEED;
		VPosRotate();
		m_fDistance -= MOVE_SPEED;
		RPosRotate();
	}
	if (pKeyboard->GetPress(DIK_D))
	{// �ړ��L�[�������ꂽ
		D3DXVECTOR3 Pythagoras = D3DXVECTOR3(m_posV.z - m_posR.z, 0.0f, m_posV.x - m_posR.x);

		move.x += -Pythagoras.x;
		move.z += Pythagoras.z;
	}


	if (MOVE_SPEED >= 10.0f)
	{
		MOVE_SPEED = 10.0f;
	}
	if (CAMERA_MOVE_SPEED >= 10.0f)
	{
		CAMERA_MOVE_SPEED = 10.0f;	
	}
	if (CAMERA_MOVE_SPEED <= 5.0f)
	{
		CAMERA_MOVE_SPEED = 5.0f;
	}
	if (MOVE_SPEED <= 5.0f)
	{
		MOVE_SPEED = 5.0f;
	}

	D3DXVec3Normalize(&move, &move);

	if (D3DXVec3Length(&move) != 0.0f)
	{
		// ���_�ʒu�̍X�V
		m_posV = m_posV + move * CAMERA_MOVE_SPEED;
		m_posR = m_posR + move * CAMERA_MOVE_SPEED;
	}
	CPlayer3D *pPlayer = CPlayerManager::GetRobot();
	// �O���E���h�̎擾
	CMesh *pGround = CGame::GetGround();

	D3DXVECTOR3 Result = {};

	if (pGround != nullptr)
	{
		if (!m_bUp)
		{// ���̓����蔻��
			if (pPlayer->GetCollision())
			{
				Result.y = (pPlayer->GetPosition().y + 110.0f) - m_posV.y;

				m_posV.y += Result.y;

				m_Gravity = 0;
				pGround->Collision(&m_posV);
				m_posR.y = pPlayer->GetPosition().y;
			}
			else
			{// �d�͂̓K��
			m_Gravity += 0.3f;

			// �J���������~������
			m_posV.y -= m_Gravity;
			VPosRotate();
			m_posR.y -= m_Gravity;

			pGround->Collision(&m_posV);
			}
		}
	}
}

//=========================================
// �}�E�X�̈ړ�
// Author : �������l
// Author : YudaKaito
//=========================================
void CCamera::MouseMove(void)
{
	CMouse *pMouse = CApplication::GetMouse();

	// ��]�̃x�N�g����ݒ�B
	m_Dest = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	// �N���b�N�̏���ۊ�
	bool hasLeftClick = pMouse->GetPress(CMouse::MOUSE_KEY_LEFT);
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	if (hasRightClick)
	{
		Rotate();
		VPosRotate();
	}
	if (hasLeftClick)
	{
		/*���N���b�N���Ă���Œ���]����*/
		Rotate();
		RPosRotate();
	}
}

//=========================================
// �W���C�p�b�h�̈ړ�
// Author : �y���m��
// �T�v : �W���C�p�b�h�g�p���̃J�����̐���
//=========================================
void CCamera::JoyPadMove(void)
{
	CJoypad *pJoypad = CApplication::GetJoy();
	
	if (pJoypad->GetUseJoyPad() >= 1)
	{
		// ��]�̃x�N�g����ݒ�B
		m_Dest.x = pJoypad->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0) * pJoypad->GetStick(CJoypad::JOYKEY_LEFT_STICK, 0).y * 100.0f;

		if ((pJoypad->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0) >= D3DX_PI * 0.25f && pJoypad->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0) <= D3DX_PI * 0.75f) ||
			(pJoypad->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0) >= -D3DX_PI * 0.75f && pJoypad->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0) <= -D3DX_PI * 0.25f))
		{
			// ��]�̃x�N�g����ݒ�B
			m_Dest.y = pJoypad->GetStick(CJoypad::JOYKEY_LEFT_STICK, 0).x * 100.0f;
		}
	
	Rotate();
	VPosRotate();
	}
}

//==================================================
// �r���[�|�[�g�̑傫���ݒ�
// Author : �y���m��
// ���� : ��ʍ���̍��WX,Y�A���A����
//==================================================
void CCamera::SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight)
{
	//��������
	m_viewport.X = X;				//�r���[�|�[�g�̍���X���W
	m_viewport.Y = Y;				//�r���[�|�[�g�̍���Y���W
	m_viewport.Width = fWidth;		//�r���[�|�[�g�̕�
	m_viewport.Height = fHeight;	//�r���[�|�[�g�̍���
}

//==================================================
// �r���[�|�[�g�̊g�k
// Author : �y���m��
// ���� : �J�n�ʒuX�A�J�n�ʒuY�A���A����
//==================================================
void CCamera::AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight)
{
	//------------------------------
	// ���̉��Z
	//------------------------------
	if (m_viewport.Width < SCREEN_WIDTH - 1.0f)
	{//�����X�N���[�����Ȃ�
	 //���̉��Z
		m_viewport.Width += fWidth;

		if (m_viewport.X > 0)
		{//�r���[�|�[�g�̍��オ��ʂ̍�����傫���Ȃ�
			m_viewport.X += X;	//�r���[�|�[�g�̍�����W���ړ�
		}
	}

	//------------------------------
	// �����̉��Z
	//------------------------------
	if (m_viewport.Height < SCREEN_HEIGHT - 1.0f)
	{//�����X�N���[�����Ȃ�
	 //�����̉��Z
		m_viewport.Height += fHeight;

		if (m_viewport.Y > 0)
		{//�r���[�|�[�g�̍��オ��ʂ̍�����傫���Ȃ�
			m_viewport.Y += Y;	//�r���[�|�[�g�̍�����W���ړ�
		}
	}
}

//=========================================
//	�ʏ�J�����̕`�揈��
//	Author: �y���m��
//=========================================
void CCamera::UpdateNormal()
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	JoyPadMove();	// �W���C�p�b�h�ړ�

	m_mode = (CCamera::CAMERA_TYPE)CPlayerManager::GetMode();

	// ��Ԃ��ƂɈړ����@��ς���
	switch (m_mode)
	{
	case TYPE_FREE:
		MouseMove();		// �}�E�X�ړ�
		FreeMove();			// �ړ�
		break;

	case TYPE_SHOULDER:
		if (m_bWork)
		{// �}�E�X�̈ړ����\�ɂ���
			MouseMove();
		}

		ShoulderMove();	// ���z�����[�h

		if (!m_bWork)
		{// �J�������[�N
			CameraWork(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f));
		}

		break;
	}

	//==================================================================================

	// �I�u�W�F�N�g�̎擾
	CObject *object = CObject::GetObjectTop();

	D3DXVECTOR3 PlayerPos = {};
	D3DXVECTOR3	PlayerRot = {};

	//�v���C���[�̍��W���擾
	while (object)
	{
		if (object != nullptr)
		{
			CObject::EObjType ObjType = object->GetObjectType();

			if (ObjType == CObject::OBJECT_PLAYER)
			{
				PlayerPos = object->GetPosition();
				PlayerRot = object->GetRot();
				break;
			}
		}
		object = object->GetObjectNext();
	}

	if (object != nullptr)
	{
		// �G���^�[�L�[�������ꂽ
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			switch (m_mode)
			{
			case TYPE_FREE:
				m_mode = TYPE_SHOULDER;
				break;

			case TYPE_SHOULDER:
				m_bWork = false;

				m_mode = TYPE_FREE;

				// 1.0f�������鏈��
				m_posV.y += 100.0f;
				VPosRotate();
				m_posR.y += 100.0f;

				// ��s�C�x���g�J�n
				m_event = EVENT_FLY;

				break;
			}
		}
	}
}

//=========================================
//	���[�_�[�p�J�����̏���
//	Author : �y���m��
//=========================================
void CCamera::UpdateRadar()
{
	CPlayer3D *pPlayer = CPlayerManager::GetPlayer();

	CMouse *pMouse = CApplication::GetMouse();

	// �N���b�N�̏���ۊ�
	bool hasLeftClick = pMouse->GetPress(CMouse::MOUSE_KEY_LEFT);
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	// ��]�̃x�N�g����ݒ�B
	m_Dest = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		m_posV = D3DXVECTOR3(PlayerPos.x, m_posV.y, PlayerPos.z);
		m_posR = D3DXVECTOR3(PlayerPos.x, m_posR.y, PlayerPos.z);
	}

	if (hasRightClick)
	{
		m_quaternion = D3DXQUATERNION(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);

		Rotate();
		RPosRotate();
	}
}

//=========================================
//	��s�C�x���g�̏���
//	Author : �y���m��
//=========================================
void CCamera::FlightEvent()
{
	// ��s���J�E���g
	m_nCntFly++;

	// �J�������㏸������
	m_posV.y += 5.0f;
	VPosRotate();
	m_posR.y += 5.0f;

	D3DXQUATERNION Dest = D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f) - m_quaternion;
	m_quaternion += Dest * 0.02f;

	if (m_nCntFly >= 120)
	{
		m_nCntFly = 0;
		m_event = EVENT_NORMAL;
	}
}

//=========================================
// �}�E�X�𗘗p�����J�����̐���
// Author : �y���m��
//=========================================
bool CCamera::Limit_Used_Mouse()
{
	CMouse *pMouse = CApplication::GetMouse();
	D3DXVECTOR3 axis = {};

	// �N���b�N�̏���ۊ�
	bool hasLeftClick = pMouse->GetPress(CMouse::MOUSE_KEY_LEFT);
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	if (hasRightClick)
	{
		if (m_rotMove.x > 0.0f)
		{
			m_MouseMove++;
		}
		else if (m_rotMove.x < 0.0f)
		{
			m_MouseMove--;
		}

		// �������̏���A������̏��
		if (m_MouseMove >= 5.0f || m_MouseMove <= -10.0f)
		{
			return true;
		}
	}

	return false;
}

//=========================================
// �J�������[�N�̏���
// Author : �y���m��
//=========================================
void CCamera::CameraWork(D3DXQUATERNION que)
{
	if (m_quaternion == D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f))
	{
		return;
	}

	m_quaternion = GetQuaternion();

	// �N�H�[�^�j�I���̃f�X�g
	D3DXQUATERNION Result = m_quaternion - que;

	D3DXQuaternionNormalize(&Result, &Result);

	m_quaternion += Result * 0.1f;

	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	MouseMove();

	m_nCntCameraWork++;

	if (m_nCntCameraWork >= 120)
	{
		m_bWork = true;
		m_nCntCameraWork = 0;
	}
}

//=========================================
// �㏸����
// Author : �y���m��
//=========================================
void CCamera::Up()
{
	m_nCntCameraWork++;

	// �J�������㏸������
	m_posV.y += 10.0f;
	VPosRotate();
	m_posR.y += 10.0f;

	if (m_nCntCameraWork >= 60)
	{
		m_bUp = false;
		m_nCntCameraWork = 0;
	}
}