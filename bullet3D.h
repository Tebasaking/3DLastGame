//=========================================
//
//	bullet3D.h
//	Author:�y���m��
//
//=========================================
#ifndef _BULLET3D_H_		//���̃}�N����`������ĂȂ�������
#define _BULLET3D_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "object3D.h"
#include "object.h"
#include "model.h"
#include "missile_alert.h"

class CModel3D;
class CObject;
class CAlert;
class CBullet3D : public CObject
{
public:
	CBullet3D();							//�R���X�g���N�^
	virtual ~CBullet3D() override;			//�f�X�g���N�^

	//����������
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	// �X�V����
	void Update(void) override;
	// �`�揈��
	void Draw(void) override;
	// �I������
	void Uninit(void) override;
	// �e�̐���
	void BulletMove(void);
	// ���b�N�I������
	D3DXVECTOR3 LockOn();

	// �T���J�E���g�̖ڕW��ݒ�
	void SetSearchValue(int val) { m_SearchValue = val; }
	// �p�x�̐ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_ShooterRot = rot; }
	void SetQue(D3DXQUATERNION que) { m_quaternion = que; }
	// �~�T�C���̑��x�̎擾
	float GetSpeed() { return m_MissileSpeed; }

	// �N�H�[�^�j�I�����I�C���[�p�ɕϊ�����
	D3DXVECTOR3 QuaternionChange();

	// �N���G�C�g����
	// ���� : ���炷���W�̑傫���A�N�H�[�^�j�I���A�ڕW�I�u�W�F�N�g���A�ˎ�I�u�W�F�N�g���A�U���̃^�C�~���O���J�E���g����ڕW�l
	static CBullet3D* Create(const D3DXVECTOR3 &pos, const D3DXQUATERNION &quaternion, CObject *object, CObject *Shooter,int val);

private:
	CAlert*					m_pAlert;					// �~�T�C���A���[�g
	CModel3D*				m_pModel;					// ���f���̍ő吔
	CObject*				m_TargetObj;				// �^�[�Q�b�g�I�u�W�F�N�g
	D3DXQUATERNION			m_quaternion;				// �N�H�[�^�j�I��
	D3DXVECTOR3				m_Rot;						// �I�C���[�p
	D3DXVECTOR3				m_pos;						// ���W
	D3DXVECTOR3				m_TargetPos;				// �^�[�Q�b�g�̍��W
	D3DXVECTOR3				m_posOld;					// �Â����W
	D3DXVECTOR3				m_posTarget;				// �ڕW�̍��W
	D3DXVECTOR3				m_ShooterRot;				// �ˎ�̊p�x
	D3DXVECTOR3				m_FllowRot;					// �ǔ����Ă������
	D3DXVECTOR3				m_move;						// �i�s������ۑ�����
	D3DXMATRIX				m_WorldMtx;					// ���[���h�}�g���b�N�X
	float					m_length;					// �e�ƖڕW�l�̋���
	float					m_MissileSpeed;				// �~�T�C���̑��x
	int						m_FrontMoveCnt;				// �^��������ԃJ�E���g
	int						m_SearchCnt;				// �T���J�E���g
	int						m_SearchValue;				// �T���J�E���g�̖ڕW�l
	int						m_MissileCnt;				// �~�T�C���J�E���g
};

#endif