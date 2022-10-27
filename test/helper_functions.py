# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitatus Lopin"

# standard libraries
import importlib
import os

# installed libraries
import cffi

project_dir = os.path.join('..', 'Amperometry_v059_2.cydsn')


def load(_filename, function_names: str, header_includes=[],
         compiled_file_end=""):
    compiled_filename = _filename + '_' + compiled_file_end
    file_path = os.path.join(project_dir, _filename)
    with open(file_path+'.c', 'r') as fp:
        source = fp.read()
    with open(file_path+'.h', 'r') as fp:
        raw_header = fp.read()
    cdef = ""

    for line in raw_header.splitlines():
        for function_name in function_names:
            if function_name in line:
                cdef += line
    cdef += "".join(header_includes)
    ffi_builder = cffi.FFI()
    ffi_builder.cdef(cdef)
    ffi_builder.set_source(compiled_filename, source,
                           include_dirs=[project_dir])
    ffi_builder.compile()

    _module = importlib.import_module(compiled_filename)
    return _module.lib


def convert_c_array_to_list(c_array, start_index, end_index):
    return_list = []
    for i in range(start_index, end_index):
        return_list.append(c_array[i])
    return return_list
