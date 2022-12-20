//=========================================
//
//	player3D.h
//	Author:�y���m��
//
//=========================================
#ifndef _PLAYER_H_		//���̃}�N����`������ĂȂ�������
#define _PLAYER_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`
#define MAX_PARTS (1)

#include "main.h"
#include "object3D.h"
#include "model.h"
#include "motion_model3D.h"

class CModel3D;
class CPlayer3D : public CMotionModel3D
{
public:
	enum PLAYER_MODE
	{
		MODE_FLY = 0,
		MODE_ROBOT,
		MODE_MAX
	};
	
	CPlayer3D();			//�R���X�g���N�^
	virtual ~CPlayer3D() override;			//�f�X�g���N�^

	//����������
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	// �X�V����
	void Update(void) override;
	// �`�揈��
	void Draw(void) override;
	// �I������
	void Uninit(void) override;
	// �N���G�C�g����
	static CPlayer3D* Create(const D3DXVECTOR3 &pos);
	void SetMode(PLAYER_MODE mode) { m_mode = mode; }
	
	// ���b�N�I������
	void LockOn(void);
	// ��]����
	void Rotate(void);
	// �e�̔��ˏ���
	void Bullet(void);

	// �X�V����(��s)
	void UpdateFly(void);
	// �X�V����(�l�^)
	void UpdateRob(void);

	//=========================================
	// ��s��Ԃł����g��Ȃ�����
	//=========================================
	// �p�����䏈��
	void Attitude(void);

	const D3DXQUATERNION& GetQuaternion() const { return m_quaternion; }

private:
	float m_scale;										// �傫��
	D3DXVECTOR3 m_size;									// �g�嗦

	CModel3D*				m_apModel[MAX_PARTS];		// ���f���̍ő吔
	CModel3D*				m_pRobot;					// ���{�b�g�̃��f���ǂݍ���
	D3DXMATRIX				m_WorldMtx;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_MouseMove;				// �}�E�X�̈ړ�����
	PLAYER_MODE				m_mode;						// ���[�h
};

#endif