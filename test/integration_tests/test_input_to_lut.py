# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitatus Lopin"

# standard libraries
import unittest

# local files
import test.helper_functions as helper_funcs
import test.integration_tests.solutions as solutions

class InputtoLUT(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        """ Load the file just one time for each test """
        cls._filenames = ['lut_protocols', 'user_selections']
        # make the waveform_lut static, for testing it doesn't matter,
        # and it suppresses an error
        cls.module = helper_funcs.load(cls._filenames,
                                       ["LUT_make_line", "LUT_MakeTriangle_Wave",
                                        "user_lookup_table_maker"],
                                       header_includes=["static uint16_t waveform_lut[];"],
                                       compiled_file_end="input_to_lut")

    def test_LS_input(self):
        index = self.module.user_lookup_table_maker("S|0184|0097|38399|CS")
        print(f"index: {index}")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0, index)
        print(f"waveform: {waveform}")
        self.assertEqual(index, 300)
