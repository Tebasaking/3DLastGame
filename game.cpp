//=========================================
// game.cpp
// Author: �y���m��
//=========================================
#include <algorithm>
#include "application.h"
#include "game.h"
#include "player_manager.h"
#include "sphere.h"
#include "meshfield.h"
#include "score.h"
#include "inputkeyboard.h"
#include "fade.h"
#include "enemy.h"
#include "camera.h"
#include "radar.h"
#include "number.h"
#include "render.h"
#include "time.h"
#include "score.h"
#include "sound.h"

bool CGame::m_bFinish = false;
CMesh* CGame::m_pMesh[3] = {};
CScore* CGame::m_pScore = nullptr;
CPlayerManager* CGame::pPlayerManager = nullptr;
//=========================================
// �R���X�g���N�^
//=========================================
CGame::CGame()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CGame::~CGame()
{
}

//=========================================
// ������ 
//=========================================
HRESULT CGame::Init(const D3DXVECTOR3 &pos)
{
	m_bFinish = false;

	/*CRender *pRender = CApplication::GetRender();
	pRender->SetFog(true, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0001f));*/

	//�^�C�}�[�̐���
	CTime *pTime = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 200.0f, 50.0f, 0.0f));
	pTime->SetAlarm(3,0);

	// �X�R�A�̐���
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 50.0f, 0.0f));

	// �C
	m_pMesh[0] = CMesh::Create(D3DXVECTOR3(0.0f, -350.0f, 0.0f), CMesh::TYPE_SEA);
	m_pMesh[2] = CMesh::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), CMesh::TYPE_WAVE);

	pPlayerManager = CPlayerManager::Create(D3DXVECTOR3(0.0f, 1000.0f, 5.0f));

	CSphere *pSphere = CSphere::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f));
	//CScore *pScore = CScore::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

	// ��
	m_pMesh[1] = CMesh::Create(D3DXVECTOR3(0.0f, -400.0f, 0.0f), CMesh::TYPE_GROUND);

	CEnemy *pEnemy[3] = {};

	// �퓬�@
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pEnemy[nCnt] = CEnemy::Create(D3DXVECTOR3(nCnt *(rand() % 100 + 100), 1000.0f, 300.0f ));
		pEnemy[nCnt]->SetType(CEnemy::ENEMY_FLY);
		pEnemy[nCnt]->SetMotion("data/MOTION/fly_motion.txt");
	}

	// �^���N
	pEnemy[2] = CEnemy::Create(D3DXVECTOR3(100.0f,1000.0f,300.0f));
	pEnemy[2]->SetType(CEnemy::ENEMY_GROUND);
	pEnemy[2]->SetMotion("data/MOTION/tank.txt");

	m_pRadar = nullptr;
	m_pRadar = CRadar::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetObjectinfo(), CRadar::RADAR_MAP);

	// �G�l�~�[
	//m_EnemyList.emplace_back(pEnemy);

	//�T�E���h����
	CSound::PlaySound(CSound::SOUND_LABEL_BGM000);

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CGame::Uninit()
{
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_pMesh[nCnt] = nullptr;
	}

	// �X�R�A�̏I������
	m_pScore->Uninit();
	m_pScore = nullptr;

	pPlayerManager = nullptr;

	Release();
}

//=========================================
// �X�V����
//=========================================
void CGame::Update()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �v���C���[�}�l�[�W���\�̍X�V����
	pPlayerManager->Update();

	// �Q�[�����I�����邩���Ȃ���
	//EnemyManage();

	if (m_bFinish)
	{
		// �Q�[�����I�������U���g��ʂ�
		//���[�h�̐ݒ�
		CFade::SetFade(CApplication::MODE_RESULT);
	}
}

//=========================================
// �G�l�~�[���폜����
//=========================================
void CGame::DeleteEnemy(CEnemy* pEnemy)
{
	// ���X�g����pEnemy���폜����
	auto itEnd = std::remove(std::begin(m_EnemyList), std::end(m_EnemyList), pEnemy);

	// �{���̈Ӗ��ŗv�f����菜��
	m_EnemyList.erase(itEnd, std::cend(m_EnemyList)); 
}

//=========================================
// �Q�[���Z�b�g
//=========================================
void CGame::EnemyManage()
{
	if (m_EnemyList.size() <= 0)
	{
      	CFade::SetFade(CApplication::MODE_RESULT);
	}
}

//=========================================
// �X�R�A�̉��Z����
//=========================================
void CGame::Add(int Score)
{
	m_pScore->Add(Score);
}