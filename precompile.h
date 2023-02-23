#pragma once
//=========================================
// プリコンパイルヘッダー
//=========================================
#ifndef PRECOMPILE_H_
#define PRECOMPILE_H_

// 基本
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <tchar.h>
#include <time.h>
#include <crtdbg.h>

// STL
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <iostream>
#include <fstream>

// DirectX9
#include <d3dx9.h>

// デバッグ用
#include "debug_proc.h"

// input
#define     DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <xinput.h>

#endif