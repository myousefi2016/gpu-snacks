#ifndef _TIMER_H_
#	define _TIMER_H_

#ifdef __GNUC__

#include <sys/time.h>
#include <unistd.h>

inline double currentSeconds(void)
{
	struct timeval now_time;
	gettimeofday(&now_time, 0);
	return (now_time.tv_sec + now_time.tv_usec * 1.e-6);
}

//inline double currentSeconds(void)
//{
//	return (double)clock() / CLOCKS_PER_SEC;
//}

inline unsigned currentMS(void)
{
     struct timeval now_time;
     gettimeofday(&now_time, 0);
     return (now_time.tv_sec * 1000 + now_time.tv_usec / 1000);
}

#endif /* __GNUC__ */

#ifdef _MSC_VER

#include <windows.h>

double currentSeconds(void)
{
     return (double)GetTickCount() / 1000;
}

unsigned currentMS(void)
{
     return GetTickCount();
}

#endif  /* _MSC_VER */

#endif  /* _TIMER_H_ */

