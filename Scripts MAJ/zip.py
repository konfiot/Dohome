import os
import zipfile

def visit (arg, dirname, files):
	for i in files :
		zipf = zipfile.ZipFile(arg, "w")
		