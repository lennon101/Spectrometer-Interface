import subprocess
from time import sleep

#import the package directory for libraries
import sys
sys.path.insert(0, '../libs')
import timestamp

tsArr = timestamp.gettimeStamp()

for scanNum in range(0,10):
	for i in range(2):
		tsArr = timestamp.gettimeStamp()
		subprocess.call(["./data-collection", str(i), str(scanNum),tsArr[0],tsArr[1],tsArr[2],tsArr[3],tsArr[4],tsArr[5],"0","0","0","0","0","test.csv"]);
		sleep(1)