# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""
Test that the LUT_MakeCVStartZero function works properly
in the lut_protocols.c file, needs LUT_make_line to work properly
"""

__author__ = "Kyle Vitautus Lopin"

# standard libraries
import unittest

# local files
import test.helper_functions as helper_funcs
from test.unit_tests.test_lut import solutions


class LUTMakeCVStartZero(unittest.TestCase):
    """ Test that the LUT_MakeTriangle_Wave function works properly"""
    _filename = 'lut_protocols'

    @classmethod
    def setUpClass(cls):
        """ Load the file just one time for each test """
        # make the waveform_lut static, for testing it doesn't matter,
        # and it suppresses an error
        cls.module, _ = helper_funcs.load(
            cls._filename, ["LUT_make_line", "LUT_MakeCVStartZero"],
            header_includes=["static uint16_t waveform_lut[];\n"
                             "static uint16_t dac_ground_value;"],
            compiled_file_end="make_CVStartZero")

    def test_down_first(self):
        """ Test it makes a cyclic voltammetry waveform that goes from low to high
            that starts at 0 """
        self.module.dac_ground_value = 100
        index = self.module.LUT_MakeCVStartZero(95, 105)
        waveform = helper_funcs.convert_c_array_to_list(
            self.module.waveform_lut, 0, index)
        soln = solutions.test_cv_zero_down_first
        self.assertEqual(index, len(soln), msg=f"test_up returned and index of {index} "
                                               f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln,
                             msg="test_down_first didn't return the proper lut array")

    def test_up_first(self):
        """ Test it makes a cyclic voltammetry waveform that goes from high to low
            that starts at 0 """
        self.module.dac_ground_value = 100
        index = self.module.LUT_MakeCVStartZero(105, 95)
        print(f"index: {index}")
        waveform = helper_funcs.convert_c_array_to_list(
            self.module.waveform_lut, 0, index)
        print(f"waveform: {waveform}")
        soln = solutions.test_cv_zero_up_first
        self.assertEqual(index, len(soln), msg=f"test_up returned and index of {index} "
                                               f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln,
                             msg="test_down_first didn't return the proper lut array")

    def test_out_of_range(self):
        """ Test it makes a cyclic voltammetry waveform that goes from low to high
            that starts at 0 """
        self.module.dac_ground_value = 100
        index = self.module.LUT_MakeCVStartZero(4000, 0)
        print(f"index: {index}")
        waveform = helper_funcs.convert_c_array_to_list(
            self.module.waveform_lut, 0, index)
        print(f"waveform: {waveform}")
        self.assertEqual(index, 5001, msg=f"test_up returned and index of {index} "
                                          f"instead of 5001")
