//========================================================
//�X�R�A��ʂ̏���
//========================================================
#include "score.h"
#include "input.h"
#include <math.h>

int CScore::m_Score = 0;
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
	// ���W����
	m_pos = pos;

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + 50.0f * nCnt,m_pos.y + 50.0f,m_pos.z));
	}

	Set();

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CScore::Update()
{
	Add(10);
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CScore::Uninit()
{
	m_Score = 0;
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CScore::Draw()
{
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
		int nCntNumber = MAX_DIGITS - nCnt - 1;
		int nNum0 = (int)pow(10, nCntNumber + 1);
		int nNum1 = (int)pow(10, nCntNumber);

		aPosTexU[nCnt] = (m_Score % nNum0) / nNum1;

		m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
	}
}

//=========================================
// �X�R�A�̉��Z����
//=========================================
void CScore::Add(int Value)
{
	m_Score += Value;
	Set();
}