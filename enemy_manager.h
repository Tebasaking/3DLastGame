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
		D3DXVECTOR3 pos;
		int			type;
	}EnemyData;

	typedef struct
	{
		int m_Amount;		// そのウェーブ中に出現するエネミーの数
		std::vector<EnemyData> m_EnemyData;
		std::vector<CEnemy*> m_EnemyList;
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
	// 読み込んだウェーブの情報を元にエネミーを生成する
	void EnemyCreate(int Wave);
	// ウェーブ内のエネミーが全滅した時
	void Extinction();
	// ウェーブ内のエネミーが死亡した処理
	void Death(CEnemy* pEnemy);

private:
	std::vector<Wave> m_Wave;
	int m_MaxWave;				// ウェーブの最大数
	int m_NowWave;				// 現在のウェーブ数
};

#endif