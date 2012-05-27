import os
import sys
import zipfile

def visit (arg, dirname, files):
	zipf = zipfile.ZipFile(arg[0], "w")
	for i in files :
		zipf.write(dirname.replace(arg[1], "") + i)

os.path.walk(sys.argv[1].replace("temp", "Client"), visit, [sys.argv[1] + "/dohome.zip", sys.argv[1].replace("temp", "Client")])