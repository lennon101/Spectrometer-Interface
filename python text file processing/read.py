#6 Jul 16 
# First attempt to read spectrum text file and format to xml 

import re;

file = open("spectrum/acquire-00.txt", "r");

array = file.readlines();
info = [];
wavelengths = [];

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
