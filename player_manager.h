//=========================================
//
//	player_manager.h
//	Author:冨所知生
//
//=========================================
#ifndef _PLAYER_MANAGE_H_		//このマクロ定義がされてなかったら
#define _PLAYER_MANAGE_H_		//２重インクルード防止のマクロ定義

#include "player3D.h"

class CPlayer3D;
class CPlayerManager
{
public:
	enum PLAYER_MODE
	{
		MODE_FLY = 0,
		MODE_ROBOT,
		MODE_MAX
	};

	CPlayerManager();			//コンストラクタ
	virtual ~CPlayerManager();			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
	// 終了処理
	void Uninit(void);
	// モードの取得
	static PLAYER_MODE GetMode() { return m_mode; }
	static CPlayer3D* GetPlayer() { return m_pFly; }
	static CPlayer3D* GetRobot() { return m_pRobot; }
	// クリエイト処理
	static CPlayerManager* Create(const D3DXVECTOR3 &pos);

private:
	static CPlayer3D* m_pFly;				// 飛行形態の処理
	static CPlayer3D* m_pRobot;				// ロボット形態の処理

	static PLAYER_MODE m_mode;		// プレイヤーの状態を管理する

	static bool m_bTransform;				// 変形
};

#endif