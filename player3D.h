//=========================================
//
//	player3D.h
//	Author:冨所知生
//
//=========================================
#ifndef _PLAYER_H_		//このマクロ定義がされてなかったら
#define _PLAYER_H_		//２重インクルード防止のマクロ定義
#define MAX_PARTS (1)

#include "main.h"
#include "object3D.h"
#include "model.h"
#include "motion_model3D.h"

class CModel3D;
class CPlayer3D : public CMotionModel3D
{
public:
	enum PLAYER_MODE
	{
		MODE_FLY = 0,
		MODE_ROBOT,
		MODE_MAX
	};
	
	CPlayer3D();			//コンストラクタ
	virtual ~CPlayer3D() override;			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 終了処理
	void Uninit(void) override;
	// クリエイト処理
	static CPlayer3D* Create(const D3DXVECTOR3 &pos);
	void SetMode(PLAYER_MODE mode) { m_mode = mode; }
	
	// ロックオン処理
	void LockOn(void);
	// 回転処理
	void Rotate(void);
	// 弾の発射処理
	void Bullet(void);

	// 更新処理(飛行)
	void UpdateFly(void);
	// 更新処理(人型)
	void UpdateRob(void);

	//=========================================
	// 飛行状態でしか使わないもの
	//=========================================
	// 姿勢制御処理
	void Attitude(void);

	const D3DXQUATERNION& GetQuaternion() const { return m_quaternion; }

private:
	float m_scale;										// 大きさ
	D3DXVECTOR3 m_size;									// 拡大率

	CModel3D*				m_apModel[MAX_PARTS];		// モデルの最大数
	CModel3D*				m_pRobot;					// ロボットのモデル読み込み
	D3DXMATRIX				m_WorldMtx;					// ワールドマトリックス
	D3DXVECTOR3				m_MouseMove;				// マウスの移動処理
	PLAYER_MODE				m_mode;						// モード
};

#endif