//=========================================
//
//	�r���{�[�h�̏���
//	Author:�y���m��
//
//=========================================
#include "application.h"
#include "meshfield.h"
#include "texture.h"
#include "calculation.h"

int nLineVtx = (MESH_X_BLOCK + 1);
//=========================================
//�R���X�g���N�^
//=========================================
CMesh::CMesh()
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_texture = CTexture::TEXTURE_SEA;
}

//=========================================
//�f�X�g���N�^
//=========================================
CMesh::~CMesh()
{

}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CMesh::Init(const D3DXVECTOR3 &pos)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESH_VERTEX_NUM,	//�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,													//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESH_INDEX_NUM,			//�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,													//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pIdxVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = pos;

	for (int nCntZ = 0; nCntZ <= MESH_Z_BLOCK; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= MESH_X_BLOCK; nCntX++)
		{
			//���_���W�̐ݒ�i���[���h���W�ł͂Ȃ����[�J�����W���w�肷��j
			pVtx[nCntX + (MESH_X_BLOCK + 1) * nCntZ].pos = D3DXVECTOR3
			(-(MESH_SIZE * MESH_X_BLOCK) / 2 + nCntX * MESH_SIZE,					// x
				sinf(nCntX * D3DX_PI * 0.25f) * 10.0f,								// y
				(MESH_SIZE * MESH_Z_BLOCK) / 2 - nCntZ * MESH_SIZE);				// z

			//�e���_�̖@���̐ݒ�
			pVtx[nCntX + (MESH_X_BLOCK + 1) * nCntZ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[nCntX + (MESH_X_BLOCK + 1) * nCntZ].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`���̐ݒ�
			pVtx[nCntX + (MESH_X_BLOCK + 1) * nCntZ].tex = D3DXVECTOR2((float)nCntX, (float)nCntZ);
		}
	}

	// ���_���A�����b�N����
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxVtxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//�c�̒��_�̐�
	for (int nCntZ = 0; nCntZ < MESH_Z_BLOCK; nCntZ++)
	{
		//�C���f�b�N�X
		int nLineTop = nCntZ * (nLineVtx * 2 + 2);

		//���̒��_�̐�
		for (int nCntX = 0; nCntX < nLineVtx; nCntX++)
		{//��̒i�̏���
			pIdx[(nCntX * 2 + 1) + nLineTop] = (WORD)(nCntX + nLineVtx * nCntZ);

			//���̒i�̏���
			pIdx[nCntX * 2 + nLineTop] = (WORD)(pIdx[(nCntX * 2 + 1) + nLineTop] + nLineVtx);
		}

		//�k�ރ|���S����O��Ɠ���ɂ��鏈��
		if (nCntZ < MESH_Z_BLOCK)
		{
			pIdx[(nLineVtx * 2) + nLineTop] = (WORD)(MESH_X_BLOCK + (nLineVtx)* nCntZ);
			pIdx[(nLineVtx * 2) + nLineTop + 1] = (WORD)(2 * (nLineVtx)+(nLineVtx)* nCntZ);
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxVtxBuff->Unlock();

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CMesh::Update()
{
	NorCreate();
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CMesh::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	if (m_pIdxVtxBuff != nullptr)
	{
		m_pIdxVtxBuff->Release();
		m_pIdxVtxBuff = nullptr;
	}

	CObject::Release();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CMesh::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();
	CTexture* pTexture = CApplication::GetTexture();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;

	// ���[���h�}�g���b�N�X�̏�����
	// �s�񏉊����֐�(��������[�s��]��[�P�ʍs��]�ɏ�����)
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �����̔��f
	// �s���]�֐� (��������[���[(y)�s�b�`(x)���[��(z)]�����̉�]�s����쐬)
	D3DXMatrixRotationZ(&mtxRot, -m_rot.z);

	// �s��|���Z�֐� (������ * ��O������������Ɋi�[)
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	// �s��ړ��֐� (��������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);		// �s��|���Z�֐�

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxVtxBuff);

	// ���b�V���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESH_VERTEX_NUM, 0, MESH_PRIMITIVE_NUM);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, nullptr);
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CMesh* CMesh::Create(const D3DXVECTOR3 &pos)
{
	int nNumCreate = m_nNumAll;

	CMesh* pCMesh = nullptr;

	pCMesh = new CMesh;

	if (pCMesh != nullptr)
	{
		pCMesh->Init(pos);
	}

	return pCMesh;
}

//=========================================
//���W�̐ݒ菈��
//=========================================
void CMesh::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;
}

//=========================================
// �e�N�X�`���̐ݒ�
//=========================================
void CMesh::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//=========================================
//�T�C�Y�̐ݒ�
//=========================================
void CMesh::SetSize(const float size)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	m_scale = size;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-size, size, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size, size, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size, -size, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size, -size, 0.0f);

	// ���_���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=========================================
//�e�N�X�`���̐ݒ�
//=========================================
void CMesh::SetAnim(const float Num, const int Pattern)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	float PatternSize = 1.0f / Pattern;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//tex�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(Num * PatternSize, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(Num * PatternSize + PatternSize, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(Num * PatternSize, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(Num * PatternSize + PatternSize, 1.0f);

	// ���_���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=========================================
