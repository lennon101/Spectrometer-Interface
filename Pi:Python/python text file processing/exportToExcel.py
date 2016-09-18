import readTextFiles 
import fileNamesToArray
import xlwt 

book = xlwt.Workbook(encoding = "utf-8");
sheets = []

prefix = "test"
bookName = "twoSpectrometers"
exportedFileName = bookName + ".xls"

#get the list of files with the required prefix: 
fileNames = fileNamesToArray.getFilesWithPrefix(prefix);

i = 0;

#for each file, read the file and export to a sheet
for file in fileNames: 
	sheetName = file; 
	print "The sheetname will be: " + sheetName;

	sheets.append(book.add_sheet(sheetName));

	readTextFiles.read(file);

	wavelengths = readTextFiles.getArrayOfIntensities();
	info = readTextFiles.getInfo();	

	if readTextFiles.getNumInfoLines() != 0:
		sheets[i].write(0,0,"wavelengths"); 
		sheets[i].write(0,1,"intensities");
	else:
		sheets[i].write(0,0,"Spectrum 1"); 
		sheets[i].write(0,1,"Spectrum 2");
		sheets[i].write(0,2,"Absolute Values");

	for row in range(2,len(wavelengths)):
		intensity1 = wavelengths[row-2][0];
		intensity2 = wavelengths[row-2][1];

		#write the values to the sheet
		sheets[i].write(row,0,intensity1)
		sheets[i].write(row,1,intensity2)

		#calculate the absolute values 
		sheets[i].write(row,2,intensity2/intensity1)

	for line in info: 
		print line; 

	i = i + 1; 

book.save(exportedFileName);