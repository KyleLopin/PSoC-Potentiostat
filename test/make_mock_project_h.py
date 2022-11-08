# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""
Make a mock project.h file
Go through the file 'warnings.txt' and for each line extract the function that
was being attempted to be called and make an empty function out of it and
return a value if needed.
"""

__author__ = "Kyle Vitatus Lopin"

# standard libraries
import os


def get_h_files(_dir):
    """ Go through the directry _dir and find all files that end in .h
    and return the list of files"""
    _h_files = []
    for file in os.listdir(_dir):
        if file.endswith('.h'):
            _h_files.append(os.path.join(_dir, file))
    return _h_files


def get_undefined_func_name(_line):
    """ Take a line from the warning file and cut out the function name
    from the warning and return the function name """
    return _line.split("'")[1]


def get_extern_symbol_name(_line, _h_files):
    """ Take a line from a warning file and cut the symbol or
     function name and return the function name """
    return _line.split("unresolved external symbol _")[1].strip()


def make_mock_function(function_line):
    print(f'mock function: {function_line}')
    if 'void' not in function_line:  # return a value
        return function_line.split(';')[0] + '{return 1;}\n'
    return function_line.split(';')[0] + '{}\n'


def function_in_header(function_name, _h_files):
    for h_file in _h_files:
        # print(f"h file: {h_file}")
        with open(h_file, 'r') as open_h_file:
            for _line in open_h_file:
                # print(f"h file line: {_line}]] [{function_name}]")
                if function_name in _line:  # should only be defined once
                    print(f'returning line: {_line}')
                    if '(' in _line:  # this is a function
                        return make_mock_function(_line)
                    return _line


# get all the header files of the project, not an generated source files
project_dir = os.path.join('..', 'Amperometry_v059_2.cydsn')
h_files = get_h_files(project_dir)
print(h_files)

# first find all the function names that are missing,
# these should be warnings given by the cffi and
# copied into the warnings.txt file
functions = []
with open('warnings.txt', 'r') as _file:
    for line in _file:
        if "warning C4013:" in line:
            _function = get_undefined_func_name(line)
        elif "error LNK2001:" in line:
            _function = get_extern_symbol_name(line, h_files)
        functions.append(_function)


print(functions)
with open('project.h', 'w') as _file:
    _file.write("#ifndef _PROJECT_H_\n#define _PROJECT_H_\n\n")
    for _function in set(functions):
        checK_for_def = function_in_header(_function, h_files)
        if checK_for_def:
            _file.write(checK_for_def)
        else:
            _file.write("int " + _function + "() {return 1;}\n")
    _file.write("\n#endif\n")
