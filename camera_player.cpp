//**************************************************************************************************
//
// �J��������(camera.h)
// Auther�F�������l
// Author : ���c�C�s
// Author : �y���m��
// �T�v : �J�����̐ݒ菈��
//
//**************************************************************************************************

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "camera_player.h"
#include "application.h"
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
#include "sound.h"
#include "utility.h"

//=============================================================================
// �R���X�g���N�^
// Author	: �������l
// �T�v		: �C���X�^���X�������ɍs������
//=============================================================================
CCameraPlayer::CCameraPlayer() :
	m_rotMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),				// �ړ�����
	m_fRotMove(0.0f),										// �ړ�����
	m_nCntCameraWork(0),
	m_Gravity(0),
	m_nCntFly(0)
{
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CCameraPlayer::~CCameraPlayer()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�ƒ����_�̊Ԃ̋������Z�o����
//=============================================================================
HRESULT CCameraPlayer::Init(D3DXVECTOR3 pos)
{
	CCamera::Init(pos);

	m_posR = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
	m_quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;

	// ���_�ƒ����_�̋���
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	m_mode = TYPE_FREE;

	m_nCntMoveSound = 10000;

	m_bUp = false;
	m_bWork = false;

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �I��
//=============================================================================
void CCameraPlayer::Uninit(void)
{
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V
//=============================================================================
void CCameraPlayer::Update(void)
{
	if (m_event == EVENT_NORMAL)
	{
		// �L�[�{�[�h�̎擾
		CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

		JoyPadMove();	// �W���C�p�b�h�ړ�

		m_mode = (CCameraPlayer::CAMERA_TYPE)CPlayerManager::GetMode();

		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
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

		D3DXVECTOR3 Result = m_Dest - m_rotMove;
		m_rotMove += Result * 0.25f;
	}
	else
	{
		FlightEvent();
	}

	m_quaternion += (m_Destquaternion - m_quaternion) * 0.1f;
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	CDebugProc::Print("camera_player\n");
	CDebugProc::Print("�J�����̍��W : (%f,%f,%f) \n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("�J�����̉�] : (%f,%f,%f,%f) \n", m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
}

//=============================================================================
// ��]�̌v�Z
// Author : �������l
// Author : YudaKaito
// �T�v : 
//=============================================================================
void CCameraPlayer::Rotate()
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

				D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// ��]���Ɖ�]�p�x���w��

				// �N�I�[�^�j�I���̃m�[�}���C�Y
				D3DXQuaternionNormalize(&quaternion, &quaternion);

				// �N�I�[�^�j�I����K�p
				m_Destquaternion *= quaternion;
				D3DXQuaternionNormalize(&m_Destquaternion, &m_Destquaternion);
			}
		}

		if (m_Objectmode == CObject::RADAR_MODE)
		{// ���[�_�[���[�h�̎�
			CPlayer3D *pPlayer = CPlayerManager::GetPlayer();

			if (pPlayer != nullptr)
			{
				D3DXQUATERNION PlayerQua = pPlayer->GetQuaternion();

				m_quaternion = D3DXQUATERNION(m_quaternion.x, PlayerQua.x, m_quaternion.z, m_quaternion.w);
			}
		}
	}

	switch (m_mode)
	{
	case TYPE_SHOULDER:
		// Y���̉�]
	{
		D3DXVECTOR3 axis;
		// �N�I�[�^�j�I���ɂ��s���]
		D3DXMATRIX mtxRot, mtxVec;

		D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);
		D3DXVec3TransformCoord(&axis, &D3DXVECTOR3(1, 0, 0), &mtxRot);

		D3DXVECTOR3 vecX;

		if (inverseVec.x >= 0.0f)
		{
			vecX = D3DXVECTOR3(0, -1, 0);
		}
		else if (inverseVec.x <= 0.0f)
		{
			vecX = D3DXVECTOR3(0, 1, 0);
		}

		D3DXVec3TransformCoord(&axis, &vecX, &mtxRot);

		if (inverseVec.x != 0.0f)
		{
			// �N�I�[�^�j�I���̌v�Z
			D3DXQUATERNION quaternion;

			D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// ��]���Ɖ�]�p�x���w��

			// �N�I�[�^�j�I���̃m�[�}���C�Y
			m_Destquaternion *= quaternion;
		}
	}
	break;

	case TYPE_FREE:
		// Z���̉�]
	{
		if (m_Objectmode != CObject::RADAR_MODE)
		{

			D3DXVECTOR3 axis;
			// �N�I�[�^�j�I���ɂ��s���]
			D3DXMATRIX mtxRot, mtxVec;

			D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);
			D3DXVec3TransformCoord(&axis, &D3DXVECTOR3(0, 0, 1), &mtxRot);

			D3DXVECTOR3 vecX;

			if (inverseVec.x >= 0.0f)
			{
				vecX = D3DXVECTOR3(0, 0, 1);
			}
			else if (inverseVec.x <= 0.0f)
			{
				vecX = D3DXVECTOR3(0, 0, -1);
			}

			D3DXVec3TransformCoord(&axis, &vecX, &mtxRot);

			if (inverseVec.x != 0.0f)
			{
				// �N�I�[�^�j�I���̌v�Z
				D3DXQUATERNION quaternion;

				D3DXQuaternionRotationAxis(&quaternion, &axis, 0.03f);	// ��]���Ɖ�]�p�x���w��

				// �N�I�[�^�j�I���̃m�[�}���C�Y
				D3DXQuaternionNormalize(&quaternion, &quaternion);

				m_Destquaternion *= quaternion;
				D3DXQuaternionNormalize(&m_Destquaternion, &m_Destquaternion);
			}
		}
	}
	break;
	}

	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&m_Destquaternion, &m_Destquaternion);
}

