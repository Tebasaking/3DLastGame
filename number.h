//=========================================
//
// スコアの処理
// Auther：冨所知生
//
//=========================================
#ifndef _NUMBER_H_		// このマクロ定義がされてなかったら
#define _NUMBER_H_		// 二重インク―ルード防止のマクロ定義

#include "object2D.h"

//=========================================
// マクロ定義
//=========================================
#define MAX_SCORE		(2)			// スコアの最大数
#define MAX_VERTEX		(4)			// 頂点の最大数

//=========================================
// スコアクラスを定義
//=========================================
class CNumber :public CObject2D
{
public:
	explicit CNumber(int nPriority = LAYER_ONE);	//コンストラクタ
	virtual ~CNumber() override;			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CNumber* Create(const D3DXVECTOR3 &pos);
	static void Add(int nNumScore, int nValue);

	//スコアの設定
	void Set(int nDigits);

private:
	int m_Score;			//スコアの値
	int m_Digit;
};

#endif