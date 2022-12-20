//=========================================
//
// スコアの処理
// Auther：冨所知生
//
//=========================================
#ifndef _SCORE_H_		// このマクロ定義がされてなかったら
#define _SCORE_H_		// 二重インク―ルード防止のマクロ定義

#include "main.h"
#include "object2D.h"
#include "number.h"

#define MAX_DIGITS		(8)			// スコアの桁数
//=========================================
// スコアクラスを定義
//=========================================
class CScore
{
public:
	explicit CScore();	//コンストラクタ
	virtual ~CScore();			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos);
	//更新処理
	void Update(void);
	//描画処理
	void Draw(void)	;
	//終了処理
	void Uninit(void);
	//クリエイト処理
	static CScore* Create(const D3DXVECTOR3 &pos);

	//スコアの設定
	void Set();
	void CScore::Add(int nValue);
	static int Get() { return m_Score; }

private:
	D3DXVECTOR3 m_pos;
	CNumber	*m_pNumber[MAX_DIGITS];		// ナンバー1
	static int m_Score;			// スコアの値

};

#endif