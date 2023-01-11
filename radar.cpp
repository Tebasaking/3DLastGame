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
#include "Object2D.h"
#include "radar.h"
#include "camera.h"

//=========================================
//�R���X�g���N�^
//=========================================
CRadar::CRadar(int nPriority)
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

	m_pBackGround = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.25f * 0.5f, SCREEN_HEIGHT * 0.5f * 0.5f,0.0f),0);
	m_pBackGround->SetTexture(CTexture::TEXTURE_FRAME);
	m_pBackGround->SetScale(D3DXVECTOR3(SCREEN_WIDTH * 0.25f * 0.5f, SCREEN_HEIGHT * 0.5f * 0.5f,0.0f));
	m_pBackGround->SetObjectType(OBJECT_RADAR);

	switch (m_type)
	{
	case RADAR_MAP:
		// �e�N�X�`���̐ݒ�
		CBillboard::SetTexture(CTexture::TEXTURE_RADAR_MAP);
		//�T�C�Y�̐ݒ�
		CBillboard::SetSize(10000.0f);
		// COLOR�ݒ�
		CBillboard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case RADAR_PLAYER:
		// �e�N�X�`���̐ݒ�
		CBillboard::SetTexture(CTexture::TEXTURE_ENEMY_FLY);
		//�T�C�Y�̐ݒ�
		CBillboard::SetSize(200.0f);
		// COLOR�ݒ�
		CBillboard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f));
		break;

	case RADAR_ENEMY:
		// �e�N�X�`���̐ݒ�
		CBillboard::SetTexture(CTexture::TEXTURE_ENEMY_TANK);
		//�T�C�Y�̐ݒ�
		CBillboard::SetSize(200.0f);
		// COLOR�ݒ�
		CBillboard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CRadar::Update()
{
	D3DXCOLOR col = GetColor();

	/*if (m_type != RADAR_MAP)
	{*/
		// �ڕW�̍��W�ݒ�
		D3DXVECTOR3 TargetPos = m_pObject->GetPosition();

		// �I�u�W�F�N�g�̐ݒ�
		m_pos = D3DXVECTOR3(TargetPos.x, 1090.0f, TargetPos.z);

		// ���W�̐ݒ�
		SetPosition(m_pos);
	//}

	CBillboard::Update();

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
CRadar* CRadar::Create(const D3DXVECTOR3 &pos, CObject *object, RADAR_TYPE type)
{
	CRadar* pCRadar = nullptr;

	pCRadar = new CRadar(5);

	if (pCRadar != nullptr)
	{
		pCRadar->SetType(type);
		pCRadar->Init(pos);
		pCRadar->SetObject(object);
	}

	return pCRadar;
}