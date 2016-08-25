/**
* @file     util.c
* @brief    utility routines used by different test/demo applications
* @author   Mark Zieg <mark.zieg@oceanoptics.com>
* @date     Jan 2011
*/

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "util.h"
#include "api/SeaBreezeWrapper.h"

#define GRAPH_WIDTH    80    // columns
#define GRAPH_HEIGHT   20    // rows

#ifndef MAXFLOAT
#define MAXFLOAT	   3.40282347e+38F
#endif

/**
* @brief wrapper over printf to force flush after every call
*
* This is very handy when tests die with a lot of buffered output undisplayed.
* Also go ahead and add a timestamp for timing tests.
*/
void logger(const char *fmt, ...) {
    time_t rawtime;
    static char buf[25]; // e.g. "Tue Nov 29 15:26:54 2011"
    va_list args;
    va_start(args, fmt);

    // get timestamp
    time(&rawtime);
    strncpy(buf, ctime(&rawtime), sizeof(buf));
    buf[sizeof(buf) - 1] = 0;

    // process args
    printf("%s ", buf);
    vprintf(fmt, args);
    va_end(args);
    if (fmt[strlen(fmt)-1] != '\n')
        printf("\n");
    fflush(stdout);
    fflush(stderr);
}

void logger_header(const char *fmt, ...) {
    static char line[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(line, sizeof(line), fmt, args);
    va_end(args);

    logger("\n");
    logger("================================================\n");
    logger(line);
    logger("================================================\n");
    logger("\n");
}

const char* get_error_string(int error) {
    static char buffer[32];
    seabreeze_get_error_string(error, buffer, sizeof(buffer));
    return buffer;
}

// returns non-zero if error occurred
int check_error(int index, int *error, const char *func) {
    if (*error) 
        logger("[%02d]   error calling %s: %s\n", index, func, 
            get_error_string(*error));
    fflush(stdout);
    return *error;
}

// you'd kind of think the SeaBreezeWrapper would provide this...
int get_external_trigger_mode(const char *type) {
         if (!strcmp(type, "HR4000"     )) return 0x01;
    else if (!strcmp(type, "QE65000"    )) return 0x01;
    else if (!strcmp(type, "STS"        )) return 0x01;
    else if (!strcmp(type, "USB2000"    )) return 0x03;
    else if (!strcmp(type, "USB2000PLUS")) return 0x02;
    logger("ERROR: can't find 'external' triggering mode for spectrometer type '%s'\n", type);
    return -1;
}

//! display a spectrum as a simple ASCII histogram
void graph(const char *type, const char *label, double *spectrum, int pixels) {
    double histogram[GRAPH_WIDTH];

    // initialize histogram
    for (int i = 0; i < GRAPH_WIDTH; i++)
        histogram[i] = 0;

    // aggregate wavelength sums by bin
    for (int i = 0; i < pixels; i++) {
        int col = (int) ((i * GRAPH_WIDTH) / pixels);
        histogram[col] += spectrum[i];
    }

    // normalize heights
    double min = MAXFLOAT;
    double max = -1 * MAXFLOAT;
    for (int i = 0; i < GRAPH_WIDTH; i++) {
        min = histogram[i] < min ? histogram[i] : min;
        max = histogram[i] > max ? histogram[i] : max;
    }

    // display graph
    printf("[%s] %s (%le, %le)\n", type, label, min, max);
    for(int row = GRAPH_HEIGHT; row; row--) {
        printf("  | ");
        for(int col = 0; col < GRAPH_WIDTH; col++)
            printf("%c", histogram[col] - min >= (max - min) * row / GRAPH_HEIGHT ? '*' : ' ');
        printf("\n");
    }

    // print x-axis
    printf("  |_");
    for(int col = 0; col < GRAPH_WIDTH; col++)
        printf("_");
    printf("\n\n");
    fflush(stdout);
}
