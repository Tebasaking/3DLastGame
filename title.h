//=========================================
//
//	title.h
//	Author:冨所知生
//
//=========================================
#ifndef _TITLE_H_		//このマクロ定義がされてなかったら
#define _TITLE_H_		//２重インクルード防止のマクロ定義

#include "mode.h"

//=========================================
// クラス
//=========================================
class CObject2D;
class CTitle : public CMode
{
public:
	CTitle();
	~CTitle();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//初期化処理
	virtual void Uninit() override;						//終了処理
	virtual void Update() override;						//更新処理
	virtual void Draw() override {};					//描画処理

private:
	static CMesh		  *m_pMesh[3];
	CObject2D *pObject2D[4];
	int		m_Select;
	bool	m_bSelect;
	int		m_SelectCnt;
	bool	m_bFade;
};

#endif	// _TITLE_H_