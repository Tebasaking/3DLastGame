//========================================================
//スコア画面の処理
//========================================================
#include "time.h"
#include "input.h"
#include "game.h"
#include "inputkeyboard.h"
#include "application.h"
#include "game.h"
#include "playerUI.h"
#include "fade.h"
#include <math.h>

int CTime::m_Time = 0;
int CTime::m_Seconds = 0;
int CTime::m_Minutes = 0;
//=========================================
//コンストラクタ
//=========================================
CTime::CTime()
{
}

//=========================================
//デストラクタ
//=========================================
CTime::~CTime()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CTime::Init(const D3DXVECTOR3 &pos)
{
	CObject2D::Init(pos);

	//サイズの設定
	CObject2D::SetSize(D3DXVECTOR3(0.0f, 0.0f,0.0f));

	//ゲームの開始時刻を取得
	m_StartGameTime = timeGetTime();

	// テクスチャの設定	CObject2D::SetTexture(CTexture::TEXTURE_NONE);
	
	m_pos = pos;

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + 50.0f * nCnt, m_pos.y, m_pos.z));
		m_pNumber[nCnt]->SetScale(D3DXVECTOR3(40.0f, 40.0f, 0.0f));
		m_pNumber[nCnt]->SetColor(D3DXCOLOR(0.0f,0.6f,0.0f,1.0f));
	}

	Set();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CTime::Update()
{
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

	if (!CGame::GetFinish())
	{
		// 計算結果の値
		int m_ResultTime;

		// 現在のゲーム時間を表示
		m_Time = timeGetTime() - m_StartGameTime;

		m_ResultTime = m_Alarm - m_Time;

		m_Minutes = m_ResultTime / 60000;					// 分に変換
		m_Seconds = m_ResultTime - (m_Minutes * 60000);		// 分に変換した分を引いた値を秒に入れる

		// 時間設定
		Set();

		// 時間が0以下になった時
		if (m_ResultTime <= 0)
		{
			CGame::Add(5000);
			// ゲームを終了する
			CGame::Finish();
			Uninit();
		}
	}
	CInput *pKeyboard = CInput::GetKey();
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CTime::Uninit()
{
	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		if (m_pNumber[nCnt] != nullptr)
		{
			m_pNumber[nCnt]->Uninit();
			m_pNumber[nCnt] = nullptr;
		}
	}

	//解放処理
	CObject2D::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CTime::Draw()
{
	CObject2D::Draw();
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CTime* CTime::Create(const D3DXVECTOR3 &pos)
{
	CTime* pCTime = nullptr;

	pCTime = new CTime;

	if (pCTime != nullptr)
	{
		pCTime->Init(pos);
	}

	return pCTime;
}

//=========================================
// スコアの加算処理
//=========================================
void CTime::Set()
{
	int aPosTexU[8];		//各桁の数字を格納

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		if (m_pNumber[nCnt] == nullptr)
		{//nullじゃないなら
			return;
		}

		// powで桁数を出す。
		int nCntNumber = MAX_DIGITS - nCnt - 1;		// defineだと0がないので-1
		int nNum0 = (int)pow(10, nCntNumber + 1);	// 桁数を10000000~の形にする
		int nNum1 = (int)pow(10, nCntNumber);		// 桁数を上より一つ少ない同じ形にする

		// 2桁目
		if (nCnt == 1)
		{// 分
		 // (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Minutes % (int)pow(10, nCnt));
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		// 1桁目、0割が起きてしまうので↑と同時に処理できない・・・
		else if (nCnt == 0)
		{
			// (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Minutes * 0.1f);
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		else if (nCnt == 2 || nCnt == 5)
		{// セミコロン
		 // 三桁目にセミコロンを設置する。
			aPosTexU[nCnt] = 11;

			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		else if (nCnt == 6 || nCnt == 7)
		{// セミコロン後の数字
			int nNumDec = nNum0 * 10;
			int nNumDec2 = nNum1 * 10;

			// (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Seconds % nNumDec) / nNumDec2;
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		else
		{// セミコロンとセミコロンの間の数字(なにもしてない)
		 // (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Seconds % nNum0) / nNum1;
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
	}
}

//=========================================
// スコアの加算処理
//=========================================
void CTime::Add(int Value)
{
	m_Seconds += Value;
	Set();
}

//=========================================
// スコアの減少処理
//=========================================
void CTime::Sub(int Value)
{
	m_Seconds -= Value;
	Set();
}

//=========================================
// アラームの設定
//=========================================
void CTime::SetAlarm(int minutes, int seconds)
{
	m_Alarm += minutes * 60000;			// 分の設定
	m_Alarm += seconds * 1000;			// 秒の設定
}