//=============================================================================
// �J�����̎��R�ړ�
// Author : �������l
// Author : YudaKaito
// Author : �y���m��
// �T�v : 
//=============================================================================
void CCameraPlayer::FreeMove(void)
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{

		m_nCntMoveSound++;

		if (m_nCntMoveSound >= 60 * 19)
		{
			CSound::PlaySound(CSound::SOUND_SE_FLIGHT);
			m_nCntMoveSound = 0;
		}
	}

	//�ړ��L�[�������ꂽ
	if (pKeyboard->GetPress(DIK_W))
	{// ��������
		MOVE_SPEED += 0.1f;
		CAMERA_MOVE_SPEED += 0.1f;
	}
	else
	{// �v���C���[�����삵�Ă��Ȃ��Ƃ���������
		MOVE_SPEED -= 0.1f;
		CAMERA_MOVE_SPEED -= 0.1f;
	}

	if (MOVE_SPEED >= 20.0f)
	{
		MOVE_SPEED = 20.0f;
	}
	if (CAMERA_MOVE_SPEED >= 20.0f)
	{
		CAMERA_MOVE_SPEED = 20.0f;
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
void CCameraPlayer::ShoulderMove()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// SE�̒�~
	CSound::StopSound(CSound::SOUND_SE_FLIGHT);

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

				m_Gravity = 0.0f;

				// �n�ʂ̓����蔻��
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
void CCameraPlayer::MouseMove(void)
{
	CMouse *pMouse = CApplication::GetMouse();

	// ��]�̃x�N�g����ݒ�B
	m_Dest = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	// �N���b�N�̏���ۊ�
	bool hasRightClick = pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT);

	if (hasRightClick)
	{
		Rotate();
		VPosRotate();
	}
}

//=========================================
// �W���C�p�b�h�̈ړ�
// Author : �y���m��
// �T�v : �W���C�p�b�h�g�p���̃J�����̐���
//=========================================
void CCameraPlayer::JoyPadMove(void)
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

//=========================================
//	��s�C�x���g�̏���
//	Author : �y���m��
//=========================================
void CCameraPlayer::FlightEvent()
{
	// ��s���J�E���g
	m_nCntFly++;

	// �J�������㏸������
	m_posV.y += 5.0f;
	VPosRotate();
	m_posR.y += 5.0f;

	D3DXQUATERNION Dest = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f) - m_quaternion;
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
bool CCameraPlayer::Limit_Used_Mouse()
{
	CMouse *pMouse = CApplication::GetMouse();
	D3DXVECTOR3 axis = {};

	// �N���b�N�̏���ۊ�
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
void CCameraPlayer::CameraWork(D3DXQUATERNION que)
{
	m_quaternion = GetQuaternion();

	if (m_quaternion.w >= 0.99999f)
	{
		return;
	}

	// �N�H�[�^�j�I���̃f�X�g
	D3DXQUATERNION Result = m_quaternion - que;

	D3DXQuaternionNormalize(&Result, &Result);

	m_Destquaternion += Result;

	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&m_Destquaternion, &m_Destquaternion);

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
void CCameraPlayer::Up()
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