//=========================================
//
//	time_over.h
//	Author:冨所知生
//
//=========================================
#ifndef _GAME_OVER_H_		//このマクロ定義がされてなかったら
#define _GAME_OVER_H_		//２重インクルード防止のマクロ定義

#include "mode.h"

//=========================================
// クラス
//=========================================
class CGameOver : public CMode
{
public:
	CGameOver();
	~CGameOver();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//初期化処理
	virtual void Uninit() override;							//終了処理
	virtual void Update() override;							//更新処理
	virtual void Draw() override {};						//描画処理
};

#endif	// _RESULT_H_