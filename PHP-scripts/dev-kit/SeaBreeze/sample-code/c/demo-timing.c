/**************************************************************************//**
 * @file    demo-timing.c
 * @date    21-Nov-2012
 * @author  Ocean Optics, Inc.
 * @brief   Demonstrate how to time SeaBreeze calls.
 *
 * Copyright (C) 2012 Ocean Optics, Inc.
 * All rights reserved.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "api/SeaBreezeWrapper.h"
#include "util.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_LABEL_SIZE     15

int main(int argc, char **argv) 
{
    int error = 0;
    logger_header("Output from %s", argv[0]);

    ////////////////////////////////////////////////////////////////////////////
    // open spectrometer
    ////////////////////////////////////////////////////////////////////////////

    int specIndex = 0;
    if(seabreeze_open_spectrometer(specIndex, &error))
    {
        logger("no devices found.");
        exit(1);
    }

    ////////////////////////////////////////////////////////////////////////////
    // describe the unit we're testing
    ////////////////////////////////////////////////////////////////////////////

    char type[MAX_LABEL_SIZE + 1];
    seabreeze_get_spectrometer_type(specIndex, &error, type, sizeof(type));
    if (check_error(specIndex, &error, "seabreeze_get_spectrometer_type")) 
        exit(1);

    int pixels = seabreeze_get_formatted_spectrum_length(specIndex, &error);
    if (check_error(specIndex, &error, "seabreeze_get_formatted_spectrum_length"))
        exit(1);

    logger("Testing index 0 (%s with %d pixels)", type, pixels);

    ////////////////////////////////////////////////////////////////////////////
    // get test parameters
    ////////////////////////////////////////////////////////////////////////////

    unsigned integ_time_millis = 0;
    printf("\nEnter integration time (millisec): ");
    scanf("%u", &integ_time_millis);

    unsigned scans_to_average = 0;
    printf("Enter scans to average: ");
    scanf("%u", &scans_to_average);

    unsigned iterations = 0;
    printf("Enter number of iterations: ");
    scanf("%u", &iterations);

    printf("\n");

    ////////////////////////////////////////////////////////////////////////////
    // configure test
    ////////////////////////////////////////////////////////////////////////////

    seabreeze_set_integration_time(specIndex, &error, integ_time_millis * 1000);
    check_error(specIndex, &error, "seabreeze_set_integration_time");

    double *spectrum = (double*) malloc(pixels * sizeof(double));
    double *average  = (double*) malloc(pixels * sizeof(double));

    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);

    ////////////////////////////////////////////////////////////////////////////
    // run test
    ////////////////////////////////////////////////////////////////////////////

    logger("Running test");
    for (unsigned i = 0; i < iterations; i++) 
    {
        logger("  Iteration %u of %u", i + 1, iterations);
        memset(average, 0, sizeof(average));
        for (int j = 0; j < scans_to_average; j++) {
            logger("    Scan %u of %u", j + 1, scans_to_average);
            memset(spectrum, 0, sizeof(spectrum));
            seabreeze_get_formatted_spectrum(specIndex, &error, spectrum, pixels);
            for (unsigned k = 0; k < pixels; k++)
                average[k] += spectrum[k];
        }
        for (unsigned k = 0; k < pixels; k++)
            average[k] /= scans_to_average;
    }
    logger("Test complete");

    ////////////////////////////////////////////////////////////////////////////
    // report elapsed time
    ////////////////////////////////////////////////////////////////////////////

    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    unsigned sec = tv_end.tv_sec - tv_start.tv_sec;
    long micros = tv_end.tv_usec - tv_start.tv_usec;
    if (micros < 0)
    {
        micros += 1000000L;
        sec++;
    }

    double elapsed_sec = sec + (micros / 1000000.0);
    double expected_sec = iterations * integ_time_millis * scans_to_average / 1000.0; 

    logger("Test took %lf sec (%lf expected)", elapsed_sec, expected_sec);

    ////////////////////////////////////////////////////////////////////////////
    // cleanup
    ////////////////////////////////////////////////////////////////////////////

    free(average);
    free(spectrum);
    seabreeze_close_spectrometer(specIndex, &error);
}
