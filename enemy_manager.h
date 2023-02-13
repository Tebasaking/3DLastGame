//=========================================
//
//	enemy_manage.h
//	Author:冨所知生
//
//=========================================
#ifndef _ENEMY_MANAGE_H_		//このマクロ定義がされてなかったら
#define _ENEMY_MANAGE_H_		//２重インクルード防止のマクロ定義

#include "object.h"
#include <vector>

#include "motion_model3D.h"

class CEnemy;
class CEnemy_Manager
{
public:
	typedef struct
	{
		std::vector<CEnemy*>  m_EnemyList;
	}Wave;

	CEnemy_Manager();			//コンストラクタ
	~CEnemy_Manager();			//デストラクタ

	// 初期化処理
	HRESULT Init();
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
	// 終了処理
	void Uninit(void);
	// クリエイト処理
	static CEnemy_Manager *Create(void);
	// ファイルを読み込む処理
	void LoadFile();

private:
	std::vector<Wave> m_Wave;
};

#endif