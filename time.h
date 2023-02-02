//=========================================
//
// タイマー処理
// Auther：冨所知生
//
//=========================================
#ifndef _TIME_H_		// このマクロ定義がされてなかったら
#define _TIME_H_		// 二重インク―ルード防止のマクロ定義

#include "object2D.h"
#include "number.h"

#define MAX_DIGITS		(8)			// スコアの桁数

//=========================================
// スコアクラスを定義
//=========================================
class CTime :public CObject2D
{
public:
	CTime();							//コンストラクタ
	virtual ~CTime() override;			//デストラクタ

	// 初期化処理
	HRESULT Init(const D3DXVECTOR3& pos) override;
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 終了処理
	void Uninit(void) override;
	// クリエイト処理
	static CTime* Create(const D3DXVECTOR3 &pos);

	// スコアの設定
	void Set();
	void Add(int nValue);
	void Sub(int nValue);

	// アラーム(終了時間)の設定
	void SetAlarm(int minutes, int seconds);

	static int GetTime() { return m_Time; }
	static int GetSec() { return m_Seconds; }
	static int GetMin() { return m_Minutes; }

private:
	D3DXVECTOR3 m_pos;					// 座標
	CNumber	  *m_pNumber[MAX_DIGITS];	// ナンバー1
	int		   m_Alarm;					// 設定された目標時間
	static int m_Time;					// 始まってからの時間
	static int m_Seconds;				// 現在の時間(秒)
	static int m_Minutes;				// 現在の時間(分)
	DWORD m_StartGameTime;				// ゲームが始まった時間
};

#endif