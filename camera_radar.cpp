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

#include "camera_radar.h"
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
#include "sound.h"
#include "utility.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
const float CCameraRadar::CAMERA_NEAR = 30.0f;		// �j�A
const float CCameraRadar::CAMERA_FUR = 100000000.0f;	// �t�@�[

//=============================================================================
// �R���X�g���N�^
// Author	: �������l
// �T�v		: �C���X�^���X�������ɍs������
//=============================================================================
CCameraRadar::CCameraRadar() :
	m_viewType(TYPE_CLAIRVOYANCE),							// ���e���@�̎��
	m_nCntFly(0),
	m_fRotMove(0.0f)										// �ړ�����
{
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CCameraRadar::~CCameraRadar()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�ƒ����_�̊Ԃ̋������Z�o����
//=============================================================================
HRESULT CCameraRadar::Init(D3DXVECTOR3 pos)
{
	m_posV = D3DXVECTOR3(0.0f, 5000.0f, -150.0f);
	m_posR = D3DXVECTOR3(0.0f, 5000.0f, 0.0f);
	m_quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_viewType = TYPE_PARALLEL;

	// ���_�ƒ����_�̋���
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	m_mode = TYPE_FREE;

	m_nCntMoveSound = 10000;

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �I��
//=============================================================================
void CCameraRadar::Uninit(void)
{
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V
//=============================================================================
void CCameraRadar::Update(void)
{
	// �v���C���[�̈ʒu���擾���āAX �� Y �̍��W�����킹��B
	CPlayer3D *pPlayer = CPlayerManager::GetPlayer();
	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		m_posV = D3DXVECTOR3(PlayerPos.x, m_posV.y, PlayerPos.z);
		m_posR = D3DXVECTOR3(PlayerPos.x, m_posR.y, PlayerPos.z);
	}

	CMouse *pMouse = CApplication::GetMouse();

	// ��]�̃x�N�g����ݒ�B
	m_Dest = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	// �E�N���b�N���A��]���s���B
	if (pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT))
	{
		m_quaternion = D3DXQUATERNION(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);

		Rotate();
		RPosRotate();
	}

	D3DXVECTOR3 Result = m_Dest - m_rotMove;
	m_rotMove += Result * 0.25f;

	// ����������������]
	m_quaternion += (m_Destquaternion - m_quaternion) * 0.1f;
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	// �f�o�b�O�p
	CDebugProc::Print("=========== camera_radar ===========\n");
	CDebugProc::Print("�J�����̍��W : (%.1f,%.1f,%.1f) \n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("�J�����̉�] : (%.2f,%.2f,%.2f,%.2f) \n", m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
	CDebugProc::Print("====================================\n");
}

//=============================================================================
// ��]�̌v�Z
// Author : �������l
// Author : YudaKaito
// �T�v : 
//=============================================================================
void CCameraRadar::Rotate()
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

	// �}�E�X�̃x�N�g�����擾
	m_axisVec.y = rollDir.x;
	m_axisVec.x = -rollDir.y;

	D3DXVECTOR3 inverseVec = m_axisVec;

	D3DXVec3Normalize(&inverseVec, &inverseVec);

	m_VecGet = inverseVec;

	// X���̉�]
	CPlayer3D *pPlayer = CPlayerManager::GetPlayer();

	if (pPlayer != nullptr)
	{
		D3DXQUATERNION PlayerQua = pPlayer->GetQuaternion();

		m_quaternion = D3DXQUATERNION(m_quaternion.x, PlayerQua.x, m_quaternion.z, m_quaternion.w);
	}

	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&m_Destquaternion, &m_Destquaternion);
}

//=========================================
// �}�E�X�̈ړ�
// Author : �������l
// Author : YudaKaito
//=========================================
void CCameraRadar::MouseMove(void)
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
void CCameraRadar::JoyPadMove(void)
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
// �}�E�X�𗘗p�����J�����̐���
// Author : �y���m��
//=========================================
bool CCameraRadar::Limit_Used_Mouse()
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
