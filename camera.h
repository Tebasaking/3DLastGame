//**************************************************************************************************
//
// �J��������(camera.h)
// Auther�F�������l
// �T�v : �J�����̐ݒ菈��
//
//**************************************************************************************************
#ifndef _CAMERA_H_			// ���̃}�N����`������ĂȂ�������
#define _CAMERA_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//***************************************************************************
// �C���N���[�h
//***************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CMove;

//=============================================================================
// �J�����N���X
// Author : �������l
// �T�v : �J�����ݒ���s���N���X
//=============================================================================
class CObject;
class CCamera
{
private:
	//--------------------------------------------------------------------
	// �萔��`
	//--------------------------------------------------------------------
	static const float CAMERA_NEAR;			// �j�A�̍ő�l
	static const float CAMERA_FUR;			// �t�@�[�̍ő�l

public:
	//=============================================================================
	// ���e���@�̃^�C�v�񋓌^
	// Author : �������l
	// �T�v : ���e���@�̎��
	//=============================================================================
	enum VIEW_TYPE
	{
		TYPE_CLAIRVOYANCE = 0,		// �������e
		TYPE_PARALLEL,				// ���s���e
		MAX_VIEW_TYPE,				// �^�C�v�̍ő吔
	};

	enum CAMERA_TYPE
	{
		TYPE_FREE,					// ���R�ɃJ�����𓮂�������
		TYPE_SHOULDER,				// ���z�����_���
		MAX_CAMERA_TYPE,			// �J�����^�C�v�̍ő吔
	};

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CCamera();
	~CCamera();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init();						// ������
	void Uninit(void);												// �I��
	void Update(void);												// �X�V
	void UpdateNormal(void);										// �ʏ폈��
	void UpdateRadar(void);											// ���[�_�[�̏���
	void Set();														// �ݒ�
	void SetViewType(VIEW_TYPE type) { m_viewType = type; }			// �^�C�v�̐ݒ�
	void SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	//�r���[�|�[�g�̑傫���ݒ�
	void AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	//�r���[�|�[�g�̊g�k
	// �I�u�W�F�N�g�̃��[�h�̐ݒ�
	void SetObjMode(CObject::Object_mode mode) { m_Objectmode = mode; }
	
	//--------------------------------------------------------------------
	// �Q�b�^�\
	//--------------------------------------------------------------------
	const D3DXVECTOR3 GetRot() { return m_rot; }					// �p�x�̎擾
	const D3DXVECTOR3 GetPosR() { return m_posR; }					// �����_�̎擾
	const D3DXVECTOR3 GetPosV() { return m_posV; }					// ���_�̎擾
	const D3DXVECTOR3 GetVec() { return m_VecGet;}					// �}�E�X�x�N�g���̎擾
	D3DXMATRIX GetView() { return m_mtxView; }						// �J�����̏��̎擾
	D3DXMATRIX GetProjection() { return m_mtxProj; }				// �J�����̏��̎擾
	CAMERA_TYPE GetMode() { return m_mode; }						// �J�����̃��[�h���̎擾
	D3DVIEWPORT9 GetViewport() { return m_viewport; }				//�r���[�|�[�g�̎擾
	CObject::Object_mode GetObjType() { return m_Objectmode; }
	const D3DXQUATERNION GetQuaternion() { return m_quaternion; }	// ���_�p�x�̎擾

private:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void Rotate();					// ��]
	void RPosRotate();				// ��]
	void VPosRotate();				// ��]
	void FreeMove(void);			// �ʏ�ړ�
	void ShoulderMove(void);		// ���z�����̈ړ� 
	void MouseMove(void);			// �}�E�X�ړ�����]�ɑ��
	void JoyPadMove(void);			// �W���C�p�b�h�ړ�����]�ɑ��

//--------------------------------------------------------------------
// �����o�ϐ�
//--------------------------------------------------------------------
	CMove				*m_pRoll;			// �ړ��N���X�̃C���X�^���X(�p�x)
	D3DXQUATERNION		m_quaternion;		// �N�I�[�^�j�I��
	D3DXQUATERNION		m_Destquaternion;	// �N�I�[�^�j�I���̃f�X�g
	D3DXMATRIX			m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			m_mtxProj;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			m_mtxView;			// �r���[�}�g���b�N�X
	D3DXVECTOR3			m_posV;				// ���_
	D3DXVECTOR3			m_posR;				// �����_
	D3DXVECTOR3			m_vecU;				// ������x�N�g��
	D3DXVECTOR3			m_rot;				// ����
	D3DXVECTOR3			m_rotMove;			// �ړ�����
	D3DXVECTOR3			m_VecGet;			// �}�E�X�̃x�N�g��
	VIEW_TYPE			m_viewType;			// ���e�̎��
	CAMERA_TYPE			m_mode;				// �J�����̃��[�h
	D3DVIEWPORT9		m_viewport;			// �r���[�|�[�g
	CObject::Object_mode m_Objectmode;		// �I�u�W�F�N�g�̃��[�h
	float				m_fDistance;		// ���_���璍���_�܂ł̋���
	float				m_fRotMove;			// �ړ�����

	float				CAMERA_MOVE_SPEED = 0.5f;
	float				MOVE_SPEED = 5.0f;

	D3DXVECTOR3			m_axisVec;			// ��]�����̃x�N�g��
	int nRotateType = -1;
};

#endif
