//=========================================
//
//	debug.cpp
//	Author : �y���m��
//
//=========================================
#include "debug.h"
#include "application.h"
#include <stdio.h>
#include <stdarg.h>

LPD3DXFONT CDebugProc::m_pFont = nullptr;
char CDebugProc::m_aStr[1024] = {};
//=========================================
//����������
//=========================================
void CDebugProc::Init()
{
#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(CApplication::GetRender()->GetDevice() , 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
#endif
}

//=========================================
//�X�V����
//=========================================
void CDebugProc::Uninit()
{
	if (m_pFont != nullptr)
	{
		m_pFont = nullptr;
	}
}

//=========================================
//�f�o�b�N�̋L������
//=========================================
void CDebugProc::print(const char *pFormat, ...)
{
	va_list List;
	va_start(List, pFormat);
	vsprintf(m_aStr,pFormat,List);
	va_end(List);
}

//=========================================
//�f�o�b�N�̕`�揈��
//=========================================
void CDebugProc::Draw()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (m_pFont != nullptr)
	{
		// �e�L�X�g�`��
		m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
		m_aStr[0] = '\0';
	}
}