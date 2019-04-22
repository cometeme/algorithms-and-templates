from os import listdir, remove

files = listdir()

for file in files:
    if('.' not in file or file.endswith("exe")):
        remove(file)
