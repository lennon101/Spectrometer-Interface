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
#include <unistd.h>
#include "api/SeaBreezeWrapper.h"
 #include <stdbool.h>

 void get_spectrum(int index);
 void get_raw_spectrum(int index);
 void get_wavelengths(int index);

/* Prototypes from spectral_correction.c */
 int is_electric_dark_supported(int index, int *error_code);
 int get_edark_corrected_spectrum(int index, int *error_code, double *buffer,
 	int buffer_length);
 int do_nonlinearity_correction(int index, int *error_code,
 	double *pixels, double pixel_count);

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
 	double roll;
 	double pitch;
 	double yaw;
 	double lat;
 	double lon; 
 };

////////////////////////////////////////////////////////////////////////////////
// globals
////////////////////////////////////////////////////////////////////////////////

 int deviceIndex = 0;
 int sampleNum;
 char* filename;

 struct Dtg dtg;
 struct FlightData flightData; 

 int numReqArguments = 15;

 void usage() {
 	puts("data-collection (C) 2014, Ocean Optics Inc\n"
 		"\n"
 		"Usage:\n"
 		"    $ ./data-collection [--index deviceIndex] [--sampleNum 0] [--dtg yr mm dd hr mm ss] [--flightData roll pitch yaw lat lon]\n"
 		"                        [--filename foo]\n"
 		"\n"
 		"Where:\n"
 		"\n"
 		"--index           takes the index of an Ocean Optics spectrometer (0-127)\n"
 		"                  (default: 0, the first or only connected spectrometer found)\n"
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
 		deviceIndex =  atoi(argv[1]);
 		sampleNum = atoi(argv[2]);

 		dtg.year = atoi(argv[3]);
 		dtg.month = atoi(argv[4]);
 		dtg.day = atoi(argv[5]);
 		dtg.hour = atoi(argv[6]);
 		dtg.minutes = atoi(argv[7]);
 		dtg.seconds = atoi(argv[8]);

 		flightData.roll = atof(argv[9]);
 		flightData.pitch = atof(argv[10]);
 		flightData.yaw = atof(argv[11]);
 		flightData.lat = atof(argv[12]);
 		flightData.lon = atof(argv[13]);

 		filename = argv[14];

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
 	char type[16];
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
 		printf("...Result is (%d) [%s]\n", flag, get_error_string(error));
 		if(0 == flag) {
 			device_count++;
 		} else {
 			break;
 		}
 	}
    

 	for(i = 0; i < device_count; i++) {
 		printf("\nGetting device %d name.\n", i);
 		seabreeze_get_model(i, &error, type, sizeof(type));
 		printf("...Result is (%s) [%s]\n", type, get_error_string(error));
 	}

 	printf("Number of devices found is: %d\n", device_count);

 	for(i = 0; i < device_count; i++) {
 		printf("\nSetting device %d integration time to 100ms.\n", i);
 		seabreeze_set_integration_time_microsec(i, &error, 100000);
 		//printf("...Result is [%s]\n", get_error_string(error));
 	}

 	if (deviceIndex < device_count){
 		printf("\n\nBegin sampling using device: %d\n", deviceIndex);

 		if (sampleNum == 0){
 			get_wavelengths(deviceIndex);
 		}else if (sampleNum>=1){
 			get_spectrum(deviceIndex);
 		}
 	}else{
 		printf("deviceIndex provided in arguments is out of range of the number of devices found" );
 		usage();
 	}
 	
 	//get_raw_spectrum_test(i);

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

 void get_spectrum(int index) {

 	int error;
 	int flag;
 	int spec_length;
 	double *spectrum = 0;

 	printf("\n\nGetting formatted spectrum length.\n");
 	spec_length = seabreeze_get_formatted_spectrum_length(index, &error);
 	printf("...Result is (%d) [%s]\n", spec_length, get_error_string(error));

 	if(spec_length > 0) {
 		spectrum = (double *)calloc((size_t)spec_length, sizeof(double));

 		printf("\nGetting a formatted spectrum.\n");
 		flag = seabreeze_get_formatted_spectrum(index, &error, spectrum, spec_length);
 		printf("...Result is (%d) [%s]\n", flag, get_error_string(error));
 		printf("\tPixel value 20 is %1.2f\n", spectrum[20]);

 		FILE *f = fopen(filename, "a");
 		if (f != NULL){

 			fprintf(f, "%u,%u,%u:%u:%u,%u:%u:%u,%.11f,%.11f,%.11f,%.5f,%.5f,", sampleNum, deviceIndex, dtg.year, dtg.month, dtg.day, dtg.hour, dtg.minutes, dtg.seconds, flightData.roll, flightData.pitch, flightData.yaw, flightData.lat, flightData.lon);
 			for (unsigned pixel = 0; pixel < spec_length; pixel++)
 				fprintf(f, "%.2lf,", spectrum[pixel]);
 			fprintf(f, "\n");
 			fclose(f);
 		}
 		else{
 			printf("ERROR: can't write %s\n", "test");
 			exit(1);
 		}

 		free(spectrum);
 	}
 }

 void get_raw_spectrum(int index) {

 	int flag;
 	int error;
 	int raw_length;
 	unsigned char *raw_spectrum;

 	printf("\n\nGetting unformatted spectrum length.\n");
 	raw_length = seabreeze_get_unformatted_spectrum_length(index, &error);
 	printf("...Result is (%d) [%s]\n", raw_length, get_error_string(error));

 	if(raw_length > 0) {
 		raw_spectrum = (unsigned char *)calloc((size_t)raw_length, sizeof(unsigned char));

 		printf("\nGetting an unformatted spectrum.\n");
 		flag = seabreeze_get_unformatted_spectrum(index, &error, raw_spectrum, raw_length);
 		printf("...Result is (%d) [%s]\n", flag, get_error_string(error));
 		printf("\tBuffer values 19 and 20 are 0x%02X%02X\n", raw_spectrum[19], raw_spectrum[20]);

 		free(raw_spectrum);
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

 		FILE *f = fopen(filename, "w");
 		if (f != NULL){

 			fprintf(f, "sampleNum,deviceIndex,date,time,roll,pitch,yaw,lat,lon,");
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
 	}
 }

