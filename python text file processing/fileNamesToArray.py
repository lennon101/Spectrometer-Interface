#A script that to return a list of files with the required prefix in the directory 

import glob
import os
import re

currentDir = os.getcwd(); 

spectrumDir = currentDir + "/spectrum/"

#filenames1 = glob.glob("/Users/danelennon/Google Drive/Development/Spectrometer-Interface/python text file processing/spectrum/*.txt");

def getFilesWithPrefix(prefix):

	namesArray = next(os.walk(spectrumDir))[2]

	filesWithPrefix = [];

	for item in namesArray:
		isCorrectPrefix = item.find(prefix);

		if (isCorrectPrefix >= 0):
			filesWithPrefix.append(item);

	print "The files to be processed are:"
	for item in filesWithPrefix:
		print item; 

	return filesWithPrefix 

