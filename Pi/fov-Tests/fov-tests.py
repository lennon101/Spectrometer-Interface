
''' system call to:
data-collection (C) 2014, Ocean Optics Inc

Usage:
    $ ./data-collection [--index specIndex] [--step cnt:integ:avg]+ [--iterations iter]
                        [--basefile foo] [--log-level foo]
Where:

--index      takes the integral index of an enumerated Ocean Optics spectrometer (0-127)
             (default: 0, the first or only connected spectrometer found)
--dtg        the date-time-group which will appended to the header of the csv
--iterations how many times to run the complete sequence of steps (default: 1) (0 to run forever)
--basefile   prefix of output filename (i.e. '/path/to/data').  Will be automatically be
             suffixed with '-iter-00000-step-00-acq-00000.csv' for each recorded acquisition.
--log-level  one of TRACE, DEBUG, INFO, WARN, ERROR

'''

'''
NB// setup required -----------

    Turn off /etc/init.d/ocean-deamon 
        sudo ocean-deamon stop
        then unplug and replug the spectrometers 

    Set the date/time of the Pi
        sudo date --set '2016-09-14 16:30:00'
'''

import subprocess

#import the package directory for libraries
import sys
sys.path.insert(0, '../libs')
import GetFileNames
import PythonCSV
import timestamp
from time import sleep

timeStampPostfix = "timestamp"
ITERATIONS = 1; 
NUM_DEVICES = 2; 

if len(sys.argv) < 2: 
    print "\nIncorrect num arguments. \n Enter a name to prefix the file with"
else:
    spectrumPefix = sys.argv[1]

    for i in range(0, NUM_DEVICES):
        newSpectrumPefix = spectrumPefix + "-device-" + str(i)
        #dtgStr = 
        subprocess.call(["./data-collection", "--index", str(i), "--dtg", "2016:09:14:10:06:00", "--basefile", newSpectrumPefix, "--log-level", "INFO"])

        timestamparray = timestamp.gettimeStamp()

        '''
        #number = str(i).zfill(10)    
        if i == NUM_DEVICES -1:
            fileName = spectrumPefix + "-" + timeStampPostfix + ".csv"
            PythonCSV.writeToCSV(fileName, timestamparray)
        '''