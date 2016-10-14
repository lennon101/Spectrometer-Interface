#6 Jul 16 
#A script that reads the spectrum text files, separates the capture information from the intensities 
#and supplies a function that returns the wavelength intensities in an array 

import re;

wavelengths = [];
numInfoLines = [];
extractedInfo = []; 

def read(dir):
	dirName = "spectrum/" + dir;

	file = open(dirName, "r");

	array = file.readlines();
	info = [];

	global extractedInfo
	global wavelengths;
	global numInfoLines; 

	#get the number of lines in the text file that contain info
	for i in range(len(array)):
		line = array[i].split(); 
		try:
			float(line[0]);
			numInfoLines = i;
			break 
		except ValueError:
			print("");

	#seperate the info from the wavelengths and intensities
	for i in range(numInfoLines):
		info.append(array[i]); 

	wavelengths = [];
	for i in range(numInfoLines,len(array)-1):
		line = array[i].split();
		print(line);
		#cast to float
		line[0] = float(line[0]);
		line[1] = float(line[1]);
		wavelengths.append(line);

	print "The number of intensities processed is: " + str(len(wavelengths) + 1);

	#separate the characters in the info from the data
	for element in info:
		print element;
		try:
			data = int(re.search(r'\d+', element).group());
			title = " ".join(re.findall("[a-zA-Z]+", element));
		except AttributeError:
			print("");
		infoArray = [title,data];

		extractedInfo.append(infoArray);

def getArrayOfIntensities():
	return wavelengths;

def getNumInfoLines():
	return numInfoLines; 

def getInfo():
	return extractedInfo;
