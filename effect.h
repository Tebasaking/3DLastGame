//=========================================
//
//	effect.h
//	Author:冨所知生
//
//=========================================
#ifndef _EFFECT_H_		//このマクロ定義がされてなかったら
#define _EFFECT_H_		//２重インクルード防止のマクロ定義

#include "billboard.h"

class CTexture;
class CEffect :public CBillboard
{
public:
	enum RENDER_MODE
	{
		MODE_NORMAL = 0,		// 通常
		MODE_ADD,				// 加算合成
		MODE_SUB,				// 減算合成
		MAX_MODE,				// 計算方法の最大数
	};

	CEffect();			//コンストラクタ
	virtual ~CEffect() override;			//デストラクタ

	//初期化処理
	HRESULT Init(const D3DXVECTOR3 &pos) override;
	//更新処理
	void Update(void) override;
	//描画処理
	void Draw(void) override;
	//終了処理
	void Uninit(void) override;
	//クリエイト処理
	static CEffect* Create(const D3DXVECTOR3 &pos,int ANIM_SPEED, int ANIM_MAX, CTexture::TEXTURE tex);
	void SetMoveVec(const D3DXVECTOR3 moveVec) { m_moveVec = moveVec; }					// 移動方向の設定
	D3DXVECTOR3 GetMoveVec() { return m_moveVec; }										// 移動方向の取得
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }							// 速度の設定
	void SetRenderMode(const RENDER_MODE renderMode) { m_renderMode = renderMode; }		// レンダーステートの計算方法の設定
	void SetLife(const int nLife) { m_nLife = nLife; }
	void SetTex(CTexture::TEXTURE tex) { m_tex = tex; }
	void DecreaseLife(void);

private:
	RENDER_MODE		m_renderMode;		// レンダーステートの計算方法
	CTexture::TEXTURE m_tex;			// テクスチャ情報
	D3DXVECTOR3		m_move;				// 移動量
	D3DXVECTOR3		m_moveVec;			// 移動方向
	D3DXVECTOR3		m_subSize;			// 大きさの減少量
	D3DXVECTOR3		m_pos;				// 座標
	D3DXCOLOR		m_col;				// 色
	D3DXQUATERNION	m_quaternion;		// クォータニオン

	int		m_size;						// サイズ
	int		m_ANIM_SPEED;
	int		m_ANIM_MAX;
	int		nAnimCount;
	int		nAnimNum;
	int		m_nLife;					// 寿命
	float	m_RandAlfa;					// α値をX(ランダム)減らす
	float	m_fSpeed;					// 速度
	float	m_fSubAlpha;				// アルファ値の減少量
};

#endif