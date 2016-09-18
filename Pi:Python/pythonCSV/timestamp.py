# import python libs
import time
import datetime

# import user define lib
import PythonCSV

#title = "%s:%s:%s" %day %month %year
#csvfile.write(["scans initiated on:", title])

def gettimeStamp():
    print "Current date & time " + time.strftime("%c")
    dt = datetime.datetime.now()

    year = '%s' % dt.year
    month = '%s' % dt.month
    day = '%s' % dt.day

    hour = '%s' % dt.hour
    minute = '%s' % dt.minute
    second = '%s' % dt.second

    timestamp = [[year, month, day, hour, minute, second]]
    return timestamp

