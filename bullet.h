//=========================================
//
//	bullet3D.h
//	Author:�y���m��
//
//=========================================
#ifndef _BULLET3D_H_		//���̃}�N����`������ĂȂ�������
#define _BULLET3D_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"
#include "billboard.h"

class CBullet :public CBillboard
{
public:
	CBullet();			//�R���X�g���N�^
	virtual ~CBullet() override;			//�f�X�g���N�^

											//����������
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;
	//�I������
	void Uninit(void) override;
	//�N���G�C�g����
	static CBullet* Create(const D3DXVECTOR3 &pos, const D3DXQUATERNION &quaternion);

private:
	D3DXVECTOR3 m_pos;				// ���W
	D3DXVECTOR3 m_posOld;			// ��O�̍��W
	D3DXVECTOR3 m_move;				// �ړ���
	D3DXQUATERNION m_quaternion;	// �N�H�[�^�j�I��
};

#endif