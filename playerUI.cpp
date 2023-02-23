//==================================================
//
// title.cpp
// Author: �y���m��
//
//==================================================
#include "application.h"
#include "title.h"
#include "player3D.h"
#include "enemy.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "texture.h"
#include "meshfield.h"
#include "sphere.h"
#include "camera.h"
#include "playerUI.h"
#include "game.h"
#include "player3D.h"
#include "number.h"
#include "camera_player.h"
#include "player_manager.h"
#include "missile_alert.h"

//==================================================
// �R���X�g���N�^
//==================================================
CPlayerUI::CPlayerUI(int nPriority) : CObject(nPriority)
{
}

//==================================================
// �f�X�g���N�^
//==================================================
CPlayerUI::~CPlayerUI()
{
}

//=========================================
// ������ 
//=========================================
HRESULT CPlayerUI::Init(const D3DXVECTOR3 &pos)
{
	// �Q�[�W���ݒ�
	m_pGage = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 50.0f, 0.0f), 4);
	m_pGageBox = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 50.0f, 0.0f), 4);

	m_pGageBox->SetScale(D3DXVECTOR3(100.0f, 5.0f, 0.0f));

	m_pGage->SetTexture(CTexture::TEXTURE_GAGE);
	m_pGageBox->SetTexture(CTexture::TEXTURE_GAGE_BOX);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pObject2D[nCnt] = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 4);
		pObject2D[nCnt]->SetScale(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	}

	pObject2D[0]->SetTexture(CTexture::TEXTURE_UI_SPEED_AND_ALT);
	pObject2D[1]->SetTexture(CTexture::TEXTURE_UI_WEAPON);

	for (int nCnt = 0; nCnt < SPEED_DIGITS; nCnt++)
	{
		m_pSpeedNum[nCnt] = CNumber::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 200.0f + (25.0f * nCnt), SCREEN_HEIGHT * 0.5f + 19.0f,0.0f));
		m_pSpeedNum[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.8f, 0.0f, 1.0f));
		m_pSpeedNum[nCnt]->SetScale(D3DXVECTOR3(15.0f, 13.0f, 0.0f));
	}
	
	for (int nCnt = 0; nCnt < ALTITUDE_DIGITS; nCnt++)
	{
		m_pAltitude[nCnt] = CNumber::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 150.0f + (25.0f * nCnt), SCREEN_HEIGHT * 0.5f + 19.0f, 0.0f));
		m_pAltitude[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.8f, 0.0f, 1.0f));
		m_pAltitude[nCnt]->SetScale(D3DXVECTOR3(15.0f, 13.0f, 0.0f));
	}

	for (int nCnt = 0; nCnt < MISSILE_DIGITS; nCnt++)
	{
		m_pMissileNum[nCnt] = CNumber::Create(D3DXVECTOR3(SCREEN_WIDTH - 330.0f + (25.0f * nCnt), SCREEN_HEIGHT - 80.0f, 0.0f));
		m_pMissileNum[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.8f, 0.0f, 1.0f));
		m_pMissileNum[nCnt]->SetScale(D3DXVECTOR3(30.0f, 30.0f, 0.0f));
	}

	// �e�N�X�`�����W�̐ݒ�
	Set();

	return S_OK;
}

//=========================================
// �X�V����
//=========================================
void CPlayerUI::Update()
{
	CPlayerManager *pPlayer = CApplication::GetGame()->GetPM();
	int nReload = 0;

	if (pPlayer != nullptr)
	{
		nReload = pPlayer->GetPlayer()->GetBulletDelay();
	}

	m_pGage->SetScale(D3DXVECTOR3(nReload, 5.0f, 0.0f));

	// �A���[�g��ON / OFF�̃`�F�b�N
	AlertCheck();

	if (m_bAlert)
	{
		SetAlertColor(RED);
	}
	else
	{
		SetAlertColor(GREEN);
	}

	Set();
}

