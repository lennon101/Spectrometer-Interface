/***********************************************************************//**
 * @file    data-collection.cpp
 * @date    Feb 3, 2014
 * @author  Ocean Optics, Inc
 *
 * This is a command-line utility to perform bulk data-collection via SeaBreeze.
 *
 * Invocation and arguments: see usage()
 *
 * To-Do:
 *
 * - more complex "step" syntax, including support for EDC, NLC, etc:
 *
 *     --step 'cnt=10,integ=100ms,avg=3,delay=50ms,edc=on,nlc=on,boxcar=5'
 *
 * - or, move to an external file specifying steps (possibly read at STDIN):
 *
 *   <steps>
 *     <step label="laser" count="1"  integ="10ms"  averaging="1" delay="0ms"  edc="on" nlc="on" boxcar="0" />
 *     <step label="raman" count="10" integ="100ms" averaging="3" delay="50ms" edc="on" nlc="on" boxcar="5" />
 *   </steps>
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
 */

#include "api/SeaBreezeWrapper.h"
#include "api/seabreezeapi/SeaBreezeAPIConstants.h"
#include "common/Log.h"
#include "util.h"

#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#endif

 using std::string;
 using std::vector;

#define MAX_EEPROM_LENGTH   15
#define MAX_DEVICES         32
#define MAX_PIXELS        2048

////////////////////////////////////////////////////////////////////////////////
// inner types
////////////////////////////////////////////////////////////////////////////////

 struct Step
 {
    unsigned long scanCount;
    unsigned long integrationTimeMicrosec;
    unsigned int  scansToAverage;
    unsigned long postScanSleepMicroseconds;
};

struct Dtg
{
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minutes;
    unsigned int seconds;
};

////////////////////////////////////////////////////////////////////////////////
// constants
////////////////////////////////////////////////////////////////////////////////

static const char *rcs_id __attribute__ ((unused)) =
"$Header: http://gforge.oceanoptics.com/svn/seabreeze/releases/Release_2014_10_01-1730-3.0/sample-code/cpp/data-collection.cpp 1215 2014-08-07 21:52:36Z mzieg $";

////////////////////////////////////////////////////////////////////////////////
// globals
////////////////////////////////////////////////////////////////////////////////

int specIndex = 0;
int error = ERROR_SUCCESS;

unsigned iterations = 1;
unsigned scanNum;
string basefile = "data";
vector<Step> steps;
Dtg dtg;

static const struct option opts[] =
{
    { "basefile",   required_argument, NULL, 0 },
    { "index",      required_argument, NULL, 0 },
    { "scanNum", required_argument, NULL, 0 },
    { "log-level",  required_argument, NULL, 0 },
    { "dtg",       required_argument, NULL, 0 },
    { NULL,         no_argument,       NULL, 0 }
};

void usage() {
    puts("data-collection (C) 2014, Ocean Optics Inc\n"
       "\n"
       "Usage:\n"
       "    $ ./data-collection [--index specIndex] [--scanNum 0/1] [--dtg yr:mm:dd:hr:mm:ss] \n"
       "                        [--basefile foo] [--log-level foo]\n"
       "\n"
       "Where:\n"
       "\n"
       "--index           takes the index of an Ocean Optics spectrometer (0-127)\n"
       "                  (default: 0, the first or only connected spectrometer found)\n"
       "--scanNum         An index declaring the scan number\n"
                            "if 0 then a header file is setup, else if 1 or more, samples are saved\n"
       "--dtg             the date-time-group which will appended to the header of the csv\n"
       "--basefile        prefix of output filename (i.e. '/path/to/data').  Will be automatically be\n"
       "                  suffixed with '-iter-00000-step-00-acq-00000.csv' for each recorded acquisition.\n"
       "--log-level       one of TRACE, DEBUG, INFO, WARN, ERROR\n"
       );

    exit(1);
}

bool hasError(const char *label)
{
    if (error != ERROR_SUCCESS)
    {
        char msg[80];
        seabreeze_get_error_string(error, msg, sizeof(msg));
        seabreeze_log_error("error calling %s: %s\n", label, msg);
    }
    return error != 0;
}

