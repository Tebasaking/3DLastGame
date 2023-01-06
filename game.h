//=========================================
//
//	game.h
//	Author:�y���m��
//
//=========================================
#ifndef _GAME_H_		//���̃}�N����`������ĂȂ�������
#define _GAME_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "mode.h"
#include <vector>

class CPlayer3D;
class CMesh;
class CPlayerManager;
class CEnemy;
class CCamera;
//=========================================
// �N���X
//=========================================
class CGame : public CMode
{
public:
	CGame();
	~CGame();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	//����������
	virtual void Uninit() override;							//�I������
	virtual void Update() override;							//�X�V����
	virtual void Draw() override {};						//�`�揈��

	void EnemyManage();
	void DeleteEnemy(CEnemy* pEnemy);

	static CMesh *GetGround() { return m_pMesh[1]; }

private:
	static CMesh		  *m_pMesh[3];
	static CPlayerManager *pPlayerManager;
	std::vector<CEnemy*>  m_EnemyList;
};

#endif	// _GAME_H_