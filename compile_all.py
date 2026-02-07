#!/usr/bin/python3

import os
import glob

# TODO: Custom linker flags per file

for file in glob.glob("euler/*.txt"):
    os.system(f'./a {file}');

for file in glob.glob("examples/*.txt"):
    os.system(f'./a {file}');

