//=========================================
//
//	エネミーを管理する処理
//	Author:冨所知生
//
//=========================================
#include <algorithm>
#include <assert.h>
#include <string.h>
#include "player3D.h"
#include "enemy_manager.h"
#include "application.h"
#include "sound.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "enemy.h"
#include "debug_proc.h"

//=========================================
// コンストラクタ
//=========================================
CEnemy_Manager::CEnemy_Manager()
{
	m_NowWave = 0;
}

//=========================================
// デストラクタ
//=========================================
CEnemy_Manager::~CEnemy_Manager()
{
}

//========================================
// オブジェクトの初期化処理
//=========================================
HRESULT CEnemy_Manager::Init()
{
	// ファイルの読み込み
	LoadFile();

	return S_OK;
}

//=========================================
// オブジェクトの更新処理
//=========================================
void CEnemy_Manager::Update()
{
	EnemyCreate(m_NowWave);
}

//=========================================
// オブジェクトの終了処理
//=========================================
void CEnemy_Manager::Uninit()
{
}

//=========================================
// オブジェクトの描画処理
//=========================================
void CEnemy_Manager::Draw()
{
}

//=========================================
// オブジェクトのクリエイト
//=========================================
CEnemy_Manager* CEnemy_Manager::Create()
{
	CEnemy_Manager* pEnemy = nullptr;

	pEnemy = new CEnemy_Manager;

	if (pEnemy != nullptr)
	{
		pEnemy->Init();
	}

	return pEnemy;
}

//=========================================
// ファイルを読み込む処理
//=========================================
void CEnemy_Manager::LoadFile()
{
	// 変数宣言
	char aStr[128];
	char aEqual[128] = {};		// ＝読み込み用変数
	int nCntTex = 0;
	int WaveNumber = 0;

	// ファイルの読み込み
	FILE *pFile = fopen("data/FILE/enemy_manage.txt", "r");

	if (pFile != nullptr)
	{
		while (fscanf(pFile, "%s", &aStr[0]) != EOF)
		{// "EOF"を読み込むまで 
			if (strncmp(&aStr[0], "#", 1) == 0)
			{// 一列読み込む
				fgets(&aStr[0], sizeof(aStr), pFile);
			}

			if (strstr(&aStr[0], "MAX_WAVE_ENEMY") != NULL)
			{// ウェーブ数
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_MaxWave);

				m_Wave.resize(m_MaxWave);
			}

			if (strcmp(&aStr[0], "WAVE_SET") == 0)
			{	
				while ((strncmp(&aStr[0], "LOAD_END",8) != 0))
				{
					int nCntEnemySpawn = 0;

					fscanf(pFile, "%s", &aStr[0]);

 					if (strcmp(&aStr[0], "WAVE_NUMBER") == 0)
					{// ウェーブの番号
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &WaveNumber);
					}

					if (strcmp(&aStr[0], "ENEMY_AMOUNT") == 0)
					{// 出現させるエネミーの数
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &nCntEnemySpawn);
						
						// エネミーの数を別途保存する
						m_Wave[WaveNumber].m_Amount = nCntEnemySpawn;
						// エネミーデータをエネミーの数分確保する
						m_Wave[WaveNumber].m_EnemyData.resize(nCntEnemySpawn);
					}

					for (int nCnt = 0; nCnt < nCntEnemySpawn; nCnt++)
					{
						fscanf(pFile, "%s", &aStr[0]);

						D3DXVECTOR3 pos = {};
						int type = 0;
						int time = 0;

						if (strncmp(&aStr[0], "-", 1) == 0)
						{// 一列読み込む
							fgets(&aStr[0], sizeof(aStr), pFile);
						}
						if (strncmp(&aStr[0], "#", 1) == 0)
						{// 一列読み込む
							fgets(&aStr[0], sizeof(aStr), pFile);
						}

						if (strcmp(&aStr[0], "ENEMY_POS") == 0)
						{// 出現させるエネミーの数
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f %f %f", &pos.x,&pos.y,&pos.z);
							fscanf(pFile, "%s", &aStr[0]);
						}
						if (strcmp(&aStr[0], "ENEMY_TYPE") == 0)
						{// 出現させるエネミーの数
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &type);
							fscanf(pFile, "%s", &aStr[0]);
						}
						if (strcmp(&aStr[0], "ENEMY_TIME") == 0)
						{// 出現させるエネミーの数
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &time);
						}

						// ウェーブにエネミーの情報を保存する
						m_Wave[WaveNumber].m_EnemyData[nCnt].pos = pos;
						m_Wave[WaveNumber].m_EnemyData[nCnt].type = type;
						m_Wave[WaveNumber].m_EnemyData[nCnt].time = time;
					}
				}
			}
		}
	}
	else
	{
		assert(false);
	}
}

//=========================================
// エネミーを生成する処理
//=========================================
void CEnemy_Manager::EnemyCreate(int Wave)
{
	int ENEMY_AMOUNT = m_Wave[Wave].m_Amount;

	for (int nCnt = 0; nCnt < ENEMY_AMOUNT; nCnt++)
	{// ウェーブのタイマーがエネミーのタイマーと一致した時、一致したエネミーを生成する
		if (m_Wave[Wave].m_Timer == m_Wave[Wave].m_EnemyData[nCnt].time * 60)
		{
			D3DXVECTOR3 pos = m_Wave[Wave].m_EnemyData[nCnt].pos;
			int	type = m_Wave[Wave].m_EnemyData[nCnt].type;

			// ウェーブにエネミーの情報を保存する
			m_Wave[Wave].m_EnemyList.push_back(CEnemy::Create(pos, (CEnemy::EnemyType)type, Wave));
		}
	}

	m_Wave[Wave].m_Timer++;
	
	// カメラの視点
	CDebugProc::Print("エネミーカウント %d \n", m_Wave[Wave].m_Timer);
}

//=========================================
// エネミーが全滅したか判定する
//=========================================
void CEnemy_Manager::Extinction()
{
	// 現在のウェーブ内のエネミーリストのサイズが0より小さいなら
	if (m_NowWave != m_MaxWave)
	{
		if (m_Wave[m_NowWave].m_EnemyList.size() <= 0)
		{// ウェーブ数を進める
			m_NowWave++;

			if (m_NowWave != m_MaxWave)
			{
				EnemyCreate(m_NowWave);
			}
		}
	}

	if (m_NowWave == m_MaxWave)
	{
		//モードの設定
		CFade::SetFade(CApplication::MODE_RESULT);
	}
}

//=========================================
// エネミーの死亡処理
//=========================================
void CEnemy_Manager::Death(CEnemy* pEnemy)
{
	// pEnemy所属しているウェーブ数の取得
	int nWave = pEnemy->GetWave();

	// リストからpEnemyを削除する
	auto itEnd = std::remove(std::begin(m_Wave[nWave].m_EnemyList), std::end(m_Wave[nWave].m_EnemyList), pEnemy);

	// 本当の意味で要素を取り除く
	m_Wave[nWave].m_EnemyList.erase(itEnd, std::cend(m_Wave[nWave].m_EnemyList));
}