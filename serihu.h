//=========================================
//
// セリフの処理
// 補足 : セリフの英語調べたらLineだったけどわかりにくいので
//			ここだけローマ字でserihu.h,cppです；；
// Auther：冨所知生
//
//=========================================
#ifndef _SERIHU_H_		// このマクロ定義がされてなかったら
#define _SERIHU_H_		// 二重インク―ルード防止のマクロ定義

#include "object2D.h"

//=========================================
// スコアクラスを定義
//=========================================
class CSerihu :public CObject2D
{
public:
	enum TYPE			//フェード状態
	{
		NONE,			// ナシ
		STOP,			// 「 作戦範囲外だ、範囲内に戻れ。 」
		SHOUT_DOWN,		// 「 敵機を一機撃墜。 」
	};

	explicit CSerihu(int nPriority = LAYER_FIVE);	//コンストラクタ
	virtual ~CSerihu() override;			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CSerihu* Create(const TYPE type);

	//スコアの設定
	void SetType(TYPE type);
	// ナンバーの大きさの設定

private:
	TYPE m_type;
};

#endif