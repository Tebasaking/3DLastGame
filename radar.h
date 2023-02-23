//=========================================
//
//	radar.h
//	Author:冨所知生
//
//=========================================
#ifndef _RADAR_H_		//このマクロ定義がされてなかったら
#define _RADAR_H_		//２重インクルード防止のマクロ定義

#include "billboard.h"
#include "camera_player.h"

class CObject2D;
class CCamera;
class CRadar :public CBillboard
{
public:
	enum RADAR_TYPE
	{
		RADAR_MAP,
		RADAR_PLAYER,
		RADAR_ENEMY,
		RADAR_FRAME
	};

	explicit CRadar(int nPriority = LAYER_FIVE);			// コンストラクタ
	virtual ~CRadar() override;								// デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//ロックオン処理
	D3DXVECTOR3 LockOn(D3DXMATRIX *mtxWorld);
	//クリエイト処理
	static CRadar* Create(const D3DXVECTOR3 &pos, CObject *object, RADAR_TYPE type);

	// 追尾するオブジェクトの設定
	void SetObject(CObject *pObj) { m_pObject = pObj; }
	// タイプの設定
	void SetType(RADAR_TYPE type) { m_type = type; }
	// タイプのゲッター
	RADAR_TYPE GetType() { return m_type; }

private:
	D3DXVECTOR3 m_pos;				// 座標
	D3DXVECTOR3 m_posOld;			// 一つ前の座標
	D3DXVECTOR3 m_move;				// 移動量
	D3DXQUATERNION m_quaternion;	// クォータニオン
	CObject *m_pObject;
	CCameraPlayer::CAMERA_TYPE Camera_Type;
	RADAR_TYPE m_type;				// レーダーのタイプ
	CObject2D *m_pBackGround;		// レーダーの背景
};

#endif