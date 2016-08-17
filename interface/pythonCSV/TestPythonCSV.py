# import python libs
import os

# import user define lib
import GetFileNames
import PythonCSV

currentDir = os.getcwd();
spectrumDir = currentDir + "/spectrum/"

fileNames = GetFileNames.getFilesWithPrefix("testData-iter-00001-step-01-acq-", spectrumDir)
print "\nThe files to be processed are:"
for item in fileNames:
    print item;

print "\nThe array gained from the first file read is:"
csvArray = PythonCSV.getCSVArray(spectrumDir + fileNames[1])
print csvArray

print "\nWrite some data to the first file read:"
arrayOfData = [['r1 c1','r1 c2'], ['row2']]
PythonCSV.writeToCSV(spectrumDir + "test", arrayOfData)