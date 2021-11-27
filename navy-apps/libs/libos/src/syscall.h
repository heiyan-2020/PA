#ifndef __SYSCALL_H__
#define __SYSCALL_H__
#define STRACE y

#ifdef STRACE
#define FOREACH(CALL)  \
  CALL(SYS_exit)  \
  CALL(SYS_yield)  \
  CALL(SYS_open)  \
  CALL(SYS_read)  \
  CALL(SYS_write)  \
  CALL(SYS_kill)  \
  CALL(SYS_getpid)  \
  CALL(SYS_close)  \
  CALL(SYS_lseek)  \
  CALL(SYS_brk)  \
  CALL(SYS_fstat)  \
  CALL(SYS_time)  \
  CALL(SYS_signal)  \
  CALL(SYS_execve)  \
  CALL(SYS_fork)  \
  CALL(SYS_link)  \
  CALL(SYS_unlink)  \
  CALL(SYS_wait)  \
  CALL(SYS_times)  \
  CALL(SYS_gettimeofday)  \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
enum SYS_CALL{
	FOREACH(GENERATE_ENUM)
};

static const char* SYS_STRING[] = {
	FOREACH(GENERATE_STRING)
};
#endif
#endif
