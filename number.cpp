//========================================================
//スコア画面の処理
//========================================================
#include "number.h"
#include "input.h"
#include "score.h"

#define MAX_NUMBER (8)			//スコアに使う数字

//======================================================
//コンストラクタ
//======================================================
CNumber::CNumber(int nPriority)
{
	//SetType(OBJTYPE_MODE);
}

//=========================================
//デストラクタ
//=========================================
CNumber::~CNumber()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CNumber::Init(const D3DXVECTOR3 &pos)
{
	CObject2D::Init(pos);

	//サイズの設定
	CObject2D::SetScale(D3DXVECTOR3(50.0f,50.0f,0.0f));

	// テクスチャの設定
	CObject2D::SetTexture(CTexture::TEXTURE_NUMBER);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CNumber::Update()
{
	//D3DXVECTOR3 pos;
	//pos = GetPosition();
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CNumber::Uninit()
{
	//解放処理
	CObject2D::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CNumber::Draw()
{
	CObject2D::Draw();
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CNumber* CNumber::Create(const D3DXVECTOR3 &pos)
{
	int nNumCreate = m_nNumAll;

	CNumber* pCNumber = nullptr;

	pCNumber = new CNumber();

	if (pCNumber != nullptr)
	{
		pCNumber->Init(pos);
	}

	return pCNumber;
}

//========================================================
//スコアの設定処理
//========================================================
void CNumber::Set(int nDigits)
{
	SetAnim(nDigits,12);
}
