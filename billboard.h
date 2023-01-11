//=========================================
//
//	object2D.h
//	Author:�y���m��
//
//=========================================
#ifndef _BILLBOARD_H_		//���̃}�N����`������ĂȂ�������
#define _BILLBOARD_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "object.h"
#include "texture.h"

class CBillboard :public CObject
{
public:
	explicit CBillboard(int nPriority = 2);	//�R���X�g���N�^
	virtual ~CBillboard();			//�f�X�g���N�^

	//����������
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;
	//�I������
	void Uninit(void) override;
	//�N���G�C�g����
	static CBillboard* Create(const D3DXVECTOR3 &pos);
	//�e�N�X�`���̐ݒ菈��
	void SetTexture(CTexture::TEXTURE texture);	// �e�N�X�`���̐ݒ�

	void SetPosition(const D3DXVECTOR3& pos);
	void SetAnim(const float Num, const int Pattern);
	void SetSize(const float Size);
	void SetColor(const D3DXCOLOR &col);

	D3DXCOLOR GetColor() { return m_col; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//�o�b�t�@
	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_pos;			//���W
	CTexture::TEXTURE		m_texture;		// �e�N�X�`���̗񋓌^
	D3DXVECTOR3				m_size;			// �傫��
	float					m_scale;		// �g�嗦
	D3DXCOLOR m_col;						// �F
	D3DXVECTOR3 m_rot;
};

#endif