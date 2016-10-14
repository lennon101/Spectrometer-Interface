/*******************************************************
 * File:    seabreeze_test_linux.c
 * Date:    July 2009
 * Author:  Ocean Optics, Inc.
 *
 * This is a test program to exercise some of the
 * SeaBreeze functionality.  This will not compile
 * under Windows, so if you are using Visual Studio,
 * remove this file from the project.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2014, Ocean Optics Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************/

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/time.h>
 #include <pthread.h>
#include <unistd.h>
#include "api/SeaBreezeWrapper.h"
 #include <stdbool.h>
 #include "util.h"
 #include <time.h>
 #include <string.h>

 #ifdef _WIN32
#include <windows.h>
#endif

 void get_spectrum(int index);
 void get_wavelengths(int index);
 void set_optimum_integ_time(int index);

 const char* get_error_string(int error) {
 	static char buffer[32];
 	seabreeze_get_error_string(error, buffer, sizeof(buffer));
 	return buffer;
 }

////////////////////////////////////////////////////////////////////////////////
// globals
////////////////////////////////////////////////////////////////////////////////

 char* filename;
 unsigned integrationTime;

 int numReqArguments = 3; //including the name of the executable

 void usage() {
 	puts("get_integ_time 2016, Dane Lennon\n"
 		"\n"
 		"Usage:\n"
 		"    $ ./get_integ_time default_integration_time filename\n"
 		"\n"
 		"Where:\n"
 		"\n"
 		"    filename        prefix of output filename (..._integ_times.csv)\n\n"
 		);

 	exit(1);
 }

 void parseArgs(int argc, char **argv){
    // iterate over cmd-line arguments
 	if (argc == numReqArguments){
 		integrationTime = atoi(argv[1]);
        filename = argv[2];

 	}else if (argc > numReqArguments){
 		printf("Too many arguments supplied");
 		usage();
 	}else {
 		printf("Expecting atleast %u arguments\n", numReqArguments);
 		usage();
 	}
 }

 int main(int argc, char *argv[]) {
 	int flag;
 	int error;
 	int device_count = 0;
 	int i;

    ////////////////////////////////////////////////////////////////////////////
    // parse & validate arguments
    ////////////////////////////////////////////////////////////////////////////
 	parseArgs(argc, argv);

    ////////////////////////////////////////////////////////////////////////////
    // find the devices connected
    ////////////////////////////////////////////////////////////////////////////

 	for(i = 0; i < SEABREEZE_MAX_DEVICES; i++) {
 		printf("\nAttempting to open spectrometer %d.\n", i);
 		flag = seabreeze_open_spectrometer(i, &error);
 		//printf("...Result is (%d) [%s]\n", flag, get_error_string(error));
 		if(0 == flag) {
 			device_count++;
 		} else {
 			break;
 		}
 	}

 	printf("Number of devices found is: %d\n", device_count);

 	for(i = 0; i < device_count; i++) {
        printf("\n\nFind optimum integ time for device: %d\n", i);
        set_optimum_integ_time(i);
 	}
 	
 	////////////////////////////////////////////////////////////////////////////////
 	/////close the spectrometers
 	///////////////////////////////////////////////////////////////////////////////
 	for(i = 0; i < device_count; i++) {
 		printf("\nClosing spectrometer %d.\n", i);
 		flag = seabreeze_close_spectrometer(i, &error);
 		//printf("...Result is (%d) [%s]\n", flag, get_error_string(error));
 	}

 	seabreeze_shutdown();

 	return 0;
 }

 void set_optimum_integ_time(int i) {
    ////////////////////////////////////////////////////////////////////////////////
    /////find optimum integration time
    ///////////////////////////////////////////////////////////////////////////////
    double max = 16000;
    double upper = max*0.95;
    double lower = max*0.85; 
    unsigned testTime = integrationTime; //in 
    unsigned incrementTime = testTime/2;

    int error;
    int flag;
    int spec_length;
    double *spectrum = 0;
    
    while (true){
        printf("\nSetting device %d integration time to %d.\n", i, testTime);
        seabreeze_set_integration_time_microsec(i, &error, testTime * 1000);
        printf("...Result is [%s]\n", get_error_string(error));

        //get the spectrum for the current device
        spec_length = seabreeze_get_formatted_spectrum_length(i, &error);

        if(spec_length > 0) {
            spectrum = (double *)calloc((size_t)spec_length, sizeof(double));
            printf("\nGetting a formatted spectrum.\n");
            flag = seabreeze_get_formatted_spectrum(i, &error, spectrum, spec_length);
            printf("...Result is (%d) [%s]\n", flag, get_error_string(error));
        }
        //find the peak value
        double peak = 0; 
        for (unsigned pixel = 0; pixel < spec_length; pixel++){
            if (spectrum[pixel]>peak){
                peak = spectrum[pixel];
            }
        }
        printf("\tPeak value is %1.2f\n", peak);
        //if max value greater than upper, decrease integ time 
        if (peak > upper){
            testTime = testTime - incrementTime; 
            incrementTime = incrementTime/2;
            if (testTime < 0){
                printf("\nIntegration time fell below zero, spectrometer in area too bright\n");
                printf("\nSetting device %d integration time to %d.\n", i, integrationTime);
                break;
            }     
        }
        //else if max value smaller than lower, increase integ time 
        else if (peak < lower){
            testTime = testTime + incrementTime;
            if (testTime > 1000){ //greater than 1 s
                printf("\nIntegration time too high, spectrometer cannot get enough light\n");
                printf("\nSetting device %d integration time to %d.\n", i, integrationTime);
                break;
            } 
        }
        //else this is the right integ time --> break 
        else {
            integrationTime = testTime;
            break; 
        }
    }
    seabreeze_set_integration_time_microsec(i, &error, integrationTime * 1000);

    FILE *file;
    const char* filename_extension = "_integ_times.csv";
    char* filename_with_extension;
    filename_with_extension = malloc(strlen(filename)+17);
    strcpy(filename_with_extension,filename);
    strcat(filename_with_extension, filename_extension);

    if (i == 0){
        file = fopen(filename_with_extension, "w");
    }else {
        file = fopen(filename_with_extension, "a");
    }

    ////////////////////////////////////////////////////////////////////////////
    // get serial number of device
    ////////////////////////////////////////////////////////////////////////////
    char serial_number[32];

    printf("\n\nGetting serial number.\n");
    flag = seabreeze_get_serial_number(i, &error, serial_number, 32);
    printf("...Result is (%d) [%s]\n", flag, get_error_string(error));
    serial_number[31] = '\0';
    if(flag > 0) {
        printf("\tSerial number: [%s]\n", serial_number);
    }

    if (file != NULL){
        printf("\tfile to append to is %s\n", filename_with_extension);
        if (i==0){
            fprintf(file, "serialnumber,integration_time\n");
        }
        fprintf(file, "%s,", serial_number);
        fprintf(file, "%d\n", integrationTime);
        fclose(file);
    }
    else{
        printf("ERROR: can't write %s\n", filename_with_extension);
        exit(1);
    }
    printf("\tOptimum integration time for device %d is %dms\n", i,testTime);
    free(spectrum);
 }





 