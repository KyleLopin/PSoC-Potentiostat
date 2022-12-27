# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitautus Lopin"

# standard libraries
import unittest

# local files
import test.helper_functions as helper_funcs
import test.unit_tests.test_lut.solutions as solutions


class RunParamMaker(unittest.TestCase):
    """ Test cases where you pass an input string to user_lookup_table_maker
    function in user_selections.c and check that the settings in the
    run_params variable are correct

    """
    @classmethod
    def setUpClass(cls):
        """ Load the file just one time for each test """
        cls._filename = 'lut_protocols'
        # make the waveform_lut static, for testing it doesn't matter,
        # and it suppresses an error
        cls.module = helper_funcs.load(cls._filename,
           ["LUT_make_line", "LUT_MakeCVStartZero"],
           header_includes=["""struct RunParams {
                uint16_t start_value;
                uint16_t end_value;
                uint8_t sweep_type;  // Can be C - cyclic voltammetry, or L for linear sweep
                // Can be S for start voltage (triange shape sweep) or Z for starting at zero volts
                uint8_t start_volt_type;
                uint8_t use_swv;
                uint16_t swv_inc;
                uint16_t swv_pulse_height;
                uint16_t timer_period;
            };
        
        struct RunParams run_params;"""],
                                       compiled_file_end="make_CVStartZero")
        
