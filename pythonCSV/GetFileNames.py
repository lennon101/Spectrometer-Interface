# A script that returns a list of files with the required prefix in the directory

import os


def getFilesWithPrefix(prefix, spectrumDir):
    namesArray = next(os.walk(spectrumDir))[2]
    filesWithPrefix = [];
    for item in namesArray:
        isCorrectPrefix = item.find(prefix);
        if (isCorrectPrefix >= 0):
            filesWithPrefix.append(item);

    return filesWithPrefix
