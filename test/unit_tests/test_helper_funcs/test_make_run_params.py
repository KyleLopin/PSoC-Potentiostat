# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitautus Lopin"

# standard libraries
import unittest

# local files
from test import helper_functions as helper_funcs
from test.unit_tests.test_helper_funcs import solutions


class MakeRunParamsTestCase(unittest.TestCase):
    """ Test cases where you pass an input string to user_lookup_table_maker
    function in user_selections.c and check that the settings in the
    run_params variable are correct

    Attributes:
        _filenames (list[str]): names of the c and h files
        used in the integration test
        _function_names (list[str]): names of the functions that will be tested
    """
    _filenames = 'helper_functions'
    _function_names = ["make_run_params"]

    @classmethod
    def setUpClass(cls) -> None:
        """ Load the file just one time for each test """
        cls.helper_include = \
            ["""struct RunParams {
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
            
            struct RunParams run_params;"""]

        cls.module, cls.ffi = helper_funcs.load(cls._filenames, cls._function_names,
                                                header_includes=cls.helper_include,
                                                compiled_file_end="helper_func")

    def test_make_run_params(self):
        run_params = self.ffi.new("struct RunParams *")
        index = self.module.make_run_params(b"S|0105|0095|38399|CS",
                                            run_params)
        print(self.module.run_params)
