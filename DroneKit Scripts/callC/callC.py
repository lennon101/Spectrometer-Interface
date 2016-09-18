from dronekit import connect
import subprocess

#import the package directory for libraries
import sys
#sys.path.insert(0, '../libs')

#./seabreeze_test_posix2 0 1 testing.csv

subprocess.call(["./../../home/root/seabreeze_test_posix2", "0","1","callfrompython"]);

subprocess.call(["pwd"]);
