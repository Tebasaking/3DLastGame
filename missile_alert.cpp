//=========================================
//
//	�~�T�C���A���[�g�̏���
//	Author:�y���m��
//
//=========================================
#include "missile_alert.h"
#include "camera.h"
#include "application.h"
#include "debug_proc.h"
#include "bullet3D.h"

D3DXVECTOR3 CAlert::m_TargetPos = {};
//=========================================
//�R���X�g���N�^
//=========================================
CAlert::CAlert()
{

}

//=========================================
//�f�X�g���N�^
//=========================================
CAlert::~CAlert()
{

}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CAlert::Init(const D3DXVECTOR3 &pos)
{
	//����������
	CObject2D::Init(pos);

	// �e�N�X�`���̐ݒ�
	CObject2D::SetTexture(CTexture::TEXTURE_ARROW);

	//�T�C�Y�̐ݒ�
	CObject2D::SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �^�C�v�ݒ�
	//SetObjectType(OBJECT_TARGET);

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CAlert::Update()
{
	D3DXVECTOR3 BulletPos;			// �G�l�~�[�̕ϊ��O���W
	D3DXVECTOR3 PlayerPos;			// �v���C���[�̍��W

	m_Size = 50.0f;

	//�v���C���[�̍��W���擾
	if (pObject != nullptr)
	{
		// �ڕW�̍��W���X�N���[�����W�ɕϊ�����
		BulletPos = pObject->GetPosition();
		m_TargetPos = CApplication::WorldToScreen(BulletPos);
	}

	CObject *object = CObject::GetObjectTop();

	//�v���C���[�̍��W���擾
	while (object)
	{
		if (object != nullptr)
		{
			EObjType ObjType = object->GetObjectType();

			if (ObjType == OBJECT_PLAYER)
			{
				PlayerPos = object->GetPosition();

				break;
			}
		}
		object = object->GetObjectNext();
	}

	// TargetPos�̐���
	if (m_TargetPos.x >= 0.0f && m_TargetPos.x <= SCREEN_WIDTH &&
		m_TargetPos.y >= 0.0f && m_TargetPos.y <= SCREEN_HEIGHT &&
		m_TargetPos.z >= 0.0f)
	{
		//m_Size = 0.0f;
	}
	else
	{
		m_Size = 50.0f;
	}

	//�T�C�Y�̐ݒ�
	CObject2D::SetScale(D3DXVECTOR3(m_Size, m_Size, 0.0f));

	// �A���[�g��X�����̎n�_
	float ALERT_SCREEN_WIDTH_BEGIN = 300.0f;
	// �A���[�g��Y�����̎n�_
	float ALERT_SCREEN_HEIGHT_BEGIN = 500;

	// �A���[�g��X�����̏I�_
	float ALERT_SCREEN_WIDTH_END = SCREEN_WIDTH - 300.0f;
	// �A���[�g��Y�����̏I�_
	float ALERT_SCREEN_HEIGHT_END = SCREEN_HEIGHT - 200.0f;

	m_TargetPos.y *= 0.1f;
	m_TargetPos.y += 500.0f;

	// �A���[�g�n�_����
	if (m_TargetPos.x >= ALERT_SCREEN_WIDTH_END)
	{
		m_TargetPos.x = ALERT_SCREEN_WIDTH_END - m_Size;
	}
	if (m_TargetPos.y <= ALERT_SCREEN_HEIGHT_BEGIN)
	{
		m_TargetPos.y = ALERT_SCREEN_HEIGHT_BEGIN + m_Size;
	}

	// �I�_����
	if (m_TargetPos.x <= ALERT_SCREEN_WIDTH_BEGIN)
	{
		m_TargetPos.x = ALERT_SCREEN_WIDTH_BEGIN + m_Size;
	}
	if (m_TargetPos.y >= ALERT_SCREEN_HEIGHT_END)
	{
		m_TargetPos.y = ALERT_SCREEN_HEIGHT_END - m_Size;
	}

	// TargetPos��PlayerPos����p�x�����߂�
	D3DXVECTOR3 rot = AtanRot(m_TargetPos, PlayerPos);

	// �X�N���[�����W�ɐݒ肷��
	SetPosition(m_TargetPos);

	SetRotation(rot);

	// ��O����
	CBullet3D *pBullet = (CBullet3D*)pObject;

	if (pBullet != nullptr)
	{
		if (pBullet->GetSpeed() >= 100)
		{
			SetScale(D3DXVECTOR3(0.0f,0.0f,0.0f));
		}
	}

	EObjType obj = GetObjectType();
	CAlert *pTagert = this;

	CDebugProc::Print("missile�̍��W x %.1f y ,%.1f,z %.1f \n", GetTest().x, GetTest().y, GetTest().z);
	// �J�����̎��_
	CDebugProc::Print("�^�[�Q�b�g�̃T�C�Y %f \n", m_Size);
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CAlert::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CAlert::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// Z�e�X�g���g�p����
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	// ���e�X�g���g�p����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	{
		// 2D�I�u�W�F�N�g�̕`�揈��
		CObject2D::Draw();
	}

	// Z�e�X�g�̏I��
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���e�X�g�̏I��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CAlert* CAlert::Create(const D3DXVECTOR3 &pos, CObject *object)
{
	CAlert* pCAlert = nullptr;

	pCAlert = new CAlert;

	if (pCAlert != nullptr)
	{
		pCAlert->Init(pos);
	}

	// �^�[�Q�b�g��ݒ肷��I�u�W�F�N�g���擾
	pCAlert->SetObject(object);

	return pCAlert;
}