#ifndef _TIMER_LINUX_H
#define _TIMER_LINUX_H

#include <sys/time.h>
#include <sys/select.h>

#include <stddef.h>
#include <sys/types.h>

#include "timer_interface.h"

struct Timer {
    struct timeval end_time;
};


#endif
