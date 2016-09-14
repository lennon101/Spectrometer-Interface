
''' system call to:
data-collection (C) 2014, Ocean Optics Inc

Usage:
    $ ./data-collection [--index specIndex] [--step cnt:integ:avg]+ [--iterations iter]
                        [--basefile foo] [--log-level foo]
Where:

--index      takes the integral index of an enumerated Ocean Optics spectrometer (0-127)
             (default: 0, the first or only connected spectrometer found)
--step       zero or more colon-delimited tuples (e.g. '10:100:3'), where the first value
             is number of acquisitions to take, the second value is the integration time in
             microseconds, and the third value is the number of scans to average (no default);
             if a 4th tuple is present, it will represent a post-scan sleep in microseconds
             (scanCount may be zero for sleep-only steps)
--iterations how many times to run the complete sequence of steps (default: 1) (0 to run forever)
--basefile   prefix of output filename (i.e. '/path/to/data').  Will be automatically be
             suffixed with '-iter-00000-step-00-acq-00000.csv' for each recorded acquisition.
--log-level  one of TRACE, DEBUG, INFO, WARN, ERROR

'''

import subprocess

#import the package directory for libraries
import sys
sys.path.insert(0, '../libs')
import GetFileNames
import PythonCSV
import timestamp
from time import sleep

if len(sys.argv) < 2: 
    print "\nIncorrect num arguments. \n Enter a name to prefix the file with"
else:
    spectrumPefix = sys.argv[1]

    timeStampPostfix = "timestamp"
    ITERATIONS = 1; 
    NUM_DEVICES = 2; 

    for i in range(0, NUM_DEVICES):
        newSpectrumPefix = spectrumPefix + "-device-" + str(i)
        subprocess.call(["./data-collection", "--index", str(i), "--step", "1:100:3", "--basefile", newSpectrumPefix, "--log-level", "INFO"])

        timestamparray = timestamp.gettimeStamp()

        #number = str(i).zfill(10)    
        if i == NUM_DEVICES -1:
            fileName = spectrumPefix + "-" + timeStampPostfix + ".csv"
            PythonCSV.writeToCSV(fileName, timestamparray)