//�I�u�W�F�N�g�̔��菈��
//=========================================
bool CMesh::Collision(D3DXVECTOR3 *pos)
{// �����蔻��
	bool bCollison = false;

	// ���_���̎擾
	VERTEX_3D *pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �C���f�b�N�X�o�b�t�@�����b�N
	WORD *pIdx;
	m_pIdxVtxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �^�[�Q�b�g���̐錾
	D3DXVECTOR3 posTarget = *pos;

	for (int nCntPolygon = 0; nCntPolygon < MESH_PRIMITIVE_NUM; nCntPolygon++)
	{
		// �ϐ��錾
		D3DXVECTOR3 V1 = pVtx[pIdx[nCntPolygon]].pos;
		D3DXVECTOR3 V2 = pVtx[pIdx[nCntPolygon + 1]].pos;
		D3DXVECTOR3 V3 = pVtx[pIdx[nCntPolygon + 2]].pos;

		V1 = WorldCastVtx(V1,m_pos,m_rot);
		V2 = WorldCastVtx(V2, m_pos, m_rot);
		V3 = WorldCastVtx(V3, m_pos, m_rot);

		if (V1 == V2
			|| V1 == V3
			|| V2 == V3)
		{// �k�ރ|���S���̏ꍇ
			continue;
		}

		// �|���S���̕Ӄx�N�g��
		D3DXVECTOR3 P1 = V2 - V1;
		D3DXVECTOR3 P2 = V3 - V2;
		D3DXVECTOR3 P3 = V1 - V3;

		// ���_�ƃ^�[�Q�b�g�̃x�N�g��
		D3DXVECTOR3 VecA = posTarget - V1;
		D3DXVECTOR3 VecB = posTarget - V2;
		D3DXVECTOR3 VecC = posTarget - V3;

		// ��r���Z�p�̕ϐ��̒�`�Ƒ��
		float fA = (P1.x * VecA.z) - (P1.z * VecA.x);
		float fB = (P2.x * VecB.z) - (P2.z * VecB.x);
		float fC = (P3.x * VecC.z) - (P3.z * VecC.x);

		if ((0.0f < fA && 0.0f < fB && 0.0f < fC)
			|| (0.0f > fA && 0.0f > fB && 0.0f > fC))
		{// ����̐ݒ�
			bCollison = true;

			// �ʖ@���x�N�g��
			D3DXVECTOR3 norVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// �ʖ@���x�N�g��
			D3DXVec3Cross(&norVec, &P1, &P2);

			//���K��
			D3DXVec3Normalize(&norVec, &norVec);

			// �ʒu�̐ݒ�
			pos->y = V1.y - ((posTarget.x - V1.x) * norVec.x + (posTarget.z - V1.z) * norVec.z) / norVec.y;

			float randcolr = (rand() % 10) * 1.0f;
			float randcolg = (rand() % 10) * 1.0f;
			float randcolb = (rand() % 10) * 1.0f;
			pVtx[pIdx[nCntPolygon]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[pIdx[nCntPolygon + 1]].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[pIdx[nCntPolygon + 2]].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		}
	}

	// �C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxVtxBuff->Unlock();

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();

	return bCollison;
}

//=========================================
// ���b�V���t�B�[���h�̖@���쐧
//=========================================
void CMesh::NorCreate()
{
	// �ʖ@���x�N�g�����쐬����

	// ���_���̎擾
	VERTEX_3D *pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �C���f�b�N�X�o�b�t�@�����b�N
	WORD *pIdx;
	m_pIdxVtxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MESH_PRIMITIVE_NUM; nCntPolygon++)
	{
		if (pIdx[nCntPolygon] == pIdx[nCntPolygon + 1] ||
			pIdx[nCntPolygon + 1] == pIdx[nCntPolygon + 2] ||
			pIdx[nCntPolygon + 2] == pIdx[nCntPolygon])
		{
			continue;
		}

		D3DXVECTOR3 V[3] = {};

		V[0] = pVtx[pIdx[nCntPolygon ]].pos;
		V[1] = pVtx[pIdx[nCntPolygon  + 1]].pos;
		V[2] = pVtx[pIdx[nCntPolygon  + 2]].pos;

		V[0] = WorldCastVtx(V[0], m_pos, m_rot);
		V[1] = WorldCastVtx(V[1], m_pos, m_rot);
		V[2] = WorldCastVtx(V[2], m_pos, m_rot);

		// �|���S���̕Ӄx�N�g��
		D3DXVECTOR3 P[3] = {};

		// �Ӄx�N�g���̎Z�o
		P[0] = V[1] - V[0];
		P[1] = V[2] - V[0];

		// �ʖ@���x�N�g��
		D3DXVECTOR3 Nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (nCntPolygon % 2 == 0)
		{// �ʖ@���x�N�g��
			D3DXVec3Cross(&Nor, &P[0], &P[1]);
		}
		else
		{// �ʖ@���x�N�g��
			D3DXVec3Cross(&Nor, &P[1], &P[0]);
		}

		//���K��
		D3DXVec3Normalize(&Nor, &Nor);

		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			// �@����ۑ�����
			pVtx[pIdx[nCntPolygon + nCnt]].nor += Nor;

			// �x�N�g���𐳋K������
			D3DXVec3Normalize(&pVtx[pIdx[nCntPolygon + nCnt]].nor, &pVtx[pIdx[nCntPolygon + nCnt]].nor);
		}
	}

	// ���_���A�����b�N����
	m_pVtxBuff->Unlock();
}