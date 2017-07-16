import os
import sys
import subprocess
from subprocess import Popen

msbuild = "msbuild"
if os.name != 'nt':
	msbuild = "xbuild"

proc = Popen(msbuild + " ./Build/LuminoBuild.sln /t:Build /p:Configuration=\"Release\" /p:Platform=\"x86\"")
proc.communicate()


a = ""
if len(sys.argv) >= 2:
	a = sys.argv[1]

proc = Popen("./Build/LuminoBuild/bin/Release/LuminoBuild.exe " + a)
proc.communicate()



