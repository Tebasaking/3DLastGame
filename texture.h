//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>

//==================================================
// 定義
//==================================================
class CTexture
{
public: /* 定義 */
	enum TEXTURE
	{
		TEXTURE_FIRE = 0,	// 仮画像1
		TEXTURE_NUMBER,		// 仮画像2
		TEXTURE_EXPLOSION,	// 爆発画像
		TEXTURE_SKY,		// 空
		TEXTURE_SEA,		// 海
		TEXTURE_GROUND,		// 陸
		TEXTURE_THUNDER,	// 戦闘機のテクスチャ
		TEXTURE_TARGET,		// ターゲット
		TEXTURE_TITLE,		// タイトル
		TEXTURE_RESULT,		// リザルト
		TEXTURE_RADAR_MAP,	// レーダーマップ
		TEXTURE_FRAME,		// レーダーのフレーム
		TEXTURE_ENEMY_TANK,	// タンク
		TEXTURE_ENEMY_FLY,	// 戦闘機
		TEXTURE_MAX,
		TEXTURE_NONE,	// 使用しない
	};

	static const char* s_FileName[];	// ファイルパス

public:
	CTexture();		// デフォルトコンストラクタ
	~CTexture();	// デストラクタ

public: /* メンバ関数 */
	void LoadAll();										// 全ての読み込み
	void Load(TEXTURE inTexture);						// 指定の読み込み
	void ReleaseAll();									// 全ての破棄
	void Release(TEXTURE inTexture);					// 指定の破棄
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// 情報の取得

private: /* メンバ変数 */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// テクスチャの情報
};

#endif // !_TEXTURE_H_
