import os
import sys
import zipfile

def visit (arg, dirname, files):
        for i in files :
                if (not(os.path.isdir(dirname.replace(arg[1], "") + i))) and (i != "vars.js"):
                        print dirname.replace(arg[1], "") + "\\" + i
                        zipf.write(dirname+ "\\" + i, dirname.replace(arg[1], "") + "\\" + i)

arg = [sys.argv[1] + "\\dohome.zip", sys.argv[1].replace("temp", "Client")]
zipf = zipfile.ZipFile(arg[0], "w")
os.path.walk(sys.argv[1].replace("temp", "Client"), visit, arg)
zipf.close();
