#ifndef _TIMER_H_
#	define _TIMER_H_

#ifndef _WIN32

#include <sys/time.h>
#include <unistd.h>

#if 1
inline double currentSeconds(void)
{
     struct timeval now_time;
     gettimeofday(&now_time, 0);
     return now_time.tv_sec + now_time.tv_usec*1.e-6;
}
#else
inline double currentSeconds(void)
{
     return (double)clock() / CLOCKS_PER_SEC;
}
#endif  /* 1 */

inline unsigned currentMS(void)
{
     struct timeval now_time;
     gettimeofday(&now_time, 0);
     return now_time.tv_sec*1000 + now_time.tv_usec/1000;
}

#else /* ifdefine _WIN32 */

#include <windows.h>

double currentSeconds(void)
{
     return (double)GetTickCount() / 1000;
}

unsigned currentMS(void)
{
     return GetTickCount();
}

#endif  /* ifndef _WIN32 */

#endif  /* _TIMER_H_ */

