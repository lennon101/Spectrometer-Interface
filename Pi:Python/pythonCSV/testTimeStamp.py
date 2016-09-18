import PythonCSV
import timestamp

timestamparray = timestamp.gettimeStamp()
PythonCSV.appendToCSV('testData-iter-00001-step-01-acq-00001.csv', timestamparray)
