//=========================================
//
//	title.h
//	Author:�y���m��
//
//=========================================
#ifndef _TITLE_H_		//���̃}�N����`������ĂȂ�������
#define _TITLE_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "mode.h"

//=========================================
// �N���X
//=========================================
class CTitle : public CMode
{
public:
	CTitle();
	~CTitle();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//����������
	virtual void Uninit() override { Release(); }					//�I������
	virtual void Update() override;						//�X�V����
	virtual void Draw() override {};					//�`�揈��

};

#endif	// _TITLE_H_