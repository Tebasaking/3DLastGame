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

CMesh* CGame::m_pMesh[3] = {};
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
	pPlayerManager = CPlayerManager::Create(D3DXVECTOR3(0.0f, 1000.0f, 5.0f));

	CSphere *pSphere = CSphere::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f));
	//CScore *pScore = CScore::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
	
	// �C
	m_pMesh[0] = CMesh::Create(D3DXVECTOR3(0.0f, -350.0f, 0.0f), CMesh::TYPE_SEA);

	//// ��
	//m_pMesh[1] = CMesh::Create(D3DXVECTOR3(0.0f, -400.0f, 0.0f), CMesh::TYPE_GROUND);

	// �퓬�@
	CEnemy *pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 1000.0, 100.0f));
	pEnemy->SetType(CEnemy::ENEMY_FLY);
	pEnemy->SetMotion("data/MOTION/fly_motion.txt");

	// �^���N
	CEnemy *pTank = CEnemy::Create(D3DXVECTOR3(100.0f,0.0f,1000.0f));
	pTank->SetType(CEnemy::ENEMY_GROUND);
	pTank->SetMotion("data/MOTION/tank.txt");

	// �G�l�~�[
	m_EnemyList.emplace_back(pEnemy);

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