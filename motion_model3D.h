//=============================================================================
//
// 3D���[�V�����L�����N�^�[�N���X(motion_model3D.h)
// Author : �������l
// �T�v : 3D�v���C���[�������s��
//
//=============================================================================
#ifndef _MOTION_MODEL3D_H_			// ���̃}�N����`������ĂȂ�������
#define _MOTION_MODEL3D_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CMotion;

//=============================================================================
// 3D�v���C���[�N���X
// Author : �������l
// �T�v : 3D�v���C���[�������s���N���X
//=============================================================================
class CMotionModel3D : public CObject
{
public:
	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CMotionModel3D *Create();			// 3D�v���C���[�̐���

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CMotionModel3D();
	~CMotionModel3D();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	void Uninit() override;															// �I��
	void Update() override;															// �X�V
	void Draw() override;															// �`��

	void SetPosOld(const D3DXVECTOR3 &posOld) { m_posOld = posOld; }				// �ߋ��ʒu�̃Z�b�^�[


	D3DXVECTOR3 GetPosOld() { return m_posOld; }									// �ߋ��ʒu�̃Q�b�^�[

	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }				// ���[���h�}�g���b�N�X�̃Z�b�^�[
	D3DXMATRIX *GetMtxWorld() { return &m_mtxWorld; }								// ���[���h�}�g���b�N�X�̃Q�b�^�[
	void SetMotion(const char *pName);												// ���[�V�������̐ݒ�
	void SetMotion(const char *pName, const int nNumMotion);						// ���[�V�������̐ݒ�(�I�[�o�[���[�h)
	CMotion *GetMotion() { return m_pMotion; }										// ���[�V�������̎擾

	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation() { return m_rot; }

protected:
	D3DXVECTOR3		m_pos;															// �ʒu
	D3DXQUATERNION	m_quaternion;													// �N�I�[�^�j�I��

private:
	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	CMotion			*m_pMotion;				// ���[�V�����C���X�^���X
	D3DXMATRIX		m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_posOld;				// �ߋ��ʒu
	D3DXVECTOR3		m_size;					// �傫��
	D3DXVECTOR3		m_rot;					// ����
};

#endif