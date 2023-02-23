//=========================================
//
//	player_ui.h
//	Author:冨所知生
//
//=========================================
#ifndef _PLAYER_UI_H_		//このマクロ定義がされてなかったら
#define _PLAYER_UI_H_		//２重インクルード防止のマクロ定義

#include "Object2D.h"

#define SPEED_DIGITS (3)
#define ALTITUDE_DIGITS (4)
#define MISSILE_DIGITS (2)
//=========================================
// クラス
//=========================================
class CNumber;
class CObject2D;
class CPlayerUI : public CObject
{
	enum COLOR
	{
		RED = 0,
		GREEN,
	};
public:
	explicit CPlayerUI(int nPriority = LAYER_ONE);
	~CPlayerUI();

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override;	// 初期化処理
	virtual void Uninit() override;							// 終了処理
	virtual void Update() override;							// 更新処理
	virtual void Draw() override {};						// 描画処理
	void Set();												// 桁数の設定
	void AlertCheck();
	void SetAlertColor(COLOR col);
	void SetAlert(bool Alert) { m_bAlert = Alert; }
	bool GetAlert() { return m_bAlert; }

private:
	CObject2D	*pObject2D[2];
	CNumber		*m_pSpeedNum[SPEED_DIGITS];					// ナンバー1
	CNumber		*m_pAltitude[ALTITUDE_DIGITS];				// ナンバー1
	CNumber		*m_pMissileNum[MISSILE_DIGITS];				// ミサイルナンバー
	
	CObject2D	*m_pGageBox;								// ゲージBOX
	CObject2D	*m_pGage;									// ゲージ

	COLOR		*m_ColorType;
	bool		m_bAlert;									// アラートが使用されているかいないか
};

#endif	// _TITLE_H_