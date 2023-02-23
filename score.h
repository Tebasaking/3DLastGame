//=========================================
//
// スコア処理
// Auther：冨所知生
//
//=========================================
#ifndef _SCORE_H_		// このマクロ定義がされてなかったら
#define _SCORE_H_		// 二重インク―ルード防止のマクロ定義

#include "object2D.h"
#include "number.h"

//=========================================
// スコアクラスを定義
//=========================================
class CScore :public CObject2D
{
private:
	static const int MAX_DIGITS = 6;
public:
	explicit CScore();							//コンストラクタ
	virtual ~CScore() override;			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;

	//更新処理
	void Update(void) override;

	//描画処理
	void Draw(void) override;

	//終了処理
	void Uninit(void) override;

	//クリエイト処理
	static CScore* Create(const D3DXVECTOR3 &pos, D3DXVECTOR3 Numsize);
	int GetScore() { return m_Score; }

	//スコアの設定
	void Set();
	void SetCor(D3DXCOLOR col);

	// スコアの加算処理
	void Add(int Score) { m_Score += Score; }
	void ScoreSet(int Score) { m_Score = Score; }
	void ScoreSizeSet(D3DXVECTOR3 size);
	void Load();
	//void Ranking();

private:
	D3DXVECTOR3 m_pos;					// 座標
	CNumber*	m_pNumber[MAX_DIGITS];	// ナンバー1
	D3DXVECTOR3 m_NumSize;				// ナンバーのサイズ
	int			m_Score;				// スコア
	int			m_nRankUpdate;			// 更新ランクNo.
	int			m_nTimerRanking;		// ランキング画面表示タイマー
	float		m_fPtnrank;				// プレイヤー点滅
	int			m_nCntrank;				// 点滅のカウント用

};

#endif
