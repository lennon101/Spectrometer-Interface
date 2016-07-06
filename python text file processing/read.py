#6 Jul 16 
# First attempt to read spectrum text file and format to xml 

import re;

file = open("spectrum/acquire-00.txt", "r");

array = file.readlines();
info = [];
wavelengths = [];

for i in range(5):
	info.append(array[i]); 

for line in info:
	print line;

for i in range(6,len(array)-len(info)):
	wavelengths.append(array[i]);

for line in wavelengths:
	print line;

#get the info out of the info array
sequenceNum = int(re.search(r'\d+', info[0]).group());
saveTime = int(re.search(r'\d+', info[1]).group());
integrationTime = int(re.search(r'\d+', info[2]).group());
numScansAveraged = int(re.search(r'\d+', info[3]).group());
boxCarSmoothing = int(re.search(r'\d+', info[4]).group());

