# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitautus Lopin"

# standard libraries
import importlib
import os
import subprocess
import unittest

# installed libraries
import cffi

# local files
import py_unit_test.helper_functions as helper_funcs
import solutions

project_dir = os.path.join('..', '..', 'Amperometry_v059_2.cydsn')
print(project_dir)


def load(_filename, function_name: str, header_includes=[]):
    compiled_filename = _filename + '_'
    file_path = os.path.join(project_dir, _filename)
    with open(file_path+'.c', 'r') as fp:
        source = fp.read()
    with open(file_path+'.h', 'r') as fp:
        raw_header = fp.read()
    cdef = ""

    for line in raw_header.splitlines():
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


class LuTMakeLineTestCase(unittest.TestCase):
    def setUp(self):
        self._filename = 'lut_protocols'
        self.module = load(self._filename, "LUT_make_line",
                           header_includes=["uint16_t waveform_lut[];"])

    def tearDown(self) -> None:
        os.remove(self._filename+'_.c')

    def test1(self):
        """ Test LUT make line function """
        index = self.module.LUT_make_line(0, 10, 0)
        print(f"index: {index}")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, index)
        print(f"waveform: {waveform}")
        self.assertEqual(index, 11)
        self.assertListEqual(waveform, solutions.test1)


# filename = 'lut_protocols'
# module = load(filename, "LUT_make_line", header_includes=["uint16_t waveform_lut[];"])
# module.LUT_make_line(3, 6, 2)
# print(module.waveform_lut[5])
# print(module.waveform_lut)
# os.remove(filename+'_.c')


if __name__ == "__main__":
    unittest.main()
