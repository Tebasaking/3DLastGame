//=========================================
//
//	radar.h
//	Author:�y���m��
//
//=========================================
#ifndef _RADAR_H_		//���̃}�N����`������ĂȂ�������
#define _RADAR_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "billboard.h"
#include "camera.h"

class CCamera;
class CRadar :public CBillboard
{
public:
	CRadar();			//�R���X�g���N�^
	virtual ~CRadar() override;			//�f�X�g���N�^

											//����������
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;
	//�I������
	void Uninit(void) override;
	//���b�N�I������
	D3DXVECTOR3 LockOn(D3DXMATRIX *mtxWorld);
	//�N���G�C�g����
	static CRadar* Create(const D3DXVECTOR3 &pos, CObject *object);

	// �ǔ�����I�u�W�F�N�g�̐ݒ�
	void SetObject(CObject *pObj) { m_pObject = pObj; }

private:
	D3DXVECTOR3 m_pos;				// ���W
	D3DXVECTOR3 m_posOld;			// ��O�̍��W
	D3DXVECTOR3 m_move;				// �ړ���
	D3DXQUATERNION m_quaternion;	// �N�H�[�^�j�I��
	CObject *m_pObject;
	CCamera::CAMERA_TYPE Camera_Type;
};

#endif