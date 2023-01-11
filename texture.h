//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include <d3dx9.h>

//==================================================
// ��`
//==================================================
class CTexture
{
public: /* ��` */
	enum TEXTURE
	{
		TEXTURE_FIRE = 0,	// ���摜1
		TEXTURE_NUMBER,		// ���摜2
		TEXTURE_EXPLOSION,	// �����摜
		TEXTURE_SKY,		// ��
		TEXTURE_SEA,		// �C
		TEXTURE_GROUND,		// ��
		TEXTURE_THUNDER,	// �퓬�@�̃e�N�X�`��
		TEXTURE_TARGET,		// �^�[�Q�b�g
		TEXTURE_TITLE,		// �^�C�g��
		TEXTURE_RESULT,		// ���U���g
		TEXTURE_RADAR_MAP,	// ���[�_�[�}�b�v
		TEXTURE_FRAME,		// ���[�_�[�̃t���[��
		TEXTURE_ENEMY_TANK,	// �^���N
		TEXTURE_ENEMY_FLY,	// �퓬�@
		TEXTURE_MAX,
		TEXTURE_NONE,	// �g�p���Ȃ�
	};

	static const char* s_FileName[];	// �t�@�C���p�X

public:
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

public: /* �����o�֐� */
	void LoadAll();										// �S�Ă̓ǂݍ���
	void Load(TEXTURE inTexture);						// �w��̓ǂݍ���
	void ReleaseAll();									// �S�Ă̔j��
	void Release(TEXTURE inTexture);					// �w��̔j��
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// ���̎擾

private: /* �����o�ϐ� */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// �e�N�X�`���̏��
};

#endif // !_TEXTURE_H_
