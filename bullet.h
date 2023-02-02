//=========================================
//
//	bullet3D.h
//	Author:冨所知生
//
//=========================================
#ifndef _BULLET_H_		//このマクロ定義がされてなかったら
#define _BULLET_H_		//２重インクルード防止のマクロ定義

#include "billboard.h"
#include "camera.h"

class CCamera;
class CBullet :public CBillboard
{
public:
	CBullet();								// コンストラクタ
	virtual ~CBullet() override;			// デストラクタ

	// 初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 終了処理
	void Uninit(void) override;
	// ロックオン処理
	D3DXVECTOR3 LockOn(D3DXMATRIX *mtxWorld);
	// クリエイト処理
	static CBullet* Create(const D3DXVECTOR3 &pos, const D3DXQUATERNION &quaternion, CObject *object);
	// オブジェクトの設定
	void SetTargetObj(CObject *obj) { m_pTargetObj = obj; }

private:
	D3DXVECTOR3						m_pos;				// 座標
	D3DXVECTOR3						m_posOld;			// 一つ前の座標
	D3DXVECTOR3						m_move;				// 移動量
	D3DXVECTOR3						m_TargetPos;		// 目標の値
	D3DXQUATERNION					m_quaternion;		// クォータニオン
	CObject*						m_pTargetObj;		// ターゲットのオブジェクト
	CCamera::CAMERA_TYPE			Camera_Type;		// カメラのタイプ
};

#endif