import urlib2
import os
import sys

urllib2.urlopen("http://mkookies.hebergratuit.com/dohome/publish_new.php")
os.remove(sys.argv[1] + "\dohome.zip")