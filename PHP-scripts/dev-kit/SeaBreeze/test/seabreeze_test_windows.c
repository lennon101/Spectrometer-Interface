/*******************************************************
 * File:    seabreeze_test_windows.c
 * Date:    July 2009
 * Author:  Ocean Optics, Inc.
 *
 * Copyright (C) 2009 Ocean Optics, Inc.
 * All rights reserved.
 *
 * Notes:
 *
 * This is a test program to exercise some of the
 * SeaBreeze functionality.  This will probably not compile
 * under gcc (Linux).
 *
 *******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "api/SeaBreezeWrapper.h"
#include <windows.h>

void speed_test(int index);
void get_spectrum_test(int index);
void get_raw_spectrum_test(int index);
void get_wavelengths_test(int index);
void read_eeprom_test(int index);
void strobe_enable_test(int index);
void trigger_mode_test(int index);
void tec_test(int index);

const char* get_error_string(int error) {
    static char buffer[32];
    seabreeze_get_error_string(error, buffer, sizeof(buffer));
    return buffer;
}

int run_all_tests() {
    int flag;
    int error;
    char type[32];
    int device_count = 0;
    int i;

    for(i = 0; i < SEABREEZE_MAX_DEVICES; i++) {
        printf("Opening spectrometer %d.\n", i);
        flag = seabreeze_open_spectrometer(i, &error);
        printf("Result is (%d) [%s]\n", flag, get_error_string(error));
        if(0 == flag) {
            device_count++;
        } else {
            break;
        }
    }

    printf("%d devices found.\n", device_count);

    for(i = 0; i < device_count; i++) {
        printf("Getting device %d name.\n", i);
        seabreeze_get_spectrometer_type(i, &error, type, sizeof(type));
        printf("Result is (%s) [%s]\n", type, get_error_string(error));
    }

    for(i = 0; i < device_count; i++) {
        printf("Setting device %d integration time to 100ms.\n", i);
        seabreeze_set_integration_time(i, &error, 100000);
        printf("Result is [%s]\n", get_error_string(error));
    }

    for(i = 0; i < device_count; i++) {
        printf("Beginning test suite for device %d\n", i);
        tec_test(i);
        strobe_enable_test(i);
        trigger_mode_test(i);
        read_eeprom_test(i);
        get_wavelengths_test(i);
        get_spectrum_test(i);
        get_raw_spectrum_test(i);
        speed_test(i);
    }

    for(i = 0; i < device_count; i++) {
        printf("Closing spectrometer %d.\n", i);
        flag = seabreeze_close_spectrometer(i, &error);
        printf("Result is (%d) [%s]\n", flag, get_error_string(error));
    }

    return 0;
}

void tec_test(int index) {

    int error;
    double temp;

    printf("Setting TEC enable to false\n");
    seabreeze_set_tec_enable(index, &error, 0);
    printf("Result is [%s]\n", get_error_string(error));

    if(0 != error) {
        printf("Bailing out of lengthy TEC test since not supported.\n");
        return;
    }

    Sleep(1000);

    printf("Setting TEC temperature to -5C\n");
    seabreeze_set_tec_temperature(index, &error, -5);
    printf("Result is [%s]\n", get_error_string(error));

    printf("Setting TEC enable to true\n");
    seabreeze_set_tec_enable(index, &error, 1);
    printf("Result is [%s]\n", get_error_string(error));

    printf("Getting TEC temperature\n");
    temp = seabreeze_read_tec_temperature(index, &error);
    printf("Result is %1.2f C [%s]\n", temp, get_error_string(error));

    Sleep(1000);
    printf("Getting TEC temperature\n");
    temp = seabreeze_read_tec_temperature(index, &error);
    printf("Result is %1.2f C [%s]\n", temp, get_error_string(error));

}

void strobe_enable_test(int index) {

    int error;

    printf("Setting strobe/lamp enable to true\n");
    seabreeze_set_strobe_enable(index, &error, 1);
    printf("Result is [%s]\n", get_error_string(error));
    Sleep(500);
    printf("Setting strobe/lamp enable to false\n");
    seabreeze_set_strobe_enable(index, &error, 0);
    printf("Result is [%s]\n", get_error_string(error));

}


void trigger_mode_test(int index) {

    int error;

    printf("Setting trigger mode to 3\n");
    seabreeze_set_trigger_mode(index, &error, 3);
    printf("Result is [%s]\n", get_error_string(error));
    Sleep(500);
    printf("Setting trigger mode to 0\n");
    seabreeze_set_trigger_mode(index, &error, 0);
    printf("Result is [%s]\n", get_error_string(error));

}

void read_eeprom_test(int index) {
    unsigned char serial_number[17];
    int flag;
    int error;

    printf("Getting EEPROM slot 0.\n");
    flag = seabreeze_read_eeprom_slot(index, &error, 0, serial_number, 17);
    printf("Result is (%d) [%s]\n", flag, get_error_string(error));
    serial_number[16] = '\0';
    if(flag > 0) {
        printf("From slot 0: %s\n", serial_number);
    }
}

void get_spectrum_test(int index) {

    int error;
    int flag;
    int spec_length;
    double *spectrum = 0;

    printf("Getting formatted spectrum length.\n");
    spec_length = seabreeze_get_formatted_spectrum_length(index, &error);
    printf("Result is (%d) [%s]\n", spec_length, get_error_string(error));

    if(spec_length > 0) {
        spectrum = (double *)calloc((size_t)spec_length, sizeof(double));

        printf("Getting a formatted spectrum.\n");
        flag = seabreeze_get_formatted_spectrum(index, &error, spectrum, spec_length);
        printf("Result is (%d) [%s]\n", flag, get_error_string(error));
        printf("Pixel value 20 is %1.2f\n", spectrum[20]);

        free(spectrum);
    }
}

void get_raw_spectrum_test(int index) {

    int flag;
    int error;    
    int raw_length;
    unsigned char *raw_spectrum;

    printf("Getting unformatted spectrum length.\n");
    raw_length = seabreeze_get_unformatted_spectrum_length(index, &error);
    printf("Result is (%d) [%s]\n", raw_length, get_error_string(error));

    if(raw_length > 0) {
        raw_spectrum = (unsigned char *)calloc((size_t)raw_length, sizeof(unsigned char));

        printf("Getting an unformatted spectrum.\n");
        flag = seabreeze_get_unformatted_spectrum(index, &error, raw_spectrum, raw_length);
        printf("Result is (%d) [%s]\n", flag, get_error_string(error));
        printf("Buffer values 19 and 20 are 0x%02X%02X\n", raw_spectrum[19], raw_spectrum[20]);

        free(raw_spectrum);
    }
}

void get_wavelengths_test(int index) {

    int error;
    int flag;
    int spec_length;
    double *wls = 0;

    printf("Getting formatted spectrum length.\n");
    spec_length = seabreeze_get_formatted_spectrum_length(index, &error);
    printf("Result is (%d) [%s]\n", spec_length, get_error_string(error));

    if(spec_length > 0) {
        wls = (double *)calloc((size_t)spec_length, sizeof(double));

        printf("Getting wavelengths.\n");
        flag = seabreeze_get_wavelengths(index, &error, wls, spec_length);
        printf("Result is (%d) [%s]\n", flag, get_error_string(error));
        printf("Pixel 20 is wavelength %1.2f nm\n", wls[20]);

        free(wls);
    }
}

void speed_test(int index) {
    clock_t start;
    clock_t end;
    int error;
    unsigned char c;
    int scans = 100;
    int i;
    int flag;
    long minimum_time;
    double secDiff;

    printf("Getting minimum integration time from device.\n");
    minimum_time = seabreeze_get_minimum_integration_time_micros(index, &error);
    printf("Minimum is %ld microseconds, result is [%s]\n", minimum_time, get_error_string(error));
    if(minimum_time < 0) {
        /* If there was an error, reset to a time that is supported widely. */
        minimum_time = 15000;
    }

    printf("Setting integration time to %ld usec.\n", minimum_time);
    seabreeze_set_integration_time(index, &error, minimum_time);
    printf("Result is [%s]\n", get_error_string(error));

    printf("Starting speed test with %d scans.\n", scans);

    start = clock();
    for(i = 0; i < scans; i++) {
        flag = seabreeze_get_unformatted_spectrum(index, &error, &c, 1);
        if(error != 0) {
            printf("Read error (%s) on speed test at iteration %d\n", get_error_string(error), i);
            break;
        }
    }
    end = clock();

    secDiff = (double)(end - start)/CLOCKS_PER_SEC;
    
    printf("Result is (%d) [%s]\n", flag, get_error_string(error));

    printf("Average acquisition time (msec) over %d scans: %1.3f\n", scans, secDiff/(1.0 * scans / 1000.0));
    printf("Total time elapsed: %1.2f seconds\n", secDiff);
}

int main() {
    int ec = run_all_tests();

    // Provided in case the user ran the application in a new terminal window, 
    // which would otherwise close (destroying the output) upon exit.
    char line[80];
    do {
        printf("\nType \"quit\" to exit..."); 
        fflush(stdout);

        memset(line, 0, 80);
        fgets(line, sizeof(line), stdin);

    } while (strncmp(line, "quit", 4));
    printf("\ndone.\n");

    return ec;
}

