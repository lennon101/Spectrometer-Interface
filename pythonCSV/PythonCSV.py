import csv

def printCSV(fileName):
    with open(fileName, 'rb') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            print row
    csvfile.close()

def getCSVArray(fileName):
    csvArray = []
    with open(fileName, 'rb') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            csvArray.append(row)
    csvfile.close()
    return csvArray

def writeToCSV(fileName,array):
    with open(fileName, 'wb') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(array)
    csvfile.close()

def appendToCSV(fileName,array):
    with open(fileName, 'ab') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(array)
    csvfile.close()
