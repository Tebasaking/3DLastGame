//=========================================
//
//	game.h
//	Author:冨所知生
//
//=========================================
#ifndef _TUTORIAL_H_		//このマクロ定義がされてなかったら
#define _TUTORIAL_H_		//２重インクルード防止のマクロ定義

#include "mode.h"

//=========================================
// クラス
//=========================================
class CTutorial : public CMode
{
public:
	CTutorial();
	~CTutorial();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//初期化処理
	virtual void Uninit() override;						//終了処理
	virtual void Update() override;						//更新処理
	virtual void Draw() override {};					//描画処理
};

#endif	// _RESULT_H_