//=========================================
//
//	title.h
//	Author:�y���m��
//
//=========================================
#ifndef _TUTORIAL_H_		//���̃}�N����`������ĂȂ�������
#define _TUTORIAL_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "mode.h"

//=========================================
// �N���X
//=========================================
class CObject2D;
class CTutorial : public CMode
{
public:
	CTutorial();
	~CTutorial();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//����������
	virtual void Uninit() override { Release(); }					//�I������
	virtual void Update() override;						//�X�V����
	virtual void Draw() override {};					//�`�揈��

private:
	CObject2D *pObject2D[3];
	int		m_Select;
	int		m_SelectCnt;
};

#endif	// _TITLE_H_