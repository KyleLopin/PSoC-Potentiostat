# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""
Helper functions used for unit and integration testing including a file to
load c files into python and convert c arrays to python lists for testing
"""

__author__ = "Kyle Vitatus Lopin"

# standard libraries
import importlib
import os
import sys

# installed libraries
import cffi

# because the test directory can be changed bases on what tests are being run,
# different paths have to be searched.
project_dir = None  # initialize and raise an exception if it in not changed
root_dir = None
for i in range(1, 4):
    path_list = ['..'] * i + ['Amperometry_v059_2.cydsn']
    path = os.path.join(*path_list)
    if os.path.isdir(path):
        project_dir = path
        root_dir = os.path.join(*(['..'] * i))
        break
if not project_dir:
    raise Exception("Project directory not found")
MOCK_FILE_DIR = os.path.join(root_dir, 'test', 'mock_files')


def load_file(_filename):
    """
    Return the raw source (.c) and header (.h) text of the file given
    Args:
        _filename: string, name of the file WITHOUT an extension

    Returns: text of the .c and .h files

    """
    file_path = os.path.join(project_dir, _filename)
    with open(file_path + '.c', 'r', encoding="utf-8") as _file:
        source = _file.read()
    with open(file_path + '.h', 'r', encoding="utf-8") as _file:
        raw_header = _file.read()
    return source, raw_header


def remove_compiled_files():
    files = os.listdir('.')
    # print(f"files: {files}")
    for file in files:
        if file.endswith(('.c', '.so', '.o')):
            if file.startswith('py_test'):
                # print(f"removing file: {file}")
                os.remove(file)


def load(_filenames, function_names: list[str], header_includes: list[str] = [],
         compiled_file_end: str = "",
         print_debug=True):
    """
    Takes in a list of c files and functions and makes a compiled module out
    of them that can be used in python programs.  Originally developed for
    unit testing c files in python.

    NOTE: errors may show up from missing functions and variables, use the
    make_mock_project_h.py file to make the project.h file that is needed to mock those
    errors away

    Args:
        _filenames: string, or list of strings of the file names of the c files to load
        function_names: list of the names of the functions that will be tested
        header_includes: list of strings to add to the header file, this is needed
        if you want to use a variable that is not in a file in _filenames
        compiled_file_end: string, the aggrecated source files will be made into
        1 c file with the name pytest_+comipiled_file_end

    Returns: the compiled files in a module

    """
    compiled_filename = 'py_test_' + compiled_file_end  # name of saved file
    if type(_filenames) is str:  # if just a string, just load 1 file
        source, raw_header = load_file(_filenames)
    else:  # load each file in the list
        source = ""
        raw_header = ""
        for filename in _filenames:
            next_src, next_raw_head = load_file(filename)
            source += next_src
            raw_header += next_raw_head
    if print_debug:  # define the TESTING flag
        source = "#define TESTING" + source
    # make the cdef with just the functions we want
    # cdef can not deal with comments or directives
    cdef = ""
    # split the str by ;, or lines ?
    lines = raw_header.split('\n')
    for index, line in enumerate(lines):
        # print(f"line: {line}")
        # print("============================")
        for function_name in function_names:
            if function_name in line:
                cdef += line
                if ';' not in line:  # if the head is 2 lines get the second line
                    cdef += lines[index+1]
                cdef += '\n'  # ; was stripped out so add it and the line break
    # everything is made, so put it together and compile it
    cdef += "".join(header_includes)
    # print(f"function names: {function_names}")
    print(f"cdef: {cdef}")
    ffi_builder = cffi.FFI()
    ffi_builder.cdef(cdef)
    # ffi_builder.new("struct RunParams run_params")
    # there should be mocked file in the current directory also so include that
    ffi_builder.set_source(compiled_filename, source,
                           include_dirs=[project_dir, ".",
                                         MOCK_FILE_DIR])
    ffi_builder.compile()
    # import the module and return it
    sys.path.append(os.getcwd())  # make sure the file can be found
    _module = importlib.import_module(compiled_filename)
    return _module.lib, _module.ffi


def convert_c_array_to_list(c_array, start_index, end_index):
    """
    Convert a c array to a python list.
    Args:
        c_array: array from a cffi c module
        start_index: where to start in the array
        end_index: where to stop

    Returns: list

    """
    return_list = []
    for i in range(start_index, end_index):
        return_list.append(c_array[i])
    return return_list
