# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitautus Lopin"

# standard libraries
import unittest

# local files
import test.helper_functions as helper_funcs
import test.test_lut.solutions as solutions


class LuTMakeTriangleWave(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        """ Load the file just one time for each test """
        cls._filename = 'lut_protocols'
        # make the waveform_lut static, for testing it doesn't matter,
        # and it suppresses an error
        cls.module = helper_funcs.load(cls._filename, ["LUT_make_line"],
                                       header_includes=["static uint16_t waveform_lut[];"],
                                       compiled_file_end="2")

    def test1(self):
        pass
