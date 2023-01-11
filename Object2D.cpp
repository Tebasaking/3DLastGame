//=========================================
//
//	2D�I�u�W�F�N�g�̏���
//	Author:�y���m��
//
//=========================================
#include "object2D.h"
#include "application.h"
#include "texture.h"

#define POLYGON_SIZE (400.0f)

//=========================================
//�R���X�g���N�^
//=========================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_texture = CTexture::TEXTURE_NONE;
}

//=========================================
//�f�X�g���N�^
//=========================================
CObject2D::~CObject2D()
{

}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CObject2D::Init(const D3DXVECTOR3 &pos)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	m_pos = pos;

	m_texture = CTexture::TEXTURE_NONE;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CObject2D::Update()
{
	//���_���W�X�V����
	VtxUpdate();
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CObject2D::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	CObject::Release();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CObject2D::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	CTexture* pTexture = CApplication::GetTexture();

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0,pTexture->GetTexture(m_texture));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

	//�A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CObject2D* CObject2D::Create(const D3DXVECTOR3 &pos,int nPriority)
{
	CObject2D* pObject2D = nullptr;

	pObject2D = new CObject2D(nPriority);
	
	if (pObject2D != nullptr)
	{
		pObject2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pObject2D->Init(pos);
	}

	return pObject2D;
}

//=========================================
//���W�̐ݒ菈��
//=========================================
void CObject2D::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;

	VtxUpdate();
}

//=========================================
// �e�N�X�`���̐ݒ�
//=========================================
void CObject2D::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//=========================================
//�T�C�Y�̐ݒ�
//=========================================
void CObject2D::SetScale(const D3DXVECTOR3 Size)
{
	m_scale = Size;

	VtxUpdate();
}

//=========================================
//�e�N�X�`���̐ݒ�
//=========================================
void CObject2D::SetAnim(const float Num,const int Pattern)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	float PatternSize = 1.0f / Pattern;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//tex�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(Num * PatternSize, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(Num * PatternSize + PatternSize, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(Num * PatternSize, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(Num * PatternSize + PatternSize, 1.0f);

	// ���_���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_���W�X�V����
//=============================================================================
void CObject2D::VtxUpdate()
{
	if (m_pVtxBuff != nullptr)
	{
		//���_���ւ̃|�C���^
		VERTEX_2D*pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_scale.x, m_pos.y - m_scale.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_scale.x, m_pos.y - m_scale.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_scale.x, m_pos.y + m_scale.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_scale.x, m_pos.y + m_scale.y, 0.0f);

		//���_�J���[�̍X�V
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=========================================
// �J���[�̐ݒ�
//=========================================
void CObject2D::SetColor(const D3DXCOLOR &col)
{
	m_col = col;
	VtxUpdate();
}