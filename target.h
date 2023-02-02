//=========================================
//
//	target.h
//	Author:冨所知生
//
//=========================================
#ifndef _TARGET_H_		//このマクロ定義がされてなかったら
#define _TARGET_H_		//２重インクルード防止のマクロ定義

#include "object2D.h"

class CTarget : public CObject2D
{
public:
	CTarget();			//コンストラクタ
	virtual ~CTarget() override;			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	// 更新処理
	void Update(void) override;
	// 描画処理
	void Draw(void) override;
	// 終了処理
	void Uninit(void) override;
	// クリエイト処理
	static CTarget* Create(const D3DXVECTOR3 &pos, CObject *object);
	// 目標の座標の取得
	static D3DXVECTOR3 GetTargetPos(void) { return m_TargetPos; }
	// ターゲットを設定したいオブジェクト情報を保存
	void SetObject(CObject *object) { pObject = object; }

	// 番号を入れる処理
	void SetNumber(int number) { m_Number = number; }
	// オブジェクトの取得
	CObject *GetTargetObject() { return pObject; }
	// サイズの取得
	float GetSize() { return m_Size; }

private:
	float  m_Size;						// サイズ
	static D3DXVECTOR3 m_TargetPos;		// 目標の座標
	CObject *pObject;					// 目標のオブジェクト情報の保存先
};

#endif