//=========================================
//
//	�G�l�~�[���Ǘ����鏈��
//	Author:�y���m��
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
// �R���X�g���N�^
//=========================================
CEnemy_Manager::CEnemy_Manager()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CEnemy_Manager::~CEnemy_Manager()
{
}

//========================================
// �I�u�W�F�N�g�̏���������
//=========================================
HRESULT CEnemy_Manager::Init()
{
	LoadFile();
	return S_OK;
}

//=========================================
// �I�u�W�F�N�g�̍X�V����
//=========================================
void CEnemy_Manager::Update()
{
}

//=========================================
// �I�u�W�F�N�g�̏I������
//=========================================
void CEnemy_Manager::Uninit()
{
}

//=========================================
// �I�u�W�F�N�g�̕`�揈��
//=========================================
void CEnemy_Manager::Draw()
{
}

//=========================================
// �I�u�W�F�N�g�̃N���G�C�g
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
// �t�@�C����ǂݍ��ޏ���
//=========================================
void CEnemy_Manager::LoadFile()
{
	// �ϐ��錾
	char aStr[128];
	char aEqual[128] = {};		// ���ǂݍ��ݗp�ϐ�
	int nCntTex = 0;
	int WaveNumber = 0;

	// �t�@�C���̓ǂݍ���
	FILE *pFile = fopen("data/FILE/enemy_manage.txt", "r");

	if (pFile != nullptr)
	{
		while (fscanf(pFile, "%s", &aStr[0]) != EOF)
		{// "EOF"��ǂݍ��ނ܂� 
			if (strncmp(&aStr[0], "#", 1) == 0)
			{// ���ǂݍ���
				fgets(&aStr[0], sizeof(aStr), pFile);
			}

			if (strstr(&aStr[0], "MAX_WAVE_ENEMY") != NULL)
			{// �E�F�[�u��
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
					{// �E�F�[�u�̔ԍ�
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &WaveNumber);
					}

					if (strcmp(&aStr[0], "ENEMY_AMOUNT") == 0)
					{// �o��������G�l�~�[�̐�
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &nCntEnemySpawn);
					}

					for (int nCnt = 0; nCnt < nCntEnemySpawn; nCnt++)
					{
						fscanf(pFile, "%s", &aStr[0]);

						// �E�F�[�u�ɃG�l�~�[�̏���ۑ�����
						m_Wave[WaveNumber].m_EnemyList.push_back(CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));

						D3DXVECTOR3 pos = {};
						int	type = 0;

						if (strcmp(&aStr[0], "ENEMY_POS") == 0)
						{// �o��������G�l�~�[�̐�
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f %f %f", &pos.x,&pos.y,&pos.z);
							
							// ���W�̐ݒ�
							m_Wave[WaveNumber].m_EnemyList[nCnt]->SetPosition(pos);
							fscanf(pFile, "%s", &aStr[0]);
						}
						if (strcmp(&aStr[0], "ENEMY_TYPE") == 0)
						{// �o��������G�l�~�[�̐�
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &type);

							// �^�C�v�̐ݒ�
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