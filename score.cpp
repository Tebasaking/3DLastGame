//========================================================
//スコア画面の処理
//========================================================
#include "score.h"
#include "input.h"
#include <math.h>

int CScore::m_Score = 0;
//=========================================
//コンストラクタ
//=========================================
CScore::CScore()
{
}

//=========================================
//デストラクタ
//=========================================
CScore::~CScore()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CScore::Init(const D3DXVECTOR3 &pos)
{
	// 座標を代入
	m_pos = pos;

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + 50.0f * nCnt,m_pos.y + 50.0f,m_pos.z));
	}

	Set();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CScore::Update()
{
	Add(10);
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CScore::Uninit()
{
	m_Score = 0;
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CScore::Draw()
{
}

//=========================================
//オブジェクトのクリエイト
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
// スコアの加算処理
//=========================================
void CScore::Set()
{
	int aPosTexU[8];		//各桁の数字を格納

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
// スコアの加算処理
//=========================================
void CScore::Add(int Value)
{
	m_Score += Value;
	Set();
}