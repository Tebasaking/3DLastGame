//=========================================
//
//	missile_alert_manager.h
//	Author:冨所知生
//
//=========================================
#ifndef _MISSILE_ALERT_MANAGER_H_		//このマクロ定義がされてなかったら
#define _MISSILE_ALERT_MANAGER_H_		//２重インクルード防止のマクロ定義

#include "object.h"

class CMissileAlertManager : public CObject
{
public:
	CMissileAlertManager();			//コンストラクタ
	virtual ~CMissileAlertManager();			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override { return S_OK; }
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override {}
	// 終了処理
	void Uninit(void) override { m_bAlert = false;  Release(); }

	void SetObject(CObject *Obj) { pObject = Obj; }
	void SetAlert(bool bAlt) { m_bAlert = bAlt; }
	bool GetAlert() { return m_bAlert; }

	static CMissileAlertManager *Create(CObject *Obj);

private:
	CObject				*pObject;			// 目標のオブジェクト情報の保存先
	bool				m_bAlert;
};

#endif