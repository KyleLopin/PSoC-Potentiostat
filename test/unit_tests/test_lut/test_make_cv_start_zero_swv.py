# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""
Test that the make cyclic voltammetry with square wave voltammetry starting at 0
volts makes the correct look-up tables.
TODO: add error checking
"""

__author__ = "Kyle Vitautas Lopin"

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
            cls._filename, ["LUT_make_line", "LUT_MakeCVStartZero_SWV"],
            header_includes=["static uint16_t waveform_lut[];\n"
                             "static uint16_t dac_ground_value;"],
            compiled_file_end="make_CVStartZero_SWV")

    def test_up_first_swv(self):
        """ Test it makes a cyclic voltammetry waveform with a square wave
        that goes from high to low that starts at 0 """
        self.module.dac_ground_value = 100
        index = self.module.LUT_MakeCVStartZero_SWV(120, 80, 30, 5)
        print(f"index: {index}")
        waveform = helper_funcs.convert_c_array_to_list(
            self.module.waveform_lut, 0, index)
        soln = solutions.test_up_first_swv
        print(f"waveform: {waveform}")
        print(f"solution: {soln}")

        self.assertEqual(index, len(soln), msg=f"test_up returned and index of {index} "
                                               f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln)

    def test_down_first_swv(self):
        """ Test it makes a cyclic voltammetry waveform with a square wave
        that goes from high to low that starts at 0 """
        self.module.dac_ground_value = 100
        index = self.module.LUT_MakeCVStartZero_SWV(80, 120, 30, 5)
        print(f"index: {index}")
        waveform = helper_funcs.convert_c_array_to_list(
            self.module.waveform_lut, 0, index)
        soln = solutions.test_down_first_swv
        print(f"waveform: {waveform}")
        print(f"solution: {soln}")

        self.assertEqual(index, len(soln), msg=f"test_up returned and index of {index} "
                                               f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln)
