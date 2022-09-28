#include "Thread.h"

pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}