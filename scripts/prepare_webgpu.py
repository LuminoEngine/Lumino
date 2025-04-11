
import os
import utils

build_dir = f"{utils.root_dir}/build/tools"

os.chdir(build_dir)

utils.call("git clone -b dawn-7069 https://github.com/eliemichel/WebGPU-distribution.git")
