# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""
For the look-up table (lut) tests, this provides the solution test arrays
"""

__author__ = "Kyle Vitatus Lopin"

# correct waveform_lut values for test_make_lines tests
test_make_lines_up = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
test_make_lines_up2 = [0, 0, 0, 0, 0, 15, 16, 17, 18, 19, 20]
test_make_lines_down = [50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40]
test_make_lines_down2 = [0, 0, 0, 0, 0, 40, 39, 38, 37, 36, 35]
test_make_lines_out_of_range = [20, 21, 22, 23, 24]

# correct waveform_lut values for the test_make_triangle_wave tests
test_triangle_upright = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0]
test_triangle_upright2 = [100, 101, 102, 103, 104, 105, 104, 103, 102, 101, 100, 100]
test_triangle_downward = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10]
test_triangle_downward2 = [120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110,
                           111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 120]

# correct waveform_lut values for the test_make_cv_start_zero tests
test_cv_zero_down_first = [100, 99, 98, 97, 96, 95, 96, 97, 98, 99, 100, 101, 102, 103,
                           104, 105, 104, 103, 102, 101, 100, 100]
test_cv_zero_up_first = [100, 101, 102, 103, 104, 105, 104, 103, 102, 101, 100, 99, 98,
                         97, 96, 95, 96, 97, 98, 99, 100, 100]

