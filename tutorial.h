//=========================================
//
//	game.h
//	Author:�y���m��
//
//=========================================
#ifndef _TUTORIAL_H_		//���̃}�N����`������ĂȂ�������
#define _TUTORIAL_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "mode.h"

//=========================================
// �N���X
//=========================================
class CTutorial : public CMode
{
public:
	CTutorial();
	~CTutorial();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//����������
	virtual void Uninit() override;						//�I������
	virtual void Update() override;						//�X�V����
	virtual void Draw() override {};					//�`�揈��
};

#endif	// _RESULT_H_