# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitatus Lopin"

# standard libraries
import importlib
import re
import subprocess

# installed libraries
import cffi


def preprocess(source):
    print(source)
    return subprocess.run(['gcc', '-E', '-P', '-'],
                          input=source, stdout=subprocess.PIPE,
                          universal_newlines=True,
                          check=True).stdout


def load(filename):
    source = open(filename+'.c').read()
    headers = open(filename+'.h').read()
    # headers = preprocess(open(filename+'.h').read())
    # headers = preprocess("".join(re.findall('\s*#include\s+.*', source)))
    headers = "uint16_t LUT_make_line(uint16_t start, uint16_t end, uint16_t index);\n" \
              "uint16_t waveform_lut[4000];"
    # headers = re.sub(r"#if .", "", headers)
    print(headers)
    ffibuilder = cffi.FFI()
    ffibuilder.cdef(headers)
    ffibuilder.set_source(filename + '_', source)
    ffibuilder.compile()

    module = importlib.import_module(filename + '_')
    return module.lib


module = load('lut_protocols')
module.LUT_make_line(3, 6, 2)
print(module.waveform_lut[5])
