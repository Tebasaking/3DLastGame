////=========================================
////
////	object2D.h
////	Author:�y���m��
////
////=========================================
//#ifndef _BALL_H_		//���̃}�N����`������ĂȂ�������
//#define _BALL_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`
//
//#include "main.h"
//#include "object.h"
//#include "texture.h"
//
//class CModel;
//class CBall :public CObject
//{
//public:
//	CBall();			//�R���X�g���N�^
//	virtual ~CBall();			//�f�X�g���N�^
//
//								//����������
//	HRESULT Init(const D3DXVECTOR3 &pos) override;
//	//�X�V����
//	void Update(void) override;
//	//�`�揈��
//	void Draw(void) override;
//	//�I������
//	void Uninit(void) override;
//	//�N���G�C�g����
//	static CBall* Create(const D3DXVECTOR3 &pos);
//	//�e�N�X�`���̐ݒ菈��
//	void SetTexture(CTexture::TEXTURE texture);	// �e�N�X�`���̐ݒ�
//	//�R���g���[������
//	void Control_();
//
//	void SetPosition(const D3DXVECTOR3& pos);
//	const D3DXVECTOR3& GetPosition() const override { return m_pos; }
//	const D3DXVECTOR3& GetRot() const override { return m_rot; }
//	const float& GetSize() const override { return m_size; }
//
//private:
//	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//�o�b�t�@
//	LPDIRECT3DINDEXBUFFER9  m_pIdxVtxBuff;	//���b�V���t�B�[���h�o�b�t�@
//	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
//	D3DXVECTOR3 m_pos;						//���W
//	CTexture::TEXTURE m_texture;			// �e�N�X�`���̗񋓌^
//	float m_size;							// �傫��
//	D3DXVECTOR3 m_rot;
//	CModel *pModel;							// ���f���̌�
//};
//
//#endif