# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""
Integration test to see if the PSoC embedded firmware will make the correct look-up
table when given input strings
"""

__author__ = "Kyle Vitautus Lopin"

# standard libraries
import unittest

# local files
from test import helper_functions as helper_funcs
from test.integration_tests import solutions


class InputToLUT(unittest.TestCase):
    """ Integration tests to check that input strings into the
    user_selection.c file creates the proper look-up tables.

    Attributes:
        _filenames (list[str]): names of the c and h files
        used in the integration tests
    """
    _filenames = []

    @classmethod
    def setUpClass(cls):
        """ Load the file just one time for each test """
        cls._filenames = ['lut_protocols', 'user_selections', 'helper_functions']
        # make the waveform_lut static, for testing it doesn't matter,
        # and it suppresses an error
        cls.module = helper_funcs.load(cls._filenames,
                       ["LUT_make_line", "LUT_MakeTriangle_Wave",
                        "user_lookup_table_maker", "helper_Convert2Dec"],
                        header_includes=["static uint16_t waveform_lut[];"],
                        compiled_file_end="input_to_lut")

    # test making cyclic voltammetry look-up tables
    def test_cv_input(self):
        """Test if the program makes a cyclic voltammetry going down correctly"""
        index = self.module.user_lookup_table_maker(b"S|0105|0095|38399|CS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        self.assertEqual(index, len(solutions.test_input_cv_to_lut1),
                         msg=f"test_LS_input returned an index of {index} "
                             f"instead of {len(solutions.test_input_cv_to_lut1)}")
        self.assertListEqual(waveform, solutions.test_input_cv_to_lut1,
                             msg="test_LS_input didn't return the correct array")

    def test_cv_input2(self):
        """Test if the program makes a cyclic voltammetry going up correctly"""
        index = self.module.user_lookup_table_maker(b"S|0090|0110|38399|CS")
        print(f"index: {index}")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        print(f"waveform: {waveform}")
        soln = solutions.test_input_cv_to_lut2
        self.assertEqual(index, len(soln),
                         msg=f"test_LS_input2 returned an index of {index} "
                             f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln,
                             msg="test_LS_input2 didn't return the correct array")

    def test_clear_cv_lut(self):
        """Test if the program clears the look-up table and rewrites over the
        look-up table correctly for a cyclic voltammetry call"""
        index = self.module.user_lookup_table_maker(b"S|0105|0095|38399|CS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        self.assertEqual(index, len(solutions.test_input_cv_to_lut1),
                         msg=f"test_LS_input returned an index of {index} "
                             f"instead of {len(solutions.test_input_cv_to_lut1)}")
        self.assertListEqual(waveform, solutions.test_input_cv_to_lut1,
                             msg="test_LS_input didn't return the correct array")
        index = self.module.user_lookup_table_maker(b"S|0090|0110|38399|CS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        soln = solutions.test_input_cv_to_lut2
        self.assertEqual(index, len(soln),
                         msg=f"test_LS_input2 returned an index of {index} "
                             f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln,
                             msg="look up table is not clearing the lut properly")

    def test_cv_input_out_of_range(self):
        """Test the program will not overwrite the look-up table
        array past its end for a cyclic voltammetry call"""
        index = self.module.user_lookup_table_maker(b"S|0090|3110|38399|CS")
        self.assertEqual(index, 5001,
                         msg=f"test_LS_input_out_of_range returned and "
                             f"index of {index} instead of 5001")

    # test the look-up tables are made correctly when given linear sweep inputs
    def test_ls_input(self):
        """Test if the program makes a cyclic voltammetry going down correctly"""
        index = self.module.user_lookup_table_maker(b"S|0135|0125|38399|LS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        print(f"index: {index}")
        print(f"waveform: {waveform}")
        soln = solutions.test_input_ls_to_lut1
        self.assertEqual(index, len(soln),
                         msg=f"test_ls_input returned an index of {index} "
                             f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln,
                             msg="test_ls_input didn't return the correct array")

    def test_ls_input2(self):
        """Test if the program makes a cyclic voltammetry going down correctly"""
        index = self.module.user_lookup_table_maker(b"S|0125|0135|38399|LS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        print(f"index: {index}")
        print(f"waveform: {waveform}")
        soln = solutions.test_input_ls_to_lut2
        self.assertEqual(index, len(soln),
                         msg=f"test_ls_input2 returned an index of {index} "
                             f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln,
                             msg="test_ls_input2 didn't return the correct array")

    def test_clear_ls_lut(self):
        """Test if the program clears the look-up table and rewrites over the
        look-up table correctly for a cyclic voltammetry call"""
        index = self.module.user_lookup_table_maker(b"S|0135|0125|38399|LS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        soln = solutions.test_input_ls_to_lut1
        self.assertEqual(index, len(soln),
                         msg=f"test_LS_input returned an index of {index} "
                             f"instead of {len(solutions.test_input_cv_to_lut1)}")
        self.assertListEqual(waveform, soln,
                             msg="test_LS_input didn't return the correct array")
        index = self.module.user_lookup_table_maker(b"S|0125|0135|38399|LS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        soln = solutions.test_input_ls_to_lut2
        self.assertEqual(index, len(soln),
                         msg=f"test_LS_input2 returned an index of {index} "
                             f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln,
                             msg="look up table is not clearing the lut properly")

    def test_ls_input_out_of_range(self):
        """Test the program will not overwrite the look-up table
        array past its end for a cyclic voltammetry call"""
        index = self.module.user_lookup_table_maker(b"S|0090|5110|38399|LS")
        self.assertEqual(index, 5001,
                         msg=f"test_LS_input_out_of_range returned and "
                             f"index of {index} instead of 5001")
