# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitatus Lopin"

# standard libraries
import os
import re

# installed libraries
import cffi
import pycparser
file_dir = os.path.join('..', 'Amperometry_v059_2.cydsn')


def comment_remover(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " " # note: a space and not an empty string
        else:
            return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)


def load(filename):
    # get header
    file = os.path.join(file_dir, filename+'.c')
    print(file)
    header = open(file).read()
    # print(header)
    parser = pycparser.CParser()
    _header = comment_remover(header)
    print('========llll')
    print(_header)
    ast = parser.parse(_header)
    print(ast)


load('lut_protocols')


