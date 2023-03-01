//===================================================
//
// サウンドヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _SOUND_H_
#define _SOUND_H_	

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include <xaudio2.h>

//================================
// サウンドクラスの定義
//================================
class CSound
{
public:
	//*****************************************************************************
	// パラメータ構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	//*****************************************************************************
	// サウンドファイル
	//*****************************************************************************
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// ゲーム中のBGM
		SOUND_LABEL_BGM_TITLE,		// タイトルのBGM
		SOUND_LABEL_BGM_RANKING,	// ランキングのBGM
		SOUND_LABEL_BGM_WIN,		// 勝利時のBGM
		SOUND_LABEL_BGM_LOSE,		// 敗北時のBGM
		SOUND_SE_MARSHALL_ATTACK,	// 近接攻撃
		SOUND_SE_EXPLOSION,			// 爆発
		SOUND_SE_STEP,				// 足音
		SOUND_SE_MISSILE,			// ミサイルの発射音
		SOUND_SE_TRANSFORM,			// 機体を変形させた時の音
		SOUND_SE_FLIGHT,			// 飛行中の音
		SOUND_SE_DROP,				// 着地の音
		SOUND_SE_JUMP,				// ジャンプの音
		SOUND_SE_LOCK_ON,			// ロックオンの音
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();	//コンストラクタ
	~CSound();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(HWND hWnd);
	void Uninit(void);

	static HRESULT PlaySound(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	static void StopSound(void);

	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	//------------------
	// 静的メンバ変数
	//------------------
	static IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// オーディオデータサイズ
};

#endif // !_SOUND_H_