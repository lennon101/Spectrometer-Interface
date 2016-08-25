/**
* @file     demo-strobe.c
* @author   Mark Zieg <mark.zieg@oceanoptics.com>
* @date     2012-10-17
*
* Demonstrate how to set the lamp and strobe period.
*
* Note that if a lamp is hooked to the "continuous strobe" pin of a spectrometer,
* there is no way to enable the lamp without a strobe period.  That is, to switch
* from strobing behavior (continuous cycle of on-and-off) to steady state, you
* have to physically rewire the connection.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#include "api/SeaBreezeWrapper.h"

// global for convenience
const int spec_index = 0;
const int strobe_index = 0;
int error = 0;

int readBool(const char *prompt) 
{
    char c = 0;
    printf("%s", prompt);
    fflush(stdin);
    while (c != 'Y' && c != 'y' && c != 'N' && c != 'n')
        scanf("%c", &c);
    fflush(stdin);
    return c == 'Y' || c == 'y';
}

void takeThrowawaySpectrum()
{
    double d;
    seabreeze_get_formatted_spectrum(spec_index, &error, &d, 1);
    check_error(spec_index, &error, "seabreeze_get_formatted_spectrum");
}

void setIntegrationTime()
{
    seabreeze_set_integration_time(spec_index, &error, 100 * 1000);  // 100ms
    check_error(spec_index, &error, "seabreeze_set_integration_time");
}

int main(int argc, char **argv) 
{
    char ver[256];
    seabreeze_get_api_version_string(ver, sizeof(ver));
    printf("SeaBreeze API Version: %s\n", ver);

    printf("Testing lamp features.\n\n");

    seabreeze_open_spectrometer(spec_index, &error);
    if (check_error(spec_index, &error, "seabreeze_open_spectrometer"))
        return 1;

    seabreeze_set_strobe_enable(spec_index, &error, 0);
    if (check_error(spec_index, &error, "seabreeze_set_strobe_enable(0)"))
        return 1;

    int done = 0;
    while (!done)
    {
        unsigned long strobe_period_millisec = 0;

        // get settings for this test pass
        int enableLamp = readBool("Enable lamp? [yn] ");
        if (enableLamp)
        {
            printf("  Strobe interval in milliseconds: ");
            scanf("%lu", &strobe_period_millisec);
        } 

        // apply settings
        setIntegrationTime();

        seabreeze_set_verbose(1);
        seabreeze_set_strobe_enable(spec_index, &error, enableLamp);
        if (check_error(spec_index, &error, "seabreeze_set_strobe_enable"))
                break;

        seabreeze_set_continuous_strobe_period_microseconds(spec_index, &error, 
            strobe_index, strobe_period_millisec * 1000);
        if (check_error(spec_index, &error, "seabreeze_set_strobe_period_microseconds"))
            break;

        seabreeze_set_verbose(0);

        takeThrowawaySpectrum();

        printf("All settings applied successfully\n\n");

        // prepare for next pass
        done = readBool("Quit? [yn]");
    }

    seabreeze_set_strobe_enable(spec_index, &error, 0);
    if (check_error(spec_index, &error, "seabreeze_set_strobe_enable"))
        return 1;

    seabreeze_close_spectrometer(spec_index, &error);
    if (check_error(spec_index, &error, "seabreeze_close_spectrometer"))
        return 1;

    seabreeze_shutdown();

    return 0;
}
