//=========================================
//
// mode.h
// Author: 冨所知生
//
//=========================================
#ifndef _MODE_H_
#define _MODE_H_

//=========================================
// インクルード
//=========================================
#include "object.h"

//=========================================
// クラス
//=========================================
class CMode : public CObject
{
public:
	CMode();
	~CMode() override;

	virtual HRESULT Init(const D3DXVECTOR3 &pos) override { return S_OK; }	//初期化処理
	virtual void Uninit() override { Release(); }					//終了処理
	virtual void Update() override {  }					//更新処理
	virtual void Draw() override {};					//描画処理

private:
	D3DXVECTOR3 m_pos;						//座標
	float m_scale;							// 拡大率
	D3DXVECTOR3 m_size;						// サイズ
	D3DXVECTOR3 m_rot;
};

#endif