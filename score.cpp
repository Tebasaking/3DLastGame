//========================================================
//スコア画面の処理
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
	CObject2D::Init(pos);

	//サイズの設定
	CObject2D::SetSize(D3DXVECTOR3(44.0f, 44.0f,0.0f));

	//// テクスチャの設定
	//CObject2D::SetTexture(CTexture::TEXTURE_NONE);

	m_pos = GetPosition();

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(pos.x + 50.0f * nCnt, pos.y, pos.z));
		m_pNumber[nCnt]->SetScale(D3DXVECTOR3(m_NumSize.x, m_NumSize.y, 0.0f));
		m_pNumber[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));
	}

	m_Score = 1000;

	Set();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CScore::Update()
{
	Set();

	if (CApplication::GetMode() != CApplication::MODE_GAME)
	{

		return;
	}

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
//オブジェクトの終了処理
//=========================================
void CScore::Uninit()
{
//解放処理
	CObject2D::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CScore::Draw()
{
	CObject2D::Draw();
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CScore* CScore::Create(const D3DXVECTOR3 &pos,D3DXVECTOR3 Numsize)
{
	CScore* pCScore = nullptr;

	pCScore = new CScore;

	if (pCScore != nullptr)
	{
		pCScore->ScoreSizeSet(Numsize);
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
		// powで桁数を出す。
		int nCntNumber = MAX_DIGITS - nCnt - 1;		// defineだと0がないので-1
		int nNum0 = (int)pow(10, nCntNumber + 1);	// 桁数を10000000~の形にする
		int nNum1 = (int)pow(10, nCntNumber);		// 桁数を上より一つ少ない同じ形にする

		// 2桁目
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
// 大きさの設定
//=========================================
void CScore::ScoreSizeSet(D3DXVECTOR3 size)
{
	m_NumSize = size;
}