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
        cls._filenames = ['lut_protocols', 'user_selections', 'helper_functions']
        # make the waveform_lut static, for testing it doesn't matter,
        # and it suppresses an error
        cls.module = helper_funcs.load(cls._filenames,
                                       ["LUT_make_line", "LUT_MakeTriangle_Wave",
                                        "user_lookup_table_maker", "helper_Convert2Dec"],
                                       header_includes=["static uint16_t waveform_lut[];"],
                                       compiled_file_end="input_to_lut")

    # test making cyclic voltammetry look-up tables
    def test_CV_input(self):
        """Test if the program makes a cyclic voltammetry going down correctly"""
        index = self.module.user_lookup_table_maker(b"S|0105|0095|38399|CS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        self.assertEqual(index, len(solutions.test_input_to_lut1),
                         msg=f"test_LS_input returned an index of {index} "
                             f"instead of {len(solutions.test_input_to_lut1)}")
        self.assertListEqual(waveform, solutions.test_input_to_lut1,
                             msg=f"test_LS_input didn't return the correct array")

    def test_CV_input2(self):
        """Test if the program makes a cyclic voltammetry going up correctly"""
        index = self.module.user_lookup_table_maker(b"S|0090|0110|38399|CS")
        print(f"index: {index}")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        print(f"waveform: {waveform}")
        soln = solutions.test_input_to_lut2
        self.assertEqual(index, len(soln),
                         msg=f"test_LS_input2 returned an index of {index} "
                             f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln,
                             msg=f"test_LS_input2 didn't return the correct array")

    def test_clear_CV_lut(self):
        """Test if the program clears the look-up table and rewrites over the
        look-up table correctly for a cyclic voltammetry call"""
        index = self.module.user_lookup_table_maker(b"S|0105|0095|38399|CS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        self.assertEqual(index, len(solutions.test_input_to_lut1),
                         msg=f"test_LS_input returned an index of {index} "
                             f"instead of {len(solutions.test_input_to_lut1)}")
        self.assertListEqual(waveform, solutions.test_input_to_lut1,
                             msg=f"test_LS_input didn't return the correct array")
        index = self.module.user_lookup_table_maker(b"S|0090|0110|38399|CS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        soln = solutions.test_input_to_lut2
        self.assertEqual(index, len(soln),
                         msg=f"test_LS_input2 returned an index of {index} "
                             f"instead of {len(soln)}")
        self.assertListEqual(waveform, soln,
                             msg=f"look up table is not clearing the lut properly")

    def test_CV_input_out_of_range(self):
        """Test the program will not overwrite the look-up table
        array past its end for a cyclic voltammetry call"""
        index = self.module.user_lookup_table_maker(b"S|0090|3110|38399|CS")
        self.assertEqual(index, 5001,
                         msg=f"test_LS_input_out_of_range returned and "
                             f"index of {index} instead of 5001")

    # test the look-up tables are made correctly when given linear sweep inputs
    def test_LS_input(self):
        """Test if the program makes a cyclic voltammetry going down correctly"""
        index = self.module.user_lookup_table_maker(b"S|0105|0095|38399|CS")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut,
                                                        0, index)
        self.assertEqual(index, len(solutions.test_input_to_lut1),
                         msg=f"test_LS_input returned an index of {index} "
                             f"instead of {len(solutions.test_input_to_lut1)}")
        self.assertListEqual(waveform, solutions.test_input_to_lut1,
                             msg=f"test_LS_input didn't return the correct array")
