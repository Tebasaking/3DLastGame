////==================================================
//// ball.cpp
//// Author: Buriya Kota
////==================================================
//
////**************************************************
//// include
////**************************************************
//#include "application.h"
//#include "inputkeyboard.h"
//#include "model.h"
//#include "ball.h"
//
////**************************************************
//// �}�N����`
////**************************************************
//#define SPEED	(15.0f)
//
//D3DXVECTOR3 RotNormalization(D3DXVECTOR3 rot);
////--------------------------------------------------
//// �f�X�g���N�^
////--------------------------------------------------
//CBall::~CBall()
//{
//}
//
////--------------------------------------------------
//// ������
////--------------------------------------------------
//HRESULT CBall::Init(const D3DXVECTOR3 &pos)
//{
//	CObject::Init(pos);
//
//	// ���f���̓ǂݍ���
//	pModel->SetModel("data/MODEL/ball.x");
//
//	return S_OK;
//}
//
////--------------------------------------------------
//// �X�V
////--------------------------------------------------
//void CBall::Update()
//{
//	Control_();
//
//	D3DXVECTOR3 pos = GetPosition();
//	D3DXVECTOR3 rot = GetRot();
//
//#ifdef _DEBUG
//	// �f�o�b�O�\��
//	CDebugProc::Print("�{�[���̌��݂̈ʒu : \n x : %f , y : %f , z : %f \n", pos.x, pos.y, pos.z);
//	CDebugProc::Print("�{�[���̌��݂̊p�x : \n x : %f , y : %f , z : %f \n", rot.x, rot.y, rot.z);
//#endif // DEBUG
//}
//
////--------------------------------------------------
//// ����
////--------------------------------------------------
//CBall *CBall::Create(const D3DXVECTOR3 &pos)
//{
//	CBall *pBall;
//	pBall = new CBall;
//
//	if (pBall != nullptr)
//	{
//		pBall->Init(D3DXVECTOR3(0.0f,0.0f,0.0f));
//		pBall->SetPosition(pos);
//	}
//
//	return pBall;
//}
//
////--------------------------------------------------
//// ����
////--------------------------------------------------
//void CBall::Control_()
//{
////	// �C���v�b�g
////	CInputKeyboard *pInputKeyoard = CApplication::GetInputKeyboard();
////
////	D3DXVECTOR3 vec(0.0f, 0.0f, 0.0f);
////
////	D3DXVECTOR3 rot = GetRot();
////
////	if ((vec.x == 0.0f) && (vec.z == 0.0f))
////	{
////		if (pInputKeyoard->GetPress(DIK_LEFT))
////		{// ��
////			vec.x += -1.0f;
////		}
////		if (pInputKeyoard->GetPress(DIK_RIGHT))
////		{// �E
////			vec.x += 1.0f;
////		}
////		if (pInputKeyoard->GetPress(DIK_UP))
////		{// ��
////			vec.z += 1.0f;
////		}
////		if (pInputKeyoard->GetPress(DIK_DOWN))
////		{// ��
////			vec.z += -1.0f;
////		}
////	}
////
////	D3DXVECTOR3 pos = GetPosition();
////	D3DXVECTOR3 maxVtx = GetVtxMax();
////	D3DXVECTOR3 rotate;
////
////	if (vec.x != 0.0f || vec.z != 0.0f)
////	{// �ړ��Ƃ���ɍ��킹����]
////	 // �x�N�g���̐��K��
////		D3DXVec3Normalize(&vec, &vec);
////		// �L�[�{�[�h
////		pos += vec * SPEED;
////
////		// �ړ��ʂɑ΂��Ẳ�]�������߂�iz���j
////		rotate.z = vec.x * SPEED / maxVtx.x;
////		rot.z += -rotate.z;
////
////		// �ړ��ʂɑ΂��Ẳ�]�������߂�ix���j
////		rotate.x = vec.z * SPEED / maxVtx.z;
////		rot.x += rotate.x;
////
////		rot.x = pModel->NormalizRot(rot.x);
////		rot.y = pModel->NormalizRot(rot.y);
////		rot.z = pModel->NormalizRot(rot.z);
////
////		pModel->SetPosition(pos);
////		pModel->SetRot(rot);
////	}
//}