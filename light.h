#ifndef _LIGHT_H_
#define _LIGHT_H_

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitLight(void);	// ポリゴンの初期化処理
void UninitLight(void);	// ポリゴンの終了処理
void UpdateLight(void);	// ポリゴンの更新処理
float NormalizeLight(float Direction);

#endif // !_LIGHT_H_

