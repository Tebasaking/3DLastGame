//=========================================
//
//	player3D.h
//	Author:�y���m��
//
//=========================================
#ifndef _ENEMY3D_H_		//���̃}�N����`������ĂȂ�������
#define _ENEMY3D_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#define MAX_PARTS (1)

#include "main.h"
#include "object.h"

#include "motion_model3D.h"

class CModel3D;
class CEnemy :public CMotionModel3D
{
public:
	//�I�u�W�F�N�g�̎��
	enum EnemyState
	{
		ENEMY_IDOL,			// �G�l�~�[�̑ҋ@���
		ENEMY_WARNNING,		// �G�l�~�[���v���C���[�𔭌��������
							//����������ԂƂ�����肽�����Ǘ]�T������΁B
	};

	CEnemy();			//�R���X�g���N�^
	virtual ~CEnemy() override;			//�f�X�g���N�^

	//����������
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;
	//�I������
	void Uninit(void) override;
	//���S����
	void Death(void);
	//�N���G�C�g����
	static CEnemy* Create(const D3DXVECTOR3 &pos);

	//�ԍ��ݒ菈��
	void SetNumber(int Number) { m_Number = Number; }
	//�ԍ��擾����
	int GetNumber() { return m_Number; }

	CModel3D *GetModel() { return m_apModel[0]; }

private:
	float m_scale;										// �傫��

	CModel3D*				m_apModel[MAX_PARTS];		// ���f���̍ő吔
	D3DXVECTOR3				m_pos;						// ���W
	D3DXVECTOR3				m_rot;						// ��]
	EnemyState				m_state;					// ���
};

#endif