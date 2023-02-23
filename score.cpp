//========================================================
//�X�R�A��ʂ̏���
//========================================================
#include "Score.h"
#include "input.h"
#include <math.h>
#include "application.h"
#include "render.h"
#include "game.h"
#include "playerUI.h"
#include<string>

//=========================================
//�R���X�g���N�^
//=========================================
CScore::CScore()
{
}

//=========================================
//�f�X�g���N�^
//=========================================
CScore::~CScore()
{

}
   
//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CScore::Init(const D3DXVECTOR3 &pos)
{
	CObject2D::Init(pos);

	//�T�C�Y�̐ݒ�
	CObject2D::SetSize(D3DXVECTOR3(0.0f, 0.0f,0.0f));

	//// �e�N�X�`���̐ݒ�
	//CObject2D::SetTexture(CTexture::TEXTURE_NONE);

	m_pos = GetPosition();

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(pos.x + 50.0f * nCnt, pos.y, pos.z));
		m_pNumber[nCnt]->SetScale(D3DXVECTOR3(40.0f, 40.0f, 0.0f));
		m_pNumber[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));
	}

	m_Score = 1000;

	Set();

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CScore::Update()
{
	Set();

	bool bAlert = CApplication::GetGame()->GetPUI()->GetAlert();

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		if (bAlert)
		{
			m_pNumber[nCnt]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			m_pNumber[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));
		}
	}
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CScore::Uninit()
{
//�������
	CObject2D::Uninit();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CScore::Draw()
{
	CObject2D::Draw();
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CScore* CScore::Create(const D3DXVECTOR3 &pos)
{
	CScore* pCScore = nullptr;

	pCScore = new CScore;

	if (pCScore != nullptr)
	{
		pCScore->Init(pos);
	}

	return pCScore;
}

//=========================================
// �X�R�A�̉��Z����
//=========================================
void CScore::Set()
{
	int aPosTexU[8];		//�e���̐������i�[

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		// pow�Ō������o���B
		int nCntNumber = MAX_DIGITS - nCnt - 1;		// define����0���Ȃ��̂�-1
		int nNum0 = (int)pow(10, nCntNumber + 1);	// ������10000000~�̌`�ɂ���
		int nNum1 = (int)pow(10, nCntNumber);		// �������������Ȃ������`�ɂ���

		// 2����
		aPosTexU[nCnt] = (m_Score % nNum0) / nNum1;
		m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
	}
}

void CScore::SetCor(D3DXCOLOR col)
{
	for (int i = 0; i < MAX_DIGITS; i++)
	{
		m_pNumber[i]->SetColor(col);
	}
}

//=========================================
// �ǂݍ���
//=========================================
void CScore::Load()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRender()->GetDevice();

	FILE*fp = fopen("data\\TXT\\Ranking.txt", "r");		//�t�@�C���ǂݍ���
	const int lenLine = 1024;							//1�P��̍ő吔

	for (int i = 0; i < MAX_RANK; i++)
	{
		if (fp != NULL)
		{
			fscanf(fp, "%d", &m_apScore[i]);	//�ǂݍ��񂾕������Ƃɐݒ肷��
		}
	}

	fclose(fp);
}

////=========================================
//// �����L���O�̕��ёւ�
////=========================================
//void CScore::Ranking()
//{
//	int nSave;
//	m_nRankUpdate = -1;
//	m_nTimerRanking = 0;
//	if (m_Time <= m_apScore[0])
//	{//��r
//		m_nRankUpdate++;
//		m_apScore[0];
//		for (int i = 0; i < MAX_RANK - 1; i++)
//		{
//			if (m_apScore[i] > m_apScore[i + 1])
//			{//���בւ�
//				nSave = m_apScore[i + 1];
//				m_apScore[i + 1] = m_apScore[i];
//				m_apScore[i] = nSave;
//				m_nRankUpdate++;
//			}
//		}
//	}
//}