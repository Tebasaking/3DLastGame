//=========================================
//
//	タイトルのオブジェクトの処理
//	Author:冨所知生
//
//=========================================
#include "enemy.h"
#include "application.h"
#include "input.h"
#include "inputkeyboard.h"
#include "texture.h"
#include "game.h"
#include "model.h"
#include "model3D.h"
#include "target.h"
#include "debug_proc.h"
#include "meshfield.h"
#include "radar.h"
#include "bullet3D.h"
#include "title_model.h"
#include "effect.h"
#include "object.h"

//=========================================
//コンストラクタ
//=========================================
CTitleModel::CTitleModel(int nPriority) : CMotionModel3D(LAYER_FIVE)
{
	SetObjectType(OBJECT_ENEMY);
	SetHP(10);
	m_type = ENEMY_MAX;
}

//=========================================
//デストラクタ
//=========================================
CTitleModel::~CTitleModel()
{
}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CTitleModel::Init(const D3DXVECTOR3 &pos)
{
	m_PlayerPos = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);

	m_apModel[0] = new CModel3D;

	m_apModel[0]->SetModelID(3);
	m_apModel[0]->Init();
	m_apModel[0]->SetPos(pos);

	m_pos = pos;

	//拡大率の設定
	m_apModel[0]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// 座標の設定
	SetPosition(pos);

	// 古い座標の設定
	SetPosOld(m_pos);

	m_scale = 1.0f;

	// モーションの初期化処理
	CMotionModel3D::Init(pos);

	if (m_type == ENEMY_GROUND)
	{
		//大きさの設定
		SetSize(D3DXVECTOR3(2.0f,2.0f,2.0f));
	}
	else if (m_type == ENEMY_FLY)
	{
		//大きさの設定
		SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CTitleModel::Update()
{
	auto FloatRandom = [](float fMax, float fMin)
	{
		return ((rand() / (float)RAND_MAX) * (fMax - fMin)) + fMin;
	};

	// 古い座標の設定
	SetPosOld(m_pos);

	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerRot;

	// グラウンドの取得
	CMesh *pGround = CGame::GetGround();

	// 座標の取得
	m_pos = GetPosition();

	// オブジェクトの取得
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		CObject *object = CObject::GetObjectTop(nCnt);

		//プレイヤーの座標を取得
		while (object)
		{
			if (object != nullptr)
			{
				EObjType ObjType = object->GetObjectType();

				if (ObjType == OBJECT_PLAYER)
				{
					m_PlayerPos = object->GetPosition();
					PlayerRot = object->GetRot();
					break;
				}
			}
			object = object->GetObjectNext();
		}
	}

	D3DXVECTOR3 rot = GetRot();

	//if (SearchEye(PlayerPos, m_pos, D3DX_PI * 0.5f, 100.0f, rot.y))
	//{
	//	m_state = ENEMY_WARNNING;
	//}

	float EnemySpeed = 5.0f;

	// 移動量用
	D3DXVECTOR3 move = move = D3DXVECTOR3(sinf(rot.y) * EnemySpeed,
		-(rot.x * EnemySpeed),
		cosf(rot.y) * EnemySpeed);

	{
		if (m_type == ENEMY_FLY)
		{
		
			rot.y += 0.01f;

			// 移動量
			move = D3DXVECTOR3(sinf(rot.y) * EnemySpeed,
				-(rot.x * EnemySpeed),
				cosf(rot.y) * EnemySpeed);

			// プレイヤーの後ろを求める
			D3DXVECTOR3 BackPos = MtxPos(D3DXVECTOR3(0.0f, 0.0f, -25.0f), m_quaternion, m_pos);

			for (int i = 0; i < 10; i++)
			{
				D3DXVECTOR3 pos = BackPos;

				pos.x += FloatRandom(5.5f, -5.5f);
				pos.y += FloatRandom(5.5f, -5.5f);
				pos.z += FloatRandom(5.5f, -5.5f);

				CEffect *pEffect = CEffect::Create(D3DXVECTOR3(pos.x + FloatRandom(1.0f, -1.0f), pos.y, pos.z + FloatRandom(1.0f, -1.0f)), 0, 0, CTexture::TEXTURE_PARTIClE);
				pEffect->SetSize(D3DXVECTOR3(8.0f, 8.0f, 0.0f));
				pEffect->SetMoveVec(D3DXVECTOR3(0.0f, FloatRandom(1.5f, -1.5f), FloatRandom(1.5f, -1.5f)));
				pEffect->SetLife(15);
				pEffect->SetRenderMode(CEffect::MODE_ADD);

				pEffect->SetColor(D3DXCOLOR(1.0f, FloatRandom(0.5f, 0.0f), 0.1f, 1.0f));

				pEffect->SetTex(CTexture::TEXTURE_PARTIClE);
				pEffect->SetQuaternion(m_quaternion);
				
				// 移動量を加算する
				m_pos += move;
			}

		}

		if (m_type == ENEMY_GROUND)
		{
			move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			rot.z = -D3DX_PI * 0.25f;
		}
	}

	// 座標の設定
	SetPosition(m_pos);

	// エネミーの回転の設定
	m_apModel[0]->SetRot(rot);

	// 回転の設定
	SetRotation(rot);

	// モーションの更新処理
	CMotionModel3D::Update();

	// カメラの視点
	//CDebugProc::Print("エネミーのスクリーン座標 : (%.3f , %.3f , %.3f )\n", m_pos.x, m_pos.y, m_pos.z);
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CTitleModel::Uninit()
{
	// モデルの解放
	m_apModel[0]->Uninit();
	delete m_apModel[0];
	m_apModel[0] = nullptr;

	// モーションの終了処理
	CMotionModel3D::Uninit();

	// オブジェクトの解放
	CObject::Release();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CTitleModel::Draw()
{
	// デバイスの取得
	PDIRECT3DDEVICE9 pDevice = CApplication::GetRender()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;
	D3DXMATRIX mtxParents;

	// マテリアルデータへのポインタ
	D3DXVECTOR3 scale(5.0f, 5.0f, 5.0f);

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_WorldMtx);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);                // 行列移動関数
	D3DXMatrixMultiply(&m_WorldMtx, &m_WorldMtx, &mtxTrans);					// 行列掛け算関数

																				// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_WorldMtx);

	// 初期化
	m_apModel[0]->Draw();

	// モーションの描画処理
	CMotionModel3D::Draw();

	SetMtxWorld(m_WorldMtx);
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CTitleModel* CTitleModel::Create(const D3DXVECTOR3 &pos, const ModelType &type)
{
	CTitleModel* pModel = nullptr;

	pModel = new CTitleModel;

	if (pModel != nullptr)
	{
		pModel->SetType(type);
		pModel->SetPosition(pos);
		pModel->Init(pos);
	}

	return pModel;
}

//=========================================
// タイプとモーションの設定
//=========================================
void CTitleModel::SetType(ModelType type)
{
	// タイプの設定
	m_type = type;

	switch (m_type)
	{
	case ENEMY_FLY:
		SetMotion("data/MOTION/fly_motion.txt");
		break;

	case ENEMY_GROUND:
		SetMotion("data/MOTION/fly_motion.txt");
		break;
	}
}