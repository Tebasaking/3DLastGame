//=========================================
//
//	missile_alert.h
//	Author:冨所知生
//
//=========================================
#ifndef _ALERT_H_		//このマクロ定義がされてなかったら
#define _ALERT_H_		//２重インクルード防止のマクロ定義

#include "object2D.h"

class CAlert : public CObject2D
{
public:
	CAlert();			//コンストラクタ
	virtual ~CAlert() override;			//デストラクタ

											//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 終了処理
	void Uninit(void) override;
	// クリエイト処理
	static CAlert* Create(const D3DXVECTOR3 &pos, CObject *object);
	// ターゲットを設定したいオブジェクト情報を保存
	void SetObject(CObject *object) { pObject = object; }

	// オブジェクトの取得
	CObject *GetTargetObject() { return pObject; }
	// サイズの取得
	float GetSize() { return m_Size; }

private:
	float  m_Size;							// サイズ
	static D3DXVECTOR3	m_TargetPos;		// 目標の座標
	D3DXVECTOR3			m_pos;				// 現在の座標
	CObject *pObject;						// 目標のオブジェクト情報の保存先
};

#endif