//=========================================
// game.cpp
// Author: �y���m��
//=========================================
#include <algorithm>
#include "application.h"
#include "game.h"
#include "player_manager.h"
#include "enemy_manager.h"
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
#include "mouse.h"

bool CGame::m_bFinish = false;
CMesh* CGame::m_pMesh[3] = {};
CScore* CGame::m_pScore = nullptr;
CPlayerManager* CGame::m_pPlayerManager = nullptr;
CEnemy_Manager* CGame::m_pEnemyManager = nullptr;
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
	CMouse *pMouse = CApplication::GetMouse();

	pMouse->UseSetPosLock(true);

	m_bFinish = false;

	/*CRender *pRender = CApplication::GetRender();
	pRender->SetFog(true, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0001f));*/

	//�^�C�}�[�̐���
	CTime *pTime = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 200.0f, 50.0f, 0.0f));
	pTime->SetAlarm(1,0);

	// �X�R�A�̐���
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 50.0f, 0.0f));

	// �C
	m_pMesh[0] = CMesh::Create(D3DXVECTOR3(0.0f, -350.0f, 0.0f), CMesh::TYPE_SEA);
	m_pMesh[2] = CMesh::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), CMesh::TYPE_WAVE);

	// �v���C���[�}�l�[�W���\�̎擾
	m_pPlayerManager = CPlayerManager::Create(D3DXVECTOR3(0.0f, 1000.0f, 5.0f));
	// �G�l�~�[�̃}�l�[�W���[�̎擾
	m_pEnemyManager = CEnemy_Manager::Create();

	// �X�t�B�A
	CSphere *pSphere = CSphere::Create(D3DXVECTOR3(0.0f, -0.0f, 0.0f));

	// ��
	m_pMesh[1] = CMesh::Create(D3DXVECTOR3(0.0f, -400.0f, 0.0f), CMesh::TYPE_GROUND);

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

	m_pPlayerManager = nullptr;

	Release();
}

//=========================================
// �X�V����
//=========================================
void CGame::Update()
{
	CInputKeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �v���C���[�}�l�[�W���\�̍X�V����
	m_pPlayerManager->Update();

	// �G�l�~�[�}�l�[�W���\�̍X�V����
	m_pEnemyManager->Update();

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
}

//=========================================
// �Q�[���Z�b�g
//=========================================
void CGame::EnemyManage()
{
}

//=========================================
// �X�R�A�̉��Z����
//=========================================
void CGame::Add(int Score)
{
	m_pScore->Add(Score);
}