//=========================================
//
//	title.h
//	Author:冨所知生
//
//=========================================
#ifndef _TUTORIAL_H_		//このマクロ定義がされてなかったら
#define _TUTORIAL_H_		//２重インクルード防止のマクロ定義

#include "mode.h"

//=========================================
// クラス
//=========================================
class CObject2D;
class CTutorial : public CMode
{
public:
	CTutorial();
	~CTutorial();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//初期化処理
	virtual void Uninit() override { Release(); }					//終了処理
	virtual void Update() override;						//更新処理
	virtual void Draw() override {};					//描画処理

private:
	CObject2D *pObject2D[3];
	int		m_Select;
	int		m_SelectCnt;
};

#endif	// _TITLE_H_