# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""
Test that the LUT_make_line function works properly in the
lut_protocols.c file
"""

__author__ = "Kyle Vitautus Lopin"

# standard libraries
import unittest

# local files
import test.helper_functions as helper_funcs
import test.unit_tests.test_lut.solutions as solutions


class LuTMakeLineTestCase(unittest.TestCase):
    """ Test that the LUT_make_line works properly

    Attributes:
        _filename (list[str]): names of the c and h files
        used in the integration tests
        module: compiles c module to use for testing
    """
    @classmethod
    def setUpClass(cls):
        """ Load the file just one time for each test """
        cls._filename = 'lut_protocols'
        cls.module = helper_funcs.load(cls._filename, ["LUT_make_line"],
                                       header_includes=["static uint16_t waveform_lut[];"],
                                       compiled_file_end="make_lines")

    def setUp(self) -> None:
        """ Reset the waveform_lut for each test """
        self.module.waveform_lut = [0, 0, 0, 0, 0]

    def test_up(self):
        """ Test LUT make line function going up """
        index = self.module.LUT_make_line(0, 10, 0)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0, index)
        self.assertEqual(index, 11, msg=f"test_up returned and index of {index} "
                                        f"instead of 11")
        self.assertListEqual(waveform, solutions.test_make_lines_up,
                             msg=f"test_up didn't return the proper lut array")

    def test_up2(self):
        """ Test LUT make line function going up not from the start """
        index = self.module.LUT_make_line(15, 20, 5)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0, index)
        self.assertEqual(index, 11, msg=f"test_up2 returned and index of {index} "
                                        f"instead of 11")
        self.assertListEqual(waveform, solutions.test_make_lines_up2,
                             msg=f"test_up2 didn't return the proper lut array")

    def test_down(self):
        """ Test LUT make line function going down """
        index = self.module.LUT_make_line(50, 40, 0)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0, index)
        self.assertEqual(index, 11, msg=f"test_down returned and index of {index} "
                                        f"instead of 11")
        self.assertListEqual(waveform, solutions.test_make_lines_down,
                             msg=f"test_down didn't return the proper lut array")

    def test_down2(self):
        """ Test LUT make line function going down offset from the start """
        index = self.module.LUT_make_line(40, 35, 5)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0, index)
        self.assertEqual(index, 11, msg=f"test_down returned and index of {index} "
                                        f"instead of 11")
        self.assertListEqual(waveform, solutions.test_make_lines_down2,
                             msg=f"test_down2 didn't return the proper lut array")

    def test_out_of_range(self):
        """ Test if the lut will stop when the index hits 5000 (MAX_LUT_SIZE) """
        start_index = 4995
        index = self.module.LUT_make_line(20, 40, start_index)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, start_index, index)
        self.assertEqual(index, 5000, msg=f"test_out_of_range returned and index of {index} "
                                          f"instead of 5000")
        self.assertListEqual(waveform, solutions.test_make_lines_out_of_range,
                             msg=f"test_out_of_range didn't return the proper lut array")
