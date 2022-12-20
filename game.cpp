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

CMesh* CGame::m_pMesh = nullptr;
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
	pPlayerManager = CPlayerManager::Create(D3DXVECTOR3(0.0f, -0.0f, 5.0f));

	CSphere *pSphere = CSphere::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f));
	CScore *pScore = CScore::Create(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
	m_pMesh = CMesh::Create(D3DXVECTOR3(0.0f, -350.0f, 0.0f));
	//CEnemy *pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	//pEnemy->SetMotion("data/MOTION/motion.txt");

	//// �G�l�~�[
	//m_EnemyList.emplace_back(pEnemy);

	return S_OK;
}

//=========================================
// �X�V����
//=========================================
void CGame::Update()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();
	
	// �v���C���[�}�l�[�W���\�̍X�V����
	pPlayerManager->Update();

	//// �Q�[�����I�����邩���Ȃ���
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