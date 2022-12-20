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

//*****************************************************************************
// �萔��`
//*****************************************************************************
const float CCamera::CAMERA_NEAR = 1.0f;	// �j�A
const float CCamera::CAMERA_FUR = 100000000.0f;	// �t�@�[

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CCamera::CCamera() :
	m_posV(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// ���_
	m_posR(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// �����_
	m_vecU(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// ������x�N�g��
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),					// ����
	m_rotMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),			 	// �ړ�����
	m_quaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f)),	// �N�I�[�^�j�I��
	m_viewType(TYPE_CLAIRVOYANCE),							// ���e���@�̎��
	m_fDistance(0.0f),										// ���_���璍���_�܂ł̋���
	m_fRotMove(0.0f)										// �ړ�����
{
	m_mtxWorld = {};	// ���[���h�}�g���b�N�X
	m_mtxProj = {};		// �v���W�F�N�V�����}�g���b�N�X
	m_mtxView = {};		// �r���[�}�g���b�N�X
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
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -150.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �Œ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_�ƒ����_�̋���
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	// �p�x�̎Z�o
	m_rot.y = 0.0f;
	m_rot.x = 0.0f;
	m_rot.z = 0.0f;

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
	MouseMove();	// �}�E�X�ړ�

	m_mode = (CCamera::CAMERA_TYPE)CPlayerManager::GetMode();

	// ��Ԃ��ƂɈړ����@��ς���
	switch (m_mode)
	{
	case TYPE_FREE:
		FreeMove();		// �ړ�
		break;
	case TYPE_SHOULDER:
		ShoulderMove();	// ���z�����[�h
		break;
	}

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �G���^�[�L�[�������ꂽ
	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		// m_mode��FREE�̎���SHOULDER�ɁASHOULDER�̎���FREE�ɂ���
		m_mode = (m_mode == TYPE_FREE) ? TYPE_SHOULDER : TYPE_FREE;
	}
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
			(float)SCREEN_WIDTH,							// ��
			(float)SCREEN_HEIGHT,							// ����
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
	static const float MIN_MOUSE_MOVED = 3.0f;		// ���̒l�ȏ㓮�����Ȃ��Ɣ������Ȃ�

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
			// �N�I�[�^�j�I���ɂ��s���]
			D3DXMATRIX mtxRot, mtxVec;
			D3DXMatrixTranslation(&mtxVec, inverseVec.x, inverseVec.y, inverseVec.z);		// �s��ړ��֐�
			D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);			// �N�I�[�^�j�I������]�s��ɕϊ�
			D3DXMatrixMultiply(&mtxVec, &mtxVec, &mtxRot);					// �s��|���Z�֐�

			D3DXVECTOR3 axis;
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, mtxVec._42, 0.0f);
			D3DXVECTOR3 vecY = m_posR - m_posV;		// ���_���璍���_�̋����x�N�g��
			D3DXVec3Normalize(&vecY, &vecY);		// ��]��

			D3DXVec3Cross(&axis, &vec, &vecY);	// �O�ςŉ�]�����Z�o�B

			// �N�I�[�^�j�I���̌v�Z
			D3DXQUATERNION quaternion;
			//D3DXQuaternionRotationAxis(&quaternion, &axis, D3DXVec3Length(&m_pRoll->GetMove()));	// ��]���Ɖ�]�p�x���w��

			D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// ��]���Ɖ�]�p�x���w��

			// �N�I�[�^�j�I���̃m�[�}���C�Y
			D3DXQuaternionNormalize(&quaternion, &quaternion);

			// �N�I�[�^�j�I����K�p
			m_quaternion *= quaternion;
		}
	}

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
}

//=========================================
//�@�J�����̌��z���ړ�
//	Author : �y���m��
//=========================================
void CCamera::ShoulderMove()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

}

//=========================================
// �}�E�X�̈ړ�
// Author : �������l
// Author : YudaKaito
// �T�v	  :  
//=========================================
void CCamera::MouseMove(void)
{
	CMouse *pMouse = CApplication::GetMouse();

	// ��]�̃x�N�g����ݒ�B
	m_rotMove = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	// �N���b�N�̏���ۊ�
	bool hasLeftClick = pMouse->GetPress(CMouse::MOUSE_KEY_LEFT);
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	if (hasLeftClick)
	{
	/*���N���b�N���Ă���Œ���]����*/
		Rotate();
		RPosRotate();
	}

	if (hasRightClick)
	{
		Rotate();
		VPosRotate();
	}
}