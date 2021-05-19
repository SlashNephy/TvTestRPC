#pragma once

#include "stdafx.h"

/*
 * SYSTEMTIME �\���̂� time_t �ɕϊ�����
 */
inline time_t SystemTime2Timet(const SYSTEMTIME& st)
{
	struct tm gm = {
		st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0
	};
	
	return mktime(&gm);
}

/*
 * �S�p �� ���p�ɕϊ�����
 * http://yamatyuu.net/computer/program/zen2han/index.html
 */
inline TCHAR* Full2Half(TCHAR* s)
{
	for (auto* p = s; *p; p++)
    {
        // �S�p�����p���L���̏ꍇ
        if (0xff01 <= *p  && *p <= 0xff5d) {
                *p -= 0xff00 - 0x20;
        } else if (*p == L'�@') {
                *p = L' ';
        }
    }

	return s;
}
