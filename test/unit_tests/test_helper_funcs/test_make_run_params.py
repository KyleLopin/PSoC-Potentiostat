# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""
TODO: convert to pytest
TODO: add error checking to the make_run_params fucntion in helper_functions.c
just a minimal set of tests now
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
    helper_include = \
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

    @classmethod
    def setUpClass(cls) -> None:
        """ Load the file just one time for each test """

        cls.module, cls.ffi = helper_funcs.load(cls._filenames, cls._function_names,
                                                header_includes=cls.helper_include,
                                                compiled_file_end="helper_func")

    def test_make_run_params(self):
        run_params = self.ffi.new("struct RunParams *")
        return_params = self.module.make_run_params(b"S|0105|0095|38399|CS",
                                                    run_params)
        self.assertEqual(return_params.start_value, 105,
                         msg="make_run_params not correctly setting start voltage")
        self.assertEqual(return_params.end_value, 95,
                         msg="make_run_params not correctly setting start voltage")
        self.assertEqual(return_params.sweep_type, 67,  # 67 - 'C'
                         msg="make_run_params not correctly setting sweep type")
        self.assertEqual(return_params.start_volt_type, 83,  # 83 - 'S'
                         msg="make_run_params not correctly start voltage type")
        self.assertEqual(return_params.use_swv, False,  # S at start so no swv
                         msg="make_run_params not correctly setting use_swv")

    def test_make_run_params2(self):
        run_params = self.ffi.new("struct RunParams *")
        return_params = self.module.make_run_params(b"S|3258|1025|24299|LS",
                                                    run_params)
        self.assertEqual(return_params.start_value, 3258,
                         msg="make_run_params not correctly setting start voltage")
        self.assertEqual(return_params.end_value, 1025,
                         msg="make_run_params not correctly setting start voltage")
        self.assertEqual(return_params.sweep_type, ord('L'),  # 76 - 'L'
                         msg="make_run_params not correctly setting sweep type")
        self.assertEqual(return_params.start_volt_type, 83,  # 83 - 'S'
                         msg="make_run_params not correctly start voltage type")
        self.assertEqual(return_params.use_swv, False,  # 0 or False
                         msg="make_run_params not correctly setting use_swv")

    def test_make_run_params_swv(self):
        run_params = self.ffi.new("struct RunParams *")
        return_params = self.module.make_run_params(b"G|0131|0125|0122|0128|23999|LS",
                                                    run_params)
        print(return_params.start_value)
        print(return_params.end_value)

        print(return_params.start_volt_type)
        print(return_params.sweep_type)
        print(return_params.swv_inc)
        print(return_params.swv_pulse_height)
        print(return_params.timer_period)
        print(return_params.use_swv)
        print(dir(return_params))
        self.assertEqual(return_params.start_value, 131,
                         msg="make_run_params not correctly setting start voltage")
        self.assertEqual(return_params.end_value, 125,
                         msg="make_run_params not correctly setting start voltage")
        self.assertEqual(return_params.sweep_type, ord('L'),
                         msg="make_run_params not correctly setting sweep type")
        self.assertEqual(return_params.start_volt_type, ord('S'),
                         msg="make_run_params not correctly start voltage type")
        self.assertEqual(return_params.use_swv, True,  # 0 or False
                         msg="make_run_params not correctly setting use_swv")
        self.assertEqual(return_params.swv_inc, 122,
                         msg="make_run_params not correctly setting start voltage")
        self.assertEqual(return_params.swv_pulse_height, 128,
                         msg="make_run_params not correctly setting start voltage")
