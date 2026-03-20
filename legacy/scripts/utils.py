#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import shutil
import subprocess
import os.path
import zipfile
import platform
import os.path
from distutils import dir_util

root_dir = os.path.abspath(os.path.dirname(__file__) + '/..')
print(f"root_dir: {root_dir}")

def cdToScript():
	os.chdir(os.path.dirname(os.path.abspath(__file__)))
    
def cd_to_root():
	os.chdir(root_dir)

def call(cmd):
	""" call command line.
	"""

	print(f"\033[32m{cmd}\033[0m")
	p = subprocess.Popen(cmd, shell=True)
	ret = p.wait()
	if ret != 0:
		raise Exception("Process call failed.")
	print('')

def mkdir(path):
	if not os.path.exists(path):
		os.mkdir(path)

def cp(src, dst):
	shutil.copyfile(src, dst)
	print('Copied ' + src + ' > ' + dst)

# フォルダコピー
def cp_dir(src, dst):
	dir_util.copy_tree(src, dst)
	print('Copied ' + src + ' > ' + dst)

class CurrentDir:
	def __init__(self, path):
		self.prev = os.getcwd()
		self.path = path
	def __enter__(self):
		os.chdir(self.path)
		print("cd: " + os.getcwd())
		return self
	def __exit__(self, type, value, traceback):
		os.chdir(self.prev)
		print("cd: " + os.getcwd())
