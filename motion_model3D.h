//=============================================================================
//
// 3Dモーションキャラクタークラス(motion_model3D.h)
// Author : 唐�ｱ結斗
// 概要 : 3Dプレイヤー生成を行う
//
//=============================================================================
#ifndef _MOTION_MODEL3D_H_			// このマクロ定義がされてなかったら
#define _MOTION_MODEL3D_H_			// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMotion;

//=============================================================================
// 3Dプレイヤークラス
// Author : 唐�ｱ結斗
// 概要 : 3Dプレイヤー生成を行うクラス
//=============================================================================
class CMotionModel3D : public CObject
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CMotionModel3D *Create();			// 3Dプレイヤーの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMotionModel3D();
	~CMotionModel3D();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画

	void SetPosOld(const D3DXVECTOR3 &posOld) { m_posOld = posOld; }				// 過去位置のセッター


	D3DXVECTOR3 GetPosOld() { return m_posOld; }									// 過去位置のゲッター

	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }				// ワールドマトリックスのセッター
	D3DXMATRIX *GetMtxWorld() { return &m_mtxWorld; }								// ワールドマトリックスのゲッター
	void SetMotion(const char *pName);												// モーション情報の設定
	void SetMotion(const char *pName, const int nNumMotion);						// モーション情報の設定(オーバーロード)
	CMotion *GetMotion() { return m_pMotion; }										// モーション情報の取得

	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRotation() { return m_rot; }

protected:
	D3DXVECTOR3		m_pos;															// 位置
	D3DXQUATERNION	m_quaternion;													// クオータニオン

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CMotion			*m_pMotion;				// モーションインスタンス
	D3DXMATRIX		m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3		m_posOld;				// 過去位置
	D3DXVECTOR3		m_size;					// 大きさ
	D3DXVECTOR3		m_rot;					// 向き
};

#endif