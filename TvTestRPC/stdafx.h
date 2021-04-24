// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// または、参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//
#pragma once

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// Windows ヘッダー ファイル
#include <windows.h>
#include <dshow.h>
#include <cstdint>
#include <tchar.h>
#define TVTEST_PLUGIN_CLASS_IMPLEMENT	// プラグインをクラスとして実装
#include "TVTestPlugin.h"
#include "strconv.h"

// プログラムに必要な追加ヘッダーをここで参照してください
