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
class CRadar;
class CScore;
class CEnemy_Manager;
class CAlert;
class CPlayerUI;
//=========================================
// クラス
//=========================================
class CGame : public CMode
{
public:
	CGame();
	~CGame();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//初期化処理
	virtual void Uninit() override;							//終了処理
	virtual void Update() override;							//更新処理
	virtual void Draw() override {};						//描画処理

	void EnemyManage();
	void DeleteEnemy(CEnemy* pEnemy);

	static CMesh *GetGround() { return m_pMesh[1]; }
	static CEnemy_Manager *GetEM() { return m_pEnemyManager; }
	static CPlayerManager *GetPM() { return m_pPlayerManager; }
	static CAlert *GetAlert() { return m_pAlert; }
	static CPlayerUI *GetPUI() { return m_PlayerUI; }
	static bool GetFinish() { return m_bFinish; }
	static void Finish() { m_bFinish = true; }
	static void Finish2() { m_bFinish_2 = true; }
	static CScore *GetScore() { return m_pScore; }
	static void Add(int Score);

private:
	static CMesh		  *m_pMesh[3];
	static CPlayerManager *m_pPlayerManager;
	static CEnemy_Manager *m_pEnemyManager;
	static CPlayerUI	*m_PlayerUI;					// プレイヤーのUI
	static CScore *m_pScore;
	static CAlert *m_pAlert;							// アラート
	static bool m_bFinish;
	static bool m_bFinish_2;
	CRadar*				  m_pRadar;
};

#endif	// _GAME_H_