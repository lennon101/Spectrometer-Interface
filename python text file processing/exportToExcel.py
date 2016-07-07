import readTextFiles 
import fileNamesToArray
import xlwt 

book = xlwt.Workbook(encoding = "utf-8");
sheets = []

#get the list of files with the required prefix: 
fileNames = fileNamesToArray.getFilesWithPrefix("acquire0");

i = 0;

#for each file, read the file and export to a sheet
for file in fileNames: 
	sheetName = "spec " + str(i); 
	print sheetName;

	sheets.append(book.add_sheet(sheetName));

	readTextFiles.read(file)

	wavelengths = readTextFiles.getArrayOfIntensities();

	sheets[i].write(0,0,"wavelengths"); 
	sheets[i].write(0,1,"intensities");

	for row in range(2,len(wavelengths)):
		sheets[i].write(row,0,wavelengths[row-2][0])
		sheets[i].write(row,1,wavelengths[row-2][1])

	i = i + 1; 
	print i; 

book.save("acquire0.xls");