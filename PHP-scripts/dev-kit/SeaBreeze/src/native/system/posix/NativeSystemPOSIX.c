/***************************************************//**
 * @file    NativeSystemPOSIX.cpp
 * @date    February 2012
 * @author  Ocean Optics, Inc.
 *
 * Copyright (C) 2012 Ocean Optics, Inc.
 * All rights reserved.
 *
 * Notes:
 *
 * The NativeSystemPOSIX class provides an implementation
 * of non-portable system calls for POSIX.  This should
 * work for at least Linux, OSX, and any other UNIX-like
 * operating system.
 *
 *******************************************************/

/* Definitions */
#define _POSIX_C_SOURCE 199309  /* Needed for Linux to define POSIX level */

#include "common/globals.h"
#include <time.h>               /* For definition of nanosleep() */
#include "native/system/NativeSystem.h"

/* Function definitions */

void sleepMilliseconds(unsigned int msecs) {
    struct timespec ts;

    /* The preferred delay function in modern POSIX is nanosleep().  This
     * replaces the old sleep() (1-second resolution) and usleep() (microsecond
     * resolution) with something that can report time left in case the
     * delay was interrupted.
     *
     * If p-threads or some other mechanism is ever added to SeaBreeze then
     * this may need to be replaced with something more thread-aware.
     */
    ts.tv_sec = msecs/1000;                 /* Whole seconds portion */
    ts.tv_nsec = (msecs % 1000) * 1000000;  /* Fraction in nanoseconds */

    /* Sleep for the given period.  Note that this discards any information
     * about whether the sleep was interrupted; this is simply best-effort.
     */
    nanosleep(&ts, NULL);
}
