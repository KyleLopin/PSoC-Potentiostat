# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitautus Lopin"

# standard libraries
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
    @classmethod
    def setUpClass(cls) -> None:
        """ Load the file just one time for each test """
        cls._filename = 'lut_protocols'
        cls.module = helper_funcs.load(cls._filename, ["LUT_make_swv_line"],
                                       header_includes=[
                                       "static uint16_t waveform_lut[];"],
                                       compiled_file_end="make_swv_lines")

    def test_swv_up(self):
        index = self.module.LUT_make_swv_line(200, 300, 10, 100, 0)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0,
                                                        index)
        print(f"index: {index}")
        print(f"waveform: {waveform}")
        soln = solutions.test_swv_up
        self.assertEqual(index, len(soln),
                         msg=f"test_swv_up returned and index of"
                             f" {index} instead of {len(soln)}")
