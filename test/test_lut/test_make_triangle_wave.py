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
        cls.module = helper_funcs.load(cls._filename,
                                       ["LUT_make_line", "LUT_MakeTriangle_Wave"],
                                       header_includes=["static uint16_t waveform_lut[];"],
                                       compiled_file_end="2")

    def test_upright(self):
        """ Test it makes a cyclic voltammetry waveform that goes from low to high
            that starts at 0 """
        index = self.module.LUT_MakeTriangle_Wave(0, 10)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0, index)
        self.assertEqual(index, 22, msg=f"test_up returned and index of {index} "
                                        f"instead of 22")
        self.assertListEqual(waveform, solutions.test_triangle_upright,
                             msg=f"test_upright didn't return the proper lut array")

    def test_upright2(self):
        """ Test it makes a cyclic voltammetry waveform that goes from low to high
            that doesn't start at 0 """
        index = self.module.LUT_MakeTriangle_Wave(100, 105)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0, index)
        self.assertEqual(index, 12, msg=f"test_up returned and index of {index} "
                                        f"instead of 12")
        self.assertListEqual(waveform, solutions.test_triangle_upright2,
                             msg=f"test_upright didn't return the proper lut array")

    def test_downward(self):
        """ Test it makes a cyclic voltammetry waveform that goes from low to high
            down to 0 """
        index = self.module.LUT_MakeTriangle_Wave(10, 0)
        print(f"index: {index}")
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0, index)
        print(f"waveform: {waveform}")
        self.assertEqual(index, 22, msg=f"test_up returned and index of {index} "
                                        f"instead of 22")
        self.assertListEqual(waveform, solutions.test_triangle_downward,
                             msg=f"test_downward didn't return the proper lut array")

    def test_downward2(self):
        """ Test it makes a cyclic voltammetry waveform that goes from low to high
            not to 0 """
        index = self.module.LUT_MakeTriangle_Wave(120, 110)
        waveform = helper_funcs.convert_c_array_to_list(self.module.waveform_lut, 0, index)
        self.assertEqual(index, 22, msg=f"test_up returned and index of {index} "
                                        f"instead of 22")
        self.assertListEqual(waveform, solutions.test_triangle_downward2,
                             msg=f"test_downward didn't return the proper lut array")

    def test_out_of_range(self):
        """ Test if the lut will stop when the index hits 5000 (MAX_LUT_SIZE) """
        index = self.module.LUT_MakeTriangle_Wave(120, 5110)
        print(f"index: {index}")
        self.assertEqual(index, 5001, msg=f"test_out_of_range returned and index of {index} "
                                          f"instead of 5000")
