# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitautus Lopin"

# standard libraries
import importlib
import os
import subprocess
import uuid

# installed libraries
import cffi

project_dir = os.path.join('..', '..', 'Amperometry_v059_2.cydsn')
print(project_dir)


def preprocess(source, include_paths=[], compiler_options=[]):
    command = (
            [
                "gcc",
            ]
            + compiler_options
            + include_paths
            + ["-E", "-P", "-"]
        )
    print(f"command: {command}")
    return subprocess.run(command,
                          input=source, stdout=subprocess.PIPE,
                          universal_newlines=True, check=True).stdout


def load(filename, function_name: str, header_includes=[]):
    compiled_filename = filename + '_'
    file_path = os.path.join(project_dir, filename)
    source = open(file_path+'.c').read()
    raw_header = open(file_path + '.h').read()
    cdef = ""

    for line in raw_header.splitlines():
        if function_name in line:
            cdef += line
    print('999')
    cdef += "".join(header_includes)
    print(cdef)
    ffi_builder = cffi.FFI()
    ffi_builder.cdef(cdef)
    ffi_builder.set_source(compiled_filename, source,
                           include_dirs=[project_dir])
    ffi_builder.compile()

    module = importlib.import_module(compiled_filename)
    return module.lib


filename = 'lut_protocols'
module = load(filename, "LUT_make_line", header_includes=["uint16_t waveform_lut[];"])
module.LUT_make_line(3, 6, 2)
print(module.waveform_lut[5])
print(module.waveform_lut)
os.remove(filename+'_.c')

