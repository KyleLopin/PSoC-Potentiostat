# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""
Integration test to see if the PSoC embedded firmware will make the correct look-up
table when given input strings, ***using the SWV*** protocol

"""

__author__ = "Kyle Vitautus Lopin"


# standard libraries
import unittest

# local files
from test import helper_functions as helper_funcs
from test.integration_tests import solutions


class InputToLUTSWV(unittest.TestCase):
    _filenames = ['helper_functions', 'user_selections', 'lut_protocols']

    @classmethod
    def setUpClass(cls):
        """ Load the file just one time for each test """
        # make the waveform_lut static, for testing it doesn't matter,
        # and it suppresses an error
        cls.module, _ = helper_funcs.load(cls._filenames,
                                          ["LUT_make_line", "LUT_MakeTriangle_Wave",
                                           "user_lookup_table_maker", "LUT_Convert2Dec"],
                                          header_includes=["static uint16_t waveform_lut[];"],
                                          compiled_file_end="input_to_lut_swv")

    @classmethod
    def tearDownClass(cls) -> None:
        helper_funcs.remove_compiled_files()

    def test_swv_cv_input(self):
        index = self.module.user_lookup_table_maker(b"S|0105|0095|38399|CS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        print(f"index: {index}")
        print(f"waveform: {waveform}")
