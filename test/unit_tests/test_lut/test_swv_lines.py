# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitautus Lopin"

# standard libraries
import sys
import unittest

# local files
from test import helper_functions as helper_funcs
from test.unit_tests.test_lut import solutions


class SWVMakeLineTestCase(unittest.TestCase):
    """ Test that the LUT_make_line works properly

    Attributes:
        _filename (list[str]): names of the c and h files
        used in the integration tests
        module: compiles c module to use for testing
    """
    _filenames = 'lut_protocols'

    @classmethod
    def setUpClass(cls) -> None:
        """ Load the file just one time for each test, and make sure the mock
         cytypes.h (and possible globals.h??) file are in place """
        # helper_funcs.setup_mock_files()
        cls.module, _ = helper_funcs.load(cls._filenames, ["LUT_make_swv_line"],
                                       header_includes=[
                                       "static uint16_t waveform_lut[];"],
                                       compiled_file_end="make_swv_lines")

    @classmethod
    def tearDownClass(cls) -> None:
        helper_funcs.remove_compiled_files()

    def test_swv_up(self):
        """ Test that the square wave voltammetry function works to a
        linear sweep type of voltage protocol in the up direction"""
        index = self.module.LUT_make_swv_line(200, 300, 50, 10, 0)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0,
                                                        index)
        soln = solutions.test_swv_up
        self.assertEqual(index, len(soln),
                         msg=f"test_swv_up returned and index of"
                             f" {index} instead of {len(soln)}")
        self.assertListEqual(waveform, soln,
                             msg="test_swv_up didn't return the proper lut array")

    def test_swv_up2(self):
        index = self.module.LUT_make_swv_line(200, 230, 5, 5, 0)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0,
                                                        index)
        soln = solutions.test_swv_up2
        self.assertEqual(index, len(soln),
                         msg=f"{sys._getframe().f_code.co_name} returned and "
                             f"index of {index} instead of {len(soln)}")

        self.assertListEqual(waveform, soln,
                             msg=f"{sys._getframe().f_code.co_name} "
                                 f"didn't return the proper lut array")

    def test_swv_down(self):
        index = self.module.LUT_make_swv_line(100, 65, 10, 10, 2)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0,
                                                        index)
        print(f"index: {index}")
        print(f"waveform: {waveform}")
        soln = solutions.test_swv_down
        print(f"solution: {soln}")
        self.assertEqual(index, len(soln),
                         msg=f"{sys._getframe().f_code.co_name} returned and "
                             f"index of {index} instead of {len(soln)}")

        self.assertListEqual(waveform, soln)