//=========================================
// �I������
//=========================================
void CPlayerUI::Uninit()
{
	for (int nCnt = 0; nCnt < SPEED_DIGITS; nCnt++)
	{
		if (m_pSpeedNum[nCnt] != nullptr)
		{
			m_pSpeedNum[nCnt]->Uninit();
		}
	}

	for (int nCnt = 0; nCnt < ALTITUDE_DIGITS; nCnt++)
	{
		if (m_pAltitude[nCnt] != nullptr)
		{
			m_pAltitude[nCnt]->Uninit();
		}
	}

	for (int nCnt = 0; nCnt < MISSILE_DIGITS; nCnt++)
	{
		if (m_pMissileNum[nCnt] != nullptr)
		{
			m_pMissileNum[nCnt]->Uninit();
		}
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pObject2D[nCnt]->Uninit();
	}

	if (m_pGageBox != nullptr)
	{
		m_pGageBox->Uninit();
	}
	if (m_pGage != nullptr)
	{
		m_pGage->Uninit();
	}
}
//=========================================
// �����̐ݒ�
//=========================================
void CPlayerUI::Set()
{
	int aPosTexU[8];		//�e���̐������i�[
	// ���x���擾����
	float MoveSpeed = CApplication::GetCamera()->GetMoveSpeed() * 10.0f;
	float Alt = CApplication::GetCamera()->GetPosR().y;

	CPlayerManager *pPlayer = CApplication::GetGame()->GetPM();
	
	int nHaveBullet = 0;

	if (pPlayer != nullptr)
	{
		nHaveBullet = pPlayer->GetPlayer()->GetBulletHave();
	}

	for (int nCnt = 0; nCnt < SPEED_DIGITS; nCnt++)
	{
		// pow�Ō������o���B
		int nCntNumber = SPEED_DIGITS - nCnt - 1;		// define����0���Ȃ��̂�-1
		int nNum0 = (int)pow(10, nCntNumber + 1);	// ������10000000~�̌`�ɂ���
		int nNum1 = (int)pow(10, nCntNumber);		// �������������Ȃ������`�ɂ���

		// 2����
		aPosTexU[nCnt] = ((int)MoveSpeed % nNum0) / nNum1;
		m_pSpeedNum[nCnt]->Set(aPosTexU[nCnt]);
	}
	for (int nCnt = 0; nCnt < ALTITUDE_DIGITS; nCnt++)
	{
		// pow�Ō������o���B
		int nCntNumber = ALTITUDE_DIGITS - nCnt - 1;		// define����0���Ȃ��̂�-1
		int nNum0 = (int)pow(10, nCntNumber + 1);	// ������10000000~�̌`�ɂ���
		int nNum1 = (int)pow(10, nCntNumber);		// �������������Ȃ������`�ɂ���

													// 2����
		aPosTexU[nCnt] = ((int)Alt % nNum0) / nNum1;
		m_pAltitude[nCnt]->Set(aPosTexU[nCnt]);
	}
	for (int nCnt = 0; nCnt < MISSILE_DIGITS; nCnt++)
	{
		// pow�Ō������o���B
		int nCntNumber = MISSILE_DIGITS - nCnt - 1;		// define����0���Ȃ��̂�-1
		int nNum0 = (int)pow(10, nCntNumber + 1);	// ������10000000~�̌`�ɂ���
		int nNum1 = (int)pow(10, nCntNumber);		// �������������Ȃ������`�ɂ���

													// 2����
		aPosTexU[nCnt] = ((int)nHaveBullet % nNum0) / nNum1;
		m_pMissileNum[nCnt]->Set(aPosTexU[nCnt]);
	}

}

//=========================================
// �F�̐ݒ�
//=========================================
void CPlayerUI::SetAlertColor(COLOR col)
{
	D3DXCOLOR Color;

	switch (col)
	{
	case RED:
		Color = D3DXCOLOR(0.9f, 0.0f, 0.0f, 1.0f);
		break;

	case GREEN:
		Color = D3DXCOLOR(0.0f, 0.8f, 0.0f, 1.0f);
		break;
	}

	// �F�̐ݒ�
	pObject2D[0]->SetColor(Color);
	pObject2D[1]->SetColor(Color);
	m_pGage->SetColor(Color);
	m_pGageBox->SetColor(Color);

	for (int nCnt = 0; nCnt < SPEED_DIGITS; nCnt++)
	{
		m_pSpeedNum[nCnt]->SetColor(Color);
	}

	for (int nCnt = 0; nCnt < ALTITUDE_DIGITS; nCnt++)
	{
		m_pAltitude[nCnt]->SetColor(Color);
	}

	for (int nCnt = 0; nCnt < MISSILE_DIGITS; nCnt++)
	{
		m_pMissileNum[nCnt]->SetColor(Color);
	}
}

//=========================================
// �A���[�g���N�����Ă��邩���Ă��Ȃ����̃`�F�b�N
//=========================================
void CPlayerUI::AlertCheck()
{
	CAlert *pAlert = CApplication::GetGame()->GetAlert();

	if (pAlert != nullptr)
	{
		// Alert�ɓ����
		m_bAlert = CApplication::GetGame()->GetAlert()->GetCheck();
	}
}