# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitatus Lopin"

# standard libraries
import importlib
import os

# installed libraries
import cffi

# because the test directory can be changed bases on what tests are being run,
# different paths have to be searched.
for i in range(1, 4):
    path_list = ['..'] * i + ['Amperometry_v059_2.cydsn']
    path = os.path.join(*path_list)
    if os.path.isdir(path):
        project_dir = path
        break
if not project_dir:
    raise Exception("Project directory not found")


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


def make_mock_files():  # nevermind figure it out with out these functions
    cytype_mock_file = os.path.join(project_dir, "cytypes_mock.h")
    cytype_file = os.path.join(project_dir, "cytypes_mock.h")
    if os.path.isfile(cytype_mock_file) and not os.path.isfile(cytype_file):
        os.rename(cytype_mock_file, cytype_file)


def reverse_mock_files():  # nevermind figure it out with out these functions
    cytype_mock_file = os.path.join(project_dir, "cytypes_mock.h")
    cytype_file = os.path.join(project_dir, "cytypes_mock.h")
    if not os.path.isfile(cytype_mock_file) and os.path.isfile(cytype_file):
        os.rename(cytype_file, cytype_mock_file)
