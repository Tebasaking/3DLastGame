//=========================================
//
//	object2D.h
//	Author:�y���m��
//
//=========================================
#ifndef _MESH_FIELD_H_		//���̃}�N����`������ĂȂ�������
#define _MESH_FIELD_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"
#include "object.h"
#include "texture.h"
#include "Object3D.h"

#define MESH_SIZE (1000.0f)
#define MESH_X_BLOCK (20)
#define MESH_Z_BLOCK (20)
#define MESH_VERTEX_NUM ((MESH_X_BLOCK + 1) * (MESH_Z_BLOCK + 1))
#define MESH_INDEX_NUM  (((MESH_X_BLOCK + 1) * 2) * ( MESH_Z_BLOCK  *(MESH_Z_BLOCK - 1)) * MESH_Z_BLOCK * 2)
#define MESH_PRIMITIVE_NUM (MESH_X_BLOCK * MESH_Z_BLOCK * 2 + 4 * (MESH_Z_BLOCK  - 1))							//�|���S���̐��H

class CMesh :public CObject
{
public:

	enum MeshType
	{
		TYPE_SEA = 0,
		TYPE_GROUND,
	};

	CMesh();			//�R���X�g���N�^
	virtual ~CMesh();			//�f�X�g���N�^

	//����������
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//�X�V����
	void Update(void) override;
	//�`�揈��
	void Draw(void) override;
	//�I������
	void Uninit(void) override;
	//�N���G�C�g����
	static CMesh* Create(const D3DXVECTOR3 &pos, MeshType type);
	//�e�N�X�`���̐ݒ菈��
	void SetTexture(CTexture::TEXTURE texture);	// �e�N�X�`���̐ݒ�
	
	void SetPosition(const D3DXVECTOR3& pos);
	void SetAnim(const float Num, const int Pattern);
	void SetSize(const float Size);

	void NorCreate();								// ���b�V���t�B�[���h�̖@���쐬
	bool Collision(D3DXVECTOR3 *pos);				// ���菈��

	// ���b�V���^�C�v�̐ݒ�
	void SetType(MeshType type) { m_type = type; }
	

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//�o�b�t�@
	LPDIRECT3DINDEXBUFFER9  m_pIdxVtxBuff;	//���b�V���t�B�[���h�o�b�t�@
	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_pos;			// ���W
	CTexture::TEXTURE m_texture;			// �e�N�X�`���̗񋓌^
	D3DXVECTOR3 m_size;						// �傫��
	float m_scale;							// �g�嗦
	float m_polygon;						// �|���S����
	D3DXVECTOR3 m_rot;
	MeshType	m_type;						// ���b�V���̃^�C�v
	// �@���i�[
	D3DXVECTOR3	m_norBox[MESH_PRIMITIVE_NUM];
};

#endif