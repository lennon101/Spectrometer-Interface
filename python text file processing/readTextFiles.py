#6 Jul 16 
#A script that reads the spectrum text files, separates the capture information from the intensities 
#and supplies a function that returns the wavelength intensities in an array 

import re;

wavelengths = [];

def read(dir):
	dirName = "spectrum/" + dir;

	file = open(dirName, "r");

	array = file.readlines();
	info = [];

	for i in range(5):
		info.append(array[i]); 

	for i in range(6,len(array)-len(info)):
		line = array[i].split();
		#cast to float
		line[0] = float(line[0]);
		line[1] = float(line[1]);
		wavelengths.append(line);

	#get the info out of the info array
	sequenceNum = int(re.search(r'\d+', info[0]).group());
	saveTime = int(re.search(r'\d+', info[1]).group());
	integrationTime = int(re.search(r'\d+', info[2]).group());
	numScansAveraged = int(re.search(r'\d+', info[3]).group());
	boxCarSmoothing = int(re.search(r'\d+', info[4]).group());

def getArrayOfIntensities():
	return wavelengths;
