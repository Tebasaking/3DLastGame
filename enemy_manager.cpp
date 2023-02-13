//=========================================
//
//	エネミーを管理する処理
//	Author:冨所知生
//
//=========================================
#include <assert.h>
#include <string.h>
#include "player3D.h"
#include "enemy_manager.h"
#include "application.h"
#include "sound.h"
#include "inputkeyboard.h"
#include "enemy.h"

//=========================================
// コンストラクタ
//=========================================
CEnemy_Manager::CEnemy_Manager()
{
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
	LoadFile();
	return S_OK;
}

//=========================================
// オブジェクトの更新処理
//=========================================
void CEnemy_Manager::Update()
{
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
				int MAX_WAVE;
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &MAX_WAVE);

				m_Wave.resize(MAX_WAVE);
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
					}

					for (int nCnt = 0; nCnt < nCntEnemySpawn; nCnt++)
					{
						fscanf(pFile, "%s", &aStr[0]);

						// ウェーブにエネミーの情報を保存する
						m_Wave[WaveNumber].m_EnemyList.push_back(CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));

						D3DXVECTOR3 pos = {};
						int	type = 0;

						if (strcmp(&aStr[0], "ENEMY_POS") == 0)
						{// 出現させるエネミーの数
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f %f %f", &pos.x,&pos.y,&pos.z);
							
							// 座標の設定
							m_Wave[WaveNumber].m_EnemyList[nCnt]->SetPosition(pos);
							fscanf(pFile, "%s", &aStr[0]);
						}
						if (strcmp(&aStr[0], "ENEMY_TYPE") == 0)
						{// 出現させるエネミーの数
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &type);

							// タイプの設定
							m_Wave[WaveNumber].m_EnemyList[nCnt]->SetType((CEnemy::EnemyType)type);
						}
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