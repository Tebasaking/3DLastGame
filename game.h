//=========================================
//
//	game.h
//	Author:冨所知生
//
//=========================================
#ifndef _GAME_H_		//このマクロ定義がされてなかったら
#define _GAME_H_		//２重インクルード防止のマクロ定義

#include "mode.h"
#include <vector>

class CPlayer3D;
class CMesh;
class CPlayerManager;
class CEnemy;
//=========================================
// クラス
//=========================================
class CGame : public CMode
{
public:
	CGame();
	~CGame();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//初期化処理
	virtual void Uninit() override { Release(); }					//終了処理
	virtual void Update() override;						//更新処理
	virtual void Draw() override {};					//描画処理

	void EnemyManage();

	void DeleteEnemy(CEnemy* pEnemy);
	static CMesh* GetMesh() { return m_pMesh; }

private:
	static CMesh	*m_pMesh;
	static CPlayerManager *pPlayerManager;
	std::vector<CEnemy*> m_EnemyList;
};

#endif	// _GAME_H_