//=========================================
//
//	�����̏���(3D [�r���{�[�h])
//	Author:�y���m��
//
//=========================================
#include "explosion.h"
#include "application.h"
#include "texture.h"
#include "calculation.h"
#include "explosion.h"
#include "missile_locus.h"
#include "sound.h"

const int CMissile_Locus::ANIM_SPEED = 10;
const int CMissile_Locus::ANIM_MAX = 8;
//=========================================
//�R���X�g���N�^
//=========================================
CMissile_Locus::CMissile_Locus()
{
	nAnimCount = 0;
	nAnimNum = 0;
}

//=========================================
//�f�X�g���N�^
//=========================================
CMissile_Locus::~CMissile_Locus()
{

}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CMissile_Locus::Init(const D3DXVECTOR3 &pos)
{
	CBillboard::Init(pos);

	// ���W
	m_pos = pos;

	m_RandAlfa = (float)(rand() % 10 + 1) * 0.01f;

	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	
	//�T�C�Y�̐ݒ�
	CBillboard::SetSize(D3DXVECTOR3(40.0f,40.0f,0.0f));

	// �e�N�X�`���̐ݒ�
	CBillboard::SetTexture(CTexture::TEXTURE_EXPLOSION);

	// �̗͂̐ݒ�
	SetHP(100);

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CMissile_Locus::Update()
{
	int nLife = GetHP();

	nLife = ManageHP(-1);

	SetPosition(m_pos);
	
	m_col = GetColor();
	m_col.a -= m_RandAlfa;
	m_col.r = 1.0f;
	m_col.g = .0f;
	m_col.b = .0f;

	SetColor(m_col);

	D3DXVECTOR3 Scale = GetSize();

	Scale.x += 1.0f;
	Scale.y += 1.0f;

	//�T�C�Y�̐ݒ�
	CBillboard::SetSize(Scale);

	int Size = (float)GetScale();

	nAnimCount++;

	CBillboard::SetAnim((float)nAnimNum, ANIM_MAX);

	// �A�j���[�V��������
	if (nAnimCount >= ANIM_SPEED)
	{
		nAnimNum++;
		nAnimCount = 0;

		if (nAnimNum >= ANIM_MAX)
		{
			nAnimNum = 0;
			//CMissile_Locus::Uninit();
		}
	}

	if (m_col.a <= 0)
	{
		Uninit();
	}
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CMissile_Locus::Uninit()
{
	CBillboard::Uninit();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CMissile_Locus::Draw()
{
	CBillboard::Draw();
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CMissile_Locus* CMissile_Locus::Create(const D3DXVECTOR3 &pos, CObject *object)
{
	int nNumCreate = m_nNumAll;

	CMissile_Locus* pLocus = nullptr;

	pLocus = new CMissile_Locus;

	if (pLocus != nullptr)
	{
		pLocus->Init(pos);
		//CSound::PlaySound(CSound::SOUND_SE_EXPLOSION);
	}

	return pLocus;
}