//=========================================
//
// スコア処理
// Auther：Arita Meigen
//
//=========================================
#ifndef _RANKING_H_		// このマクロ定義がされてなかったら
#define _RANKING_H_		// 二重インク―ルード防止のマクロ定義

#include "object2D.h"

class CScore;

#define MAX_RANK		(3)			//表示するランキングの最大数

//=========================================
// スコアクラスを定義
//=========================================
class CRanking
{
public:
	CRanking();							//コンストラクタ
	virtual ~CRanking();			//デストラクタ

									//初期化処理
	HRESULT Init();

	//更新処理
	void Update(void);

	//描画処理
	void Draw(void);

	//終了処理
	void Uninit(void);

	//クリエイト処理
	static CRanking* Create();

	void Save();
	void Load();
	void Ranking();

	//セッター
	static void SetCurrentScore(int Time);
	void SetCor(D3DXCOLOR col);

private:
	D3DXVECTOR3 m_pos;					// 座標
	int			m_nRankUpdate;			//更新ランクNo.
	int			m_MAX_DIGITS;
	static int	m_Score;
	CScore*		m_apScore[MAX_RANK];	// スコアの最大数

};

#endif

