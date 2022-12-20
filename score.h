//=========================================
//
// �X�R�A�̏���
// Auther�F�y���m��
//
//=========================================
#ifndef _SCORE_H_		// ���̃}�N����`������ĂȂ�������
#define _SCORE_H_		// ��d�C���N�\���[�h�h�~�̃}�N����`

#include "main.h"
#include "object2D.h"
#include "number.h"

#define MAX_DIGITS		(8)			// �X�R�A�̌���
//=========================================
// �X�R�A�N���X���`
//=========================================
class CScore
{
public:
	explicit CScore();	//�R���X�g���N�^
	virtual ~CScore();			//�f�X�g���N�^

	//����������
	HRESULT Init(const D3DXVECTOR3 &pos);
	//�X�V����
	void Update(void);
	//�`�揈��
	void Draw(void)	;
	//�I������
	void Uninit(void);
	//�N���G�C�g����
	static CScore* Create(const D3DXVECTOR3 &pos);

	//�X�R�A�̐ݒ�
	void Set();
	void CScore::Add(int nValue);
	static int Get() { return m_Score; }

private:
	D3DXVECTOR3 m_pos;
	CNumber	*m_pNumber[MAX_DIGITS];		// �i���o�[1
	static int m_Score;			// �X�R�A�̒l

};

#endif