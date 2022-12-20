//=============================================================================
//
// フェード処理
// Author:髙野馨將
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "object2D.h"


//前方宣言
class CMode;

//フェードクラス
class CFade : public CObject2D
{
public:
	enum FADE			//フェード状態
	{
		FADE_NONE = 0,	//何もしてない状態
		FADE_IN,		//フェードイン状態
		FADE_OUT,		//フェードアウト状態
		FADE_MAX
	};

	explicit CFade(int nPriority = 4);	//コンストラクタ
	~CFade();									//デストラクタ

	void Init(CApplication::MODE modeNext);		//フェードの初期化
	void Uninit(void) override;					//フェードの終了
	void Update(void) override;					//フェードの更新
	void Draw(void) override;					//フェードの描画

	static void SetFade(CApplication::MODE modeNext);	//フェードの設定
	static FADE GetFade() { return m_pFade; }

	static CFade *Create(CApplication::MODE modeNext);

private:
	static CApplication::MODE m_ModeNext;	//次の画面(モード)
	static FADE m_pFade;					//フェードの状態
	static D3DXCOLOR m_Color;				//カラー
};
#endif