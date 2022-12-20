//=========================================
//
//	�o���b�g�̏���(3D [�r���{�[�h])
//	Author:�y���m��
//
//=========================================
#include "bullet.h"
#include "application.h"
#include "calculation.h"
#include "texture.h"
#include "explosion.h"

//=========================================
//�R���X�g���N�^
//=========================================
CBullet::CBullet()
{
}

//=========================================
//�f�X�g���N�^
//=========================================
CBullet::~CBullet()
{

}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CBullet::Init(const D3DXVECTOR3 &pos)
{
	CBillboard::Init(pos);
	m_pos = pos;

	//�T�C�Y�̐ݒ�
	CBillboard::SetSize(10.0f);

	// �e�N�X�`���̐ݒ�
	CBillboard::SetTexture(CTexture::TEXTURE_FIRE);

	// ���C�t�ݒ�
	SetHP(100);

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CBullet::Update()
{
	// ���C�t�̏��̎擾
	int nLife = GetHP();

	// ���C�t�̌���
	nLife = ManageHP(-1);

	// �e�̈ړ��e�X�g
	m_pos = WorldCastVtx(D3DXVECTOR3(0.0f, 0.0f, 50.0f), m_pos, m_quaternion);

	// ���W�̐ݒ�
	SetPosition(m_pos);

	if (nLife <= 0)
	{
		CBullet::Uninit();
	}

	int Size = GetScale();

	CObject *object = CObject::GetObjectTop();


	//=========================================
	// �e�ƃG�l�~�[�̓����蔻��
	//=========================================
	while(object)
	{
		if (object != nullptr)
		{
			EObjType ObjType = object->GetObjectType();
			D3DXVECTOR3 size = GetSize();

			if (ObjType == OBJECT_ENEMY)
			{
				D3DXVECTOR3 posTarget = object->GetPosition();
				D3DXVECTOR3 SizeTarget = object->GetSize();

				bool bCollision = Collision(m_pos, m_posOld, posTarget, size, SizeTarget, false);

				if (bCollision)
				{
					// �e�̏I��
					CBullet::Uninit();
					CExplosion::Create(m_pos,m_quaternion);
					object->ManageHP(-1);
				}
			}
		}
		
		object = object->GetObjectNext();
	}
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CBullet::Uninit()
{
	CBillboard::Uninit();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CBullet::Draw()
{
	CBillboard::Draw();
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CBullet* CBullet::Create(const D3DXVECTOR3 &pos, const D3DXQUATERNION &quaternion)
{

	CBullet* pCBullet = nullptr;

	pCBullet = new CBullet;

	if (pCBullet != nullptr)
	{
		pCBullet->Init(pos);
		pCBullet->m_quaternion = quaternion;
	}

	return pCBullet;
}