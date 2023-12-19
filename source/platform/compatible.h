#ifndef _COMPATIBLE_H_
#define _COMPATIBLE_H_

#include <windows.h>
#include <stdio.h>

static BOOL GetCurrentConsoleFont2(HANDLE hConsoleOutput,
		BOOL bMaximumWindow, PCONSOLE_FONT_INFO lpConsoleCurrentFont)
{
	static int inited = 0;
	typedef BOOL (WINAPI *GCCF_t)(HANDLE, BOOL, PCONSOLE_FONT_INFO);
	static GCCF_t GCCF = NULL;
	const char *name = "GetCurrentConsoleFont";
	if (inited == 0) {
		HINSTANCE hDLL = LoadLibraryA("kernel32.dll");
		if (hDLL) {
			GCCF = (GCCF_t)GetProcAddress(hDLL, name);
		}
		inited = 1;
	}
	if (GCCF != NULL) {
		return GCCF(hConsoleOutput, bMaximumWindow, lpConsoleCurrentFont);
	}
	return FALSE;
}

#if 1
typedef struct _CONSOLE_FONT_INFOEX2 {
	ULONG cbSize;
	DWORD nFont;
	COORD dwFontSize;
	UINT  FontFamily;
	UINT  FontWeight;
	WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX2, *PCONSOLE_FONT_INFOEX2;
#else
typedef CONSOLE_FONT_INFOEX CONSOLE_FONT_INFOEX2;
typedef PCONSOLE_FONT_INFOEX PCONSOLE_FONT_INFOEX2;
#endif

static BOOL GetCurrentConsoleFontEx2(HANDLE hConsoleOutput,
		BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX2 lpConsoleCurrentFontEx) {
	static int inited = 0;
	typedef BOOL (WINAPI *GCCFE_t)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX2);
	static GCCFE_t GCCFE = NULL;
	const char *name = "GetCurrentConsoleFontEx";
	if (inited == 0) {
		HINSTANCE hDLL = LoadLibraryA("kernel32.dll");
		if (hDLL) {
			GCCFE = (GCCFE_t)GetProcAddress(hDLL, name);
		}
		inited = 1;
	}
	memset(lpConsoleCurrentFontEx, 0, sizeof(CONSOLE_FONT_INFOEX2));
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX2);
	if (GCCFE) {
		BOOL hr = GCCFE(hConsoleOutput, bMaximumWindow, lpConsoleCurrentFontEx);
		return hr;
	}
	return FALSE;
}

static BOOL SetCurrentConsoleFontEx2(HANDLE hConsoleOutput,
		BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX2 lpConsoleCurrentFontEx) {
	static int inited = 0;
	typedef BOOL (WINAPI *SCCFE_t)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX2);
	static SCCFE_t SCCFE = NULL;
	const char *name = "SetCurrentConsoleFontEx";
	if (inited == 0) {
		HINSTANCE hDLL = LoadLibraryA("kernel32.dll");
		if (hDLL) {
			SCCFE = (SCCFE_t)GetProcAddress(hDLL, name);
		}
		inited = 1;
	}
	if (SCCFE) {
		BOOL hr = SCCFE(hConsoleOutput, bMaximumWindow, lpConsoleCurrentFontEx);
		return hr;
	}
	return FALSE;
}

#endif


