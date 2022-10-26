# Copyright (c) 2022 Kyle Lopin (Naresuan University) <kylel@nu.ac.th>

"""

"""

__author__ = "Kyle Vitatus Lopin"


def convert_c_array_to_list(c_array, number_elements):
    return_list = []
    for i in range(number_elements):
        return_list.append(c_array[i])
    return return_list