void parseArgs(int argc, char **argv)
{
    int longIndex = 0;

    // iterate over cmd-line arguments
    while (1)
    {
        // read next option
        int opt = getopt_long(argc, argv, "", opts, &longIndex);

        // no more options
        if (opt == -1)
            break;

        // was this a valid argument?
        if (opt == 0){
            string option(opts[longIndex].name);

            if (option == "index")
                specIndex = atoi(optarg);
            else if (option == "scanNum")
                scanNum = atoi(optarg);
            else if (option == "basefile")
                basefile = optarg;
            else if (option == "log-level")
                seabreeze_log_set_level_string(optarg);
            else if (option == "dtg")
            {
                vector<string> tuples = OOI::Util::split(string(optarg), ':');
                
                dtg.year = strtol(tuples[0].c_str(), NULL, 10);
                dtg.month = strtol(tuples[1].c_str(), NULL, 10);
                dtg.day = strtol(tuples[2].c_str(), NULL, 10);
                dtg.hour = strtol(tuples[3].c_str(), NULL, 10);
                dtg.minutes = strtol(tuples[4].c_str(), NULL, 10);
                dtg.seconds = strtol(tuples[5].c_str(), NULL, 10);
            }
            else
                usage();
        }
        else
            usage();
    }

     Step step; //1:100:3
     step.scanCount                 = 1;
     step.integrationTimeMicrosec   = 100;
     step.scansToAverage            = 3;
     step.postScanSleepMicroseconds = 0;
     steps.push_back(step);
 }

 int main(int argc, char **argv)
 {
    seabreeze_log_set_level_string("info");

    ////////////////////////////////////////////////////////////////////////////
    // parse & validate arguments
    ////////////////////////////////////////////////////////////////////////////

    seabreeze_log_debug("processing cmd-line args");
    parseArgs(argc, argv);

    if (dtg.year == 0)
    {
        printf("ERROR: no dtg provided\n");
        usage();
    }

    if (scanNum==0){
        printf("Setting up the header file for the csv\n");
    }else if (scanNum >= 1){
        printf("performing normal sampling\n");
    }else {
        printf("ERROR: incorrect scanNum option provided\n");
        usage();
    }

    if (iterations > 0)
        seabreeze_log_info("The following step%s will be executed %lu time%s:",
            steps.size() > 1 ? "s" : "", iterations, iterations > 1 ? "s" : "");
    else
        seabreeze_log_info("The following step%s will be executed indefinitely:",
            steps.size() > 1 ? "s" : "");

    for (unsigned i = 0; i < steps.size(); i++)
    {
        seabreeze_log_info("Step #%u", i + 1);
        seabreeze_log_info("    scan count       = %lu", steps[i].scanCount);
        seabreeze_log_info("    integration time = %lu microsec", steps[i].integrationTimeMicrosec);
        seabreeze_log_info("    scans to average = %lu", steps[i].scansToAverage);
        seabreeze_log_info("    post-scan sleep  = %lu microsec", steps[i].postScanSleepMicroseconds);
        seabreeze_log_info("");
    }

    ////////////////////////////////////////////////////////////////////////////
    // initialize SeaBreeze
    ////////////////////////////////////////////////////////////////////////////

    // give SeaBreeze time to fully instantiate
    seabreeze_log_debug("initializing SeaBreeze");
    sleep(1);

    seabreeze_open_spectrometer(specIndex, &error);
    if (hasError("open_spectrometer"))
        exit(1);

    unsigned pixels = seabreeze_get_formatted_spectrum_length(specIndex, &error);
    if (hasError("get_formatted_spectrum_length"))
        exit(1);

    double wavelengths[pixels];
    seabreeze_get_wavelengths(specIndex, &error, wavelengths, sizeof(wavelengths));
    if (hasError("get_wavelengths"))
        exit(1);

    ////////////////////////////////////////////////////////////////////////////
    // perform data collection
    ////////////////////////////////////////////////////////////////////////////

    // iterate over iterations
    unsigned iteration = 0;
    bool done = false;

    if (scanNum ==0) {
        // save wavelengths to file
        char filename[256];
        snprintf(filename, sizeof(filename), "%s.csv",basefile.c_str());

        seabreeze_log_info("saving wavelengths %s", filename);
        FILE *f = fopen(filename, "w");
        if (f != NULL){
            fprintf(f, "scan,date,time,lat,long,roll,pitch,yaw,");
            for (unsigned i = 0; i < pixels; ++i)
                fprintf(f, "%.2lf,", wavelengths[i]);
            fprintf(f, "\n");
            fclose(f);
        }
        else
        {
            printf("ERROR: can't write %s\n", filename);
            exit(1);
        }
    } else {
        while (!done)
        {
            seabreeze_log_debug("processing iteration #%u", iteration + 1);

        // iterate over steps
            for (unsigned stepCount = 0; stepCount < steps.size(); stepCount++)
            {
                seabreeze_log_debug("processing step #%d", stepCount + 1);
                Step& step = steps[stepCount];

            // handle sleep-only steps
                if (step.scanCount == 0)
                {
                    seabreeze_log_info("sleeping %lu microseconds", step.postScanSleepMicroseconds);
                    usleep(step.postScanSleepMicroseconds);
                    continue;
                }

            // apply settings for this step
                seabreeze_set_integration_time_microsec(specIndex, &error, step.integrationTimeMicrosec);
                if (hasError("set_integration_time_microsec"))
                    exit(1);

            // iterate over scans
                for (unsigned scanCount = 0; scanCount < step.scanCount; scanCount++)
                {
                // process acquisition
                    seabreeze_log_debug("collecting scan %d of %lu", scanCount + 1, step.scanCount);
                    double spectrum[pixels];
                    seabreeze_get_formatted_spectrum(specIndex, &error, spectrum, sizeof(spectrum));
                    if (hasError("get_formatted_spectrum"))
                        exit(1);

                // perform multi-scan averaging (used to be in SeaBreeze, now application code)
                    if (step.scansToAverage > 1)
                    {
                        double tmp[pixels];
                        for (unsigned acqCount = 1; acqCount < step.scansToAverage; acqCount++)
                        {
                            seabreeze_get_formatted_spectrum(specIndex, &error, tmp, sizeof(tmp));
                            if (hasError("get_formatted_spectrum"))
                                exit(1);
                            for (unsigned pixel = 0; pixel < pixels; pixel++)
                                spectrum[pixel] += tmp[pixel];
                        }
                        for (unsigned pixel = 0; pixel < pixels; pixel++)
                            spectrum[pixel] /= step.scansToAverage;
                    }

                // save averaged acquisition to file
                    char filename[256];
                    snprintf(filename, sizeof(filename), "%s.csv",
                        basefile.c_str());

                    float lat = 0.00;
                    float lon = 0.00;
                    float roll = -30.00;
                    float pitch = 0.00;
                    float yaw = 0.00;

                    seabreeze_log_info("saving %s", filename);
                    FILE *f = fopen(filename, "a");
                    if (f != NULL)
                    {

                        fprintf(f, "%u,%u:%u:%u,%u:%u:%u,%.2f,%.2f,%.2f,%.2f,%.2f,", scanNum, dtg.year, dtg.month, dtg.day, dtg.hour, dtg.minutes, dtg.seconds, lat,lon, roll, pitch, yaw);
                        for (unsigned pixel = 0; pixel < pixels; pixel++)
                            fprintf(f, "%.2lf,", spectrum[pixel]);
                        fprintf(f, "\n");
                        fclose(f);
                    }
                    else
                    {
                        printf("ERROR: can't write %s\n", filename);
                        exit(1);
                    }

                // perform post-scan delay
                    if (step.postScanSleepMicroseconds > 0)
                    {
                        seabreeze_log_debug("sleeping %lu microseconds", step.postScanSleepMicroseconds);
                        usleep(step.postScanSleepMicroseconds);
                    }
                }
            }

            iteration++;
            if (iterations > 0 && iteration >= iterations)
                done = true;
        }
    }


    

    ////////////////////////////////////////////////////////////////////////////
    // shutdown
    ////////////////////////////////////////////////////////////////////////////

    seabreeze_close_spectrometer(specIndex, &error);

    return 0;
}