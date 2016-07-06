import read 
import xlwt 

book = xlwt.Workbook(encoding = "utf-8");
sheet1 = book.add_sheet("Sheet 1");

wavelengths = read.getArrayOfIntensities();

sheet1.write(0,0,"wavelengths"); 
sheet1.write(0,1,"intensities");

for row in range(2,len(wavelengths)):
	sheet1.write(row,0,wavelengths[row-2][0])
	sheet1.write(row,1,wavelengths[row-2][1])

book.save("test.xls");