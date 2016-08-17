import csv

def printCSV(fileName):
    with open(fileName, 'rb') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            print row

def getCSVArray(fileName):
    csvArray = []
    with open(fileName, 'rb') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            csvArray.append(row)
    return csvArray

def writeToCSV(fileName,array):
    with open(fileName, 'wb') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(array)
