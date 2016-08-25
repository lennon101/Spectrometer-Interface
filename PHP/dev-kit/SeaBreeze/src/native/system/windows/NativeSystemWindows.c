/***************************************************//**
 * @file    NativeSystemWindows.cpp
 * @date    February 2012
 * @author  Ocean Optics, Inc.
 *
 * Copyright (C) 2012 Ocean Optics, Inc.
 * All rights reserved.
 *
 * Notes:
 *
 * The NativeSystemPOSIX class provides an implementation
 * of non-portable system calls for the Windows API.
 *
 *******************************************************/

#include "common/globals.h"
#include <windows.h>               /* For definition of Sleep() */
#include "native/system/NativeSystem.h"

/* Function definitions */

void sleepMilliseconds(unsigned int msecs) {
    /* Simply delegate to the Windows API Sleep() function.  This delays for
     * the given number of milliseconds.
     */
    Sleep(msecs);
}
