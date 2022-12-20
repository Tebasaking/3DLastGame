//=========================================
//
//	debug.h
//	Author:�y���m��
//
//=========================================
#ifndef _DEBUG_H_		//���̃}�N����`������ĂȂ�������
#define _DEBUG_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"
//=========================================
// �N���X
//=========================================
class CDebugProc
{
public:
	CDebugProc(){ }
	~CDebugProc() {}

	void Init(void);
	void Uninit(void);
	static void print(const char *pFormat,...);
	static void Draw(void);

private:
	static LPD3DXFONT m_pFont;
	static char m_aStr[1024];
};

#endif	// _DEBUG_H_