//=========================================
//
//	�o���b�g�̏���(3D [�r���{�[�h])
//	Author:�y���m��
//
//=========================================
#include "application.h"
#include "calculation.h"
#include "texture.h"
#include "explosion.h"
#include "motion_model3D.h"
#include "radar.h"
#include "camera.h"

//=========================================
//�R���X�g���N�^
//=========================================
CRadar::CRadar()
{
	SetObjectType(OBJECT_RADAR);
}

//=========================================
//�f�X�g���N�^
//=========================================
CRadar::~CRadar()
{

}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CRadar::Init(const D3DXVECTOR3 &pos)
{
	CBillboard::Init(pos);
	m_pos = pos;

	//�T�C�Y�̐ݒ�
	CBillboard::SetSize(1000.0f);

	// �e�N�X�`���̐ݒ�
	CBillboard::SetTexture(CTexture::TEXTURE_FIRE);

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CRadar::Update()
{
	// �ڕW�̍��W�ݒ�
	D3DXVECTOR3 TargetPos = m_pObject->GetPosition();

	// �I�u�W�F�N�g�̐ݒ�
	m_pos = D3DXVECTOR3(TargetPos.x, 100.0f, TargetPos.z);

	// ���W�̐ݒ�
	SetPosition(m_pos);
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CRadar::Uninit()
{
	CBillboard::Uninit();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CRadar::Draw()
{
	CBillboard::Draw();
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CRadar* CRadar::Create(const D3DXVECTOR3 &pos, CObject *object)
{
	CRadar* pCRadar = nullptr;

	pCRadar = new CRadar;

	if (pCRadar != nullptr)
	{
		pCRadar->Init(pos);
		pCRadar->SetObject(object);
	}

	return pCRadar;
}