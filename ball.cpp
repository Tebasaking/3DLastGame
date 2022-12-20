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
//// マクロ定義
////**************************************************
//#define SPEED	(15.0f)
//
//D3DXVECTOR3 RotNormalization(D3DXVECTOR3 rot);
////--------------------------------------------------
//// デストラクタ
////--------------------------------------------------
//CBall::~CBall()
//{
//}
//
////--------------------------------------------------
//// 初期化
////--------------------------------------------------
//HRESULT CBall::Init(const D3DXVECTOR3 &pos)
//{
//	CObject::Init(pos);
//
//	// モデルの読み込み
//	pModel->SetModel("data/MODEL/ball.x");
//
//	return S_OK;
//}
//
////--------------------------------------------------
//// 更新
////--------------------------------------------------
//void CBall::Update()
//{
//	Control_();
//
//	D3DXVECTOR3 pos = GetPosition();
//	D3DXVECTOR3 rot = GetRot();
//
//#ifdef _DEBUG
//	// デバッグ表示
//	CDebugProc::Print("ボールの現在の位置 : \n x : %f , y : %f , z : %f \n", pos.x, pos.y, pos.z);
//	CDebugProc::Print("ボールの現在の角度 : \n x : %f , y : %f , z : %f \n", rot.x, rot.y, rot.z);
//#endif // DEBUG
//}
//
////--------------------------------------------------
//// 生成
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
//// 操作
////--------------------------------------------------
//void CBall::Control_()
//{
////	// インプット
////	CInputKeyboard *pInputKeyoard = CApplication::GetInputKeyboard();
////
////	D3DXVECTOR3 vec(0.0f, 0.0f, 0.0f);
////
////	D3DXVECTOR3 rot = GetRot();
////
////	if ((vec.x == 0.0f) && (vec.z == 0.0f))
////	{
////		if (pInputKeyoard->GetPress(DIK_LEFT))
////		{// 左
////			vec.x += -1.0f;
////		}
////		if (pInputKeyoard->GetPress(DIK_RIGHT))
////		{// 右
////			vec.x += 1.0f;
////		}
////		if (pInputKeyoard->GetPress(DIK_UP))
////		{// 上
////			vec.z += 1.0f;
////		}
////		if (pInputKeyoard->GetPress(DIK_DOWN))
////		{// 下
////			vec.z += -1.0f;
////		}
////	}
////
////	D3DXVECTOR3 pos = GetPosition();
////	D3DXVECTOR3 maxVtx = GetVtxMax();
////	D3DXVECTOR3 rotate;
////
////	if (vec.x != 0.0f || vec.z != 0.0f)
////	{// 移動とそれに合わせた回転
////	 // ベクトルの正規化
////		D3DXVec3Normalize(&vec, &vec);
////		// キーボード
////		pos += vec * SPEED;
////
////		// 移動量に対しての回転涼を求める（z軸）
////		rotate.z = vec.x * SPEED / maxVtx.x;
////		rot.z += -rotate.z;
////
////		// 移動量に対しての回転涼を求める（x軸）
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