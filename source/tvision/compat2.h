#ifndef _COMPAT2_H_
#define _COMPAT2_H_

#include <windows.h>
#include <stdint.h>
#include <time.h>

/* get system time */
static inline void itimeofday_default(long *sec, long *usec)
{
	#if defined(__unix)
	struct timeval time;
	gettimeofday(&time, NULL);
	if (sec) *sec = (long)(time.tv_sec);
	if (usec) *usec = (long)(time.tv_usec);
	#elif defined(_WIN32)
	typedef void (WINAPI * GetSystemTime_t)(LPFILETIME);
	static GetSystemTime_t GetSystemTime_p = NULL;
	static int64_t epoch = 0;
	static volatile int inited = 0;
	LARGE_INTEGER ularge;
	FILETIME file_time;
	int64_t current;
	if (inited == 0) {
		ularge.HighPart = 0x019db1de;
		ularge.LowPart = 0xd53e8000;
		epoch = (int64_t)ularge.QuadPart;
		GetSystemTime_p = (GetSystemTime_t)GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")),
				"GetSystemTimePreciseAsFileTime"
				);
		inited = 1;	
	}
	if (GetSystemTime_p == NULL) {
		GetSystemTimeAsFileTime(&file_time);
	}
	else {
		GetSystemTime_p(&file_time);
	}
	ularge.LowPart = file_time.dwLowDateTime;
	ularge.HighPart = file_time.dwHighDateTime;
	current = ((int64_t)ularge.QuadPart) - epoch;
	if (sec) *sec = (long)(current / 10000000);
	if (usec) *usec = (long)((current % 10000000) / 10);
	#endif
}

static inline int64_t iclock64(void)
{
	long s, u;
	int64_t value;
	itimeofday_default(&s, &u);
	value = ((int64_t)s) * 1000 + (u / 1000);
	return value;
}

#endif


