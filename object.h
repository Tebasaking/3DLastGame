//=========================================
//
//	object.h
//	Author:�y���m��
//
//
//=========================================
#ifndef _OBJECT_H_		//���̃}�N����`������ĂȂ�������
#define _OBJECT_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`
#define MAX_OBJECT (10000)

#include "main.h"

class CObject
{
public:
	//�I�u�W�F�N�g�̎��
	enum EObjType
	{
		OBJECT_NONE,		// �����^�C�v
		OBJECT_PLAYER,		// �v���C���[
		OBJECT_ENEMY,		// �G�l�~�[
		OBJECT_BULLET,		// �e�̎��
		OBJECT_RADAR,		// ���[�_�[�̃I�u�W�F�N�g
		OBJECT_EMPTY,		// �ݒ肳��Ă��Ȃ�
		OBJECT_FADE,
		OBJECT_MAX
	};

	enum Priority
	{
		LAYER_ONE,
		LAYER_TOW,
		LAYER_THREE,
		LAYER_FOUR,
		LAYER_FIVE,
		LAYER_MAX
	};

	enum Object_mode
	{
		NORMAL_MODE,
		RADAR_MODE,
		MAX_MODE
	};

	explicit CObject(int nPriority = LAYER_ONE);			//�R���X�g���N�^
	virtual ~CObject();			//�f�X�g���N�^

	virtual HRESULT Init(const D3DXVECTOR3 &pos) = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0; 

	static void UpdateAll();
	static void DrawAll(Object_mode mode);
	static void UninitAll();

	const D3DXVECTOR3& GetPosition() const { return m_pos; }
	const D3DXVECTOR3& GetRot() const { return m_rot; }
	const float& GetScale() const { return m_scale; }
	const D3DXVECTOR3& GetSize() const { return m_size; }

	// �Q�b�^�[
	CObject	*GetObjectinfo() { return this; }
	int		GetNumber() { return m_Number; }
	int		GetHP() { return m_HP; }
	bool	GetTargeting() { return m_Targeting; }
	bool	GetCollision() { return m_Collision; }
	static  CObject *GetObjectTop() { return m_pTop[0]; }
	static  CObject *GetObjectType(CObject::EObjType type);
	CObject *GetObjectNext() { return m_pNext; }
	EObjType GetObjectType() { return m_type; }

	// �Z�b�^�[
	void SetHP(int HP) { m_HP = HP; }
	void SetPosition(const D3DXVECTOR3& pos);
	void SetObjectType(EObjType type) { m_type = type; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	void SetCollision(bool bCollision) { m_Collision = bCollision; }

	// �}�g���b�N�X
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }				// ���[���h�}�g���b�N�X�̃Z�b�^�[
	D3DXMATRIX *GetMtxWorld() { return &m_mtxWorld; }								// ���[���h�}�g���b�N�X�̃Q�b�^�[

	void Release();
	void DFlagDelete();
	int ManageHP(int Value);

	bool Collision(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3 posTarget, D3DXVECTOR3 size, D3DXVECTOR3 SizeTarget, bool bExtrude);
	// ����: �ڕW�̍��W �A ��̒��S �A �͈�(�p�x) �A���� , ����(�p�x)
	bool SearchEye(D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2, float rangeRadian, float length, float directionDegree);

protected:
	static int	m_nNumObject;
	static int m_nNumAll;
	int m_nID;
	int m_Number;								// ���Ԗڂ̃I�u�W�F�N�g��(�ԍ��J�E���g�͊e����)

private:
	bool m_Targeting;							// �^�[�Q�b�g�̑ΏۂɂȂ��Ă��邩���Ȃ���
	bool m_DeathFlag;							// ���S�t���O
	bool bCollision;							// �����蔻��
	int m_Prioryty;								// �v���C�I���e�B
	static CObject *m_pTop[LAYER_MAX];			// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject *m_pCurrent[LAYER_MAX];		// ����(��Ԍ��)�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pNext;							// �����̎��̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pPrev;							// �����̈�O�̃I�u�W�F�N�g�ւ̃|�C���^
	EObjType m_type;
	D3DXVECTOR3 m_pos;							// ���W�ݒ�
	D3DXVECTOR3 m_size;							// �傫���ݒ�
	D3DXVECTOR3 m_rot;							// ��]�ݒ�
	D3DXMATRIX	m_mtxWorld;						// ���[���h�}�g���b�N�X
	Object_mode m_objmode;						// �I�u�W�F�N�g�̕`�惂�[�h
	float		m_scale;						// �g�嗦�ݒ�
	int			m_HP;							// �̗�
	bool		m_Collision;					// �����蔻��
};

#endif