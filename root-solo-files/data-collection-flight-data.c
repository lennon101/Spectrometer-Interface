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

 const char* get_error_string(int error) {
 	static char buffer[32];
 	seabreeze_get_error_string(error, buffer, sizeof(buffer));
 	return buffer;
 }

////////////////////////////////////////////////////////////////////////////////
// inner types
////////////////////////////////////////////////////////////////////////////////

 struct Dtg{	
 	int year;
 	int month;
 	int day;
 	int hour;
 	int minutes;
 	int seconds;
 };

 struct FlightData{	
 	double alt;
 	double roll;
 	double pitch;
 	double yaw;
 	unsigned lat;
 	unsigned lon; 
 };

////////////////////////////////////////////////////////////////////////////////
// globals
////////////////////////////////////////////////////////////////////////////////

 int sampleNum;
 char* filename;

 struct Dtg dtg;
 struct FlightData flightData; 

 int numReqArguments = 15; //including the name of the executable
 unsigned scans_to_average = 3;

 void usage() {
 	puts("data-collection 2016, Dane Lennon\n"
 		"\n"
 		"Usage:\n"
 		"    $ ./data-collection sampleNum yr mm dd hr mm ss alt roll pitch yaw lat lon\n"
 		"                        filename\n"
 		"\n"
 		"Where:\n"
 		"\n"
 		"--sampleNum       An index declaring the sample number\n"
 		"					if 0 then a header file is setup, else if 1 or more, samples are saved\n"
 		"--dtg             the date-time-group which will appended to the header of the csv\n"
 		"--basefile        prefix of output filename (i.e. '/path/to/data').  Will be automatically be\n"
 		"                  prefixed for each recorded acquisition.\n"
 		);

 	exit(1);
 }

 void parseArgs(int argc, char **argv){
    // iterate over cmd-line arguments
 	if (argc == numReqArguments){
 		sampleNum = atoi(argv[1]);

 		dtg.year = atoi(argv[2]);
 		dtg.month = atoi(argv[3]);
 		dtg.day = atoi(argv[4]);
 		dtg.hour = atoi(argv[5]);
 		dtg.minutes = atoi(argv[6]);
 		dtg.seconds = atoi(argv[7]);

 		flightData.alt = atof(argv[8]);
 		flightData.roll = atof(argv[9]);
 		flightData.pitch = atof(argv[10]);
 		flightData.yaw = atof(argv[11]);
 		flightData.lat = atoi(argv[12]);
 		flightData.lon = atoi(argv[13]);

 		filename = argv[14];

 	}else if (argc > numReqArguments){
 		printf("Too many arguments supplied");
 		usage();
 	}else if (argc == 3){ 
        printf("simulating vehicle data");
        sampleNum = atoi(argv[1]);    

        dtg.year = 2016;
        dtg.month = 9;
        dtg.day = 22;
        dtg.hour = 12;
        dtg.minutes = 30;
        dtg.seconds = 30;

        flightData.alt = 10;
        flightData.roll = 2.5;
        flightData.pitch = 3.5;
        flightData.yaw = 4.5;
        flightData.lat = 250;
        flightData.lon = 350;

        filename = argv[2];

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

 	if (dtg.year == 0)
 	{
 		printf("ERROR: invalid dtg provided\n");
 		usage();
 	}

 	printf("sampleNum is: %i\n", sampleNum);

 	if (sampleNum==0){
 		printf("Setting up the header file for the csv\n");
 	}else if (sampleNum >= 1){
 		printf("performing normal sampling\n");
 	}else {
 		printf("ERROR: incorrect scanNum option provided\n");
 		usage();
 	}

    ////////////////////////////////////////////////////////////////////////////

 	for(i = 0; i < SEABREEZE_MAX_DEVICES; i++) {
 		printf("\nOpening spectrometer %d.\n", i);
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
 		if (sampleNum == 0){
            printf("\n\nset the header for device: %d\n", i);
 			get_wavelengths(i); //create the header file
 		}else if (sampleNum>=1){
            printf("\n\nBegin sampling using device: %d\n", i);
 			get_spectrum(i);
 		}
 	}
 	
 	////////////////////////////////////////////////////////////////////////////////
 	/////close the spectrometers
 	///////////////////////////////////////////////////////////////////////////////
 	for(i = 0; i < device_count; i++) {
 		printf("\nClosing spectrometer %d.\n", i);
 		flag = seabreeze_close_spectrometer(i, &error);
 		printf("...Result is (%d) [%s]\n", flag, get_error_string(error));
 	}

 	seabreeze_shutdown();

 	return 0;
 }

 void get_spectrum(int index) {

 	int error;
 	int flag;
 	int spec_length;
 	double *spectrum = 0;

 	printf("\n\nGetting formatted spectrum length.\n");
 	spec_length = seabreeze_get_formatted_spectrum_length(index, &error);
 	printf("...Result is (%d) [%s]\n", spec_length, get_error_string(error));

    double *average = (double*) malloc(spec_length * sizeof(double));


 	if(spec_length > 0) {
 		spectrum = (double *)calloc((size_t)spec_length, sizeof(double));

 		printf("\nGetting a formatted spectrum.\n");
 		flag = seabreeze_get_formatted_spectrum(index, &error, spectrum, spec_length);
 		//printf("...Result is (%d) [%s]\n", flag, get_error_string(error));

        ////////////////////////////////////////////////////////////////////////////
        // get serial number of device
        ////////////////////////////////////////////////////////////////////////////
        char serial_number[32];
        int error;

        printf("\n\nGetting serial number.\n");
        flag = seabreeze_get_serial_number(index, &error, serial_number, 32);
        printf("...Result is (%d) [%s]\n", flag, get_error_string(error));
        serial_number[31] = '\0';
        if(flag > 0) {
            printf("\tSerial number: [%s]\n", serial_number);
        }

        ////////////////////////////////////////////////////////////////////////////
        // avarage the spectrum
        ////////////////////////////////////////////////////////////////////////////
        printf("averaging %u scans\n", scans_to_average);
        memset(average, 0, spec_length * sizeof(double));
        for (int i = 0; i < scans_to_average; i++) {
            memset(spectrum, 0, spec_length * sizeof(double));
            seabreeze_get_formatted_spectrum(index, &error, spectrum, spec_length);
            for (unsigned j = 0; j < spec_length; j++)
                average[j] += spectrum[j];
        }
        for (unsigned i = 0; i < spec_length; i++)
            average[i] /= scans_to_average;

        ////////////////////////////////////////////////////////////////////////////
        // add spectrum to csv file
        ////////////////////////////////////////////////////////////////////////////

        printf("The name of the file to append to is %s\n", filename);
        const char* filename_extension = ".csv";
        char* filename_with_extension;
        filename_with_extension = malloc(strlen(filename)+4);
        strcpy(filename_with_extension,filename);
        strcat(filename_with_extension, filename_extension);
 		FILE *f = fopen(filename_with_extension, "a");
 		if (f != NULL){

 			fprintf(f, "%u,%s,%u-%u-%u,%u:%u:%u,%.11f,%.11f,%.11f,%.11f,%u,%u,", sampleNum, serial_number, dtg.year, dtg.month, dtg.day, dtg.hour, dtg.minutes, dtg.seconds, flightData.alt, flightData.roll, flightData.pitch, flightData.yaw, flightData.lat, flightData.lon);
 			for (unsigned pixel = 0; pixel < spec_length; pixel++)
 				fprintf(f, "%.2lf,", average[pixel]);
 			fprintf(f, "\n");
 			fclose(f);
 		}
 		else{
 			printf("ERROR: can't write %s\n", filename); 
 			exit(1);
 		}

 		free(spectrum);
 	}
 }

 void get_wavelengths(int index) {

 	int error;
 	int flag;
 	int spec_length;
 	double *wls = 0;

 	spec_length = seabreeze_get_formatted_spectrum_length(index, &error);
 	printf("spectrum length is (%d) [%s]\n", spec_length, get_error_string(error));

 	if(spec_length > 0) {
 		wls = (double *)calloc((size_t)spec_length, sizeof(double));

 		printf("\nGetting wavelengths.\n");
 		flag = seabreeze_get_wavelengths(index, &error, wls, spec_length);
 		printf("...Result is (%d) [%s]\n", flag, get_error_string(error));

        FILE *f;
        const char* filename_extension = ".csv";
        char* filename_with_extension;
        filename_with_extension = (char *)malloc(strlen(filename)+5);
        strcpy(filename_with_extension,filename);
        strcat(filename_with_extension, filename_extension);
        if (index == 0){
            f = fopen(filename_with_extension, "w");
        }else {
            f = fopen(filename_with_extension, "a");
        }
        
 		if (f != NULL){

 			fprintf(f, "sampleNum,serialnumber,date,time,alt,roll,pitch,yaw,lat,lon,");
 			for (unsigned pixel = 0; pixel < spec_length; pixel++)
 				fprintf(f, "%.2lf,", wls[pixel]);
 			fprintf(f, "\n");
 			fclose(f);
 		}
 		else{
 			printf("ERROR: can't write %s\n", "test");
 			exit(1);
 		}
 		free(wls);
        free(filename_with_extension);
 	}
 }


 