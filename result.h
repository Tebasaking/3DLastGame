//=========================================
//
//	game.h
//	Author:�y���m��
//
//=========================================
#ifndef _RESULT_H_		//���̃}�N����`������ĂȂ�������
#define _RESULT_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "mode.h"

//=========================================
// �N���X
//=========================================
class CResult : public CMode
{
public:
	CResult();
	~CResult();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//����������
	virtual void Uninit() override { Release(); }			//�I������
	virtual void Update() override;						//�X�V����
	virtual void Draw() override {};					//�`�揈��

};

#endif	// _RESULT_H_