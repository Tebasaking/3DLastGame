//=========================================
//
//	main.h
//	Author:�y���m��
//
//=========================================
#ifndef _MAIN_H_		//���̃}�N����`������ĂȂ�������
#define _MAIN_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include <tchar.h> // _T
#include <d3dx9.h>

//�O���錾
class CRender;
class CObject;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���_�f�[�^
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
};

struct VERTEX_3D
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR	col;	//���_�J���[
	D3DXVECTOR2 tex;	//texture���W
};

// �X�N���[���̕�
const int SCREEN_WIDTH = 1280;
// �X�N���[���̍���
const int SCREEN_HEIGHT = 720;

// ���_�t�H�[�}�b�g
const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		//���W�E�@���E�J���[

#endif