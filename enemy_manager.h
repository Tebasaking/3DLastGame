//=========================================
//
//	enemy_manage.h
//	Author:�y���m��
//
//=========================================
#ifndef _ENEMY_MANAGE_H_		//���̃}�N����`������ĂȂ�������
#define _ENEMY_MANAGE_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "object.h"
#include <vector>

#include "motion_model3D.h"

class CEnemy;
class CEnemy_Manager
{
public:
	typedef struct
	{
		std::vector<CEnemy*>  m_EnemyList;
	}Wave;

	CEnemy_Manager();			//�R���X�g���N�^
	~CEnemy_Manager();			//�f�X�g���N�^

	// ����������
	HRESULT Init();
	// �X�V����
	void Update(void);
	// �`�揈��
	void Draw(void);
	// �I������
	void Uninit(void);
	// �N���G�C�g����
	static CEnemy_Manager *Create(void);
	// �t�@�C����ǂݍ��ޏ���
	void LoadFile();

private:
	std::vector<Wave> m_Wave;
};

#endif