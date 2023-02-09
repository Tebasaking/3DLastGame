//========================================================
//スコア画面の処理
//========================================================
#include "Score.h"
#include "input.h"
#include <math.h>
#include "application.h"
#include "render.h"
#include "game.h"
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
	CObject2D::SetSize(D3DXVECTOR3(0.0f, 0.0f,0.0f));

	//// テクスチャの設定
	//CObject2D::SetTexture(CTexture::TEXTURE_NONE);

	m_pos = GetPosition();

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(pos.x + 50.0f * nCnt, pos.y, pos.z));
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
// 読み込み
//=========================================
void CScore::Load()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRender()->GetDevice();

	FILE*fp = fopen("data\\TXT\\Ranking.txt", "r");		//ファイル読み込み
	const int lenLine = 1024;							//1単語の最大数

	for (int i = 0; i < MAX_RANK; i++)
	{
		if (fp != NULL)
		{
			fscanf(fp, "%d", &m_apScore[i]);	//読み込んだ文字ごとに設定する
		}
	}

	fclose(fp);
}

////=========================================
//// ランキングの並び替え
////=========================================
//void CScore::Ranking()
//{
//	int nSave;
//	m_nRankUpdate = -1;
//	m_nTimerRanking = 0;
//	if (m_Time <= m_apScore[0])
//	{//比較
//		m_nRankUpdate++;
//		m_apScore[0];
//		for (int i = 0; i < MAX_RANK - 1; i++)
//		{
//			if (m_apScore[i] > m_apScore[i + 1])
//			{//並べ替え
//				nSave = m_apScore[i + 1];
//				m_apScore[i + 1] = m_apScore[i];
//				m_apScore[i] = nSave;
//				m_nRankUpdate++;
//			}
//		}
//	}
//}