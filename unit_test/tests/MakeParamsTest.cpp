#include "CppUTest/TestHarness.h"

extern "C"
{
	//#include "../../Amperometry_v059_2.cydsn/lut_protocols.h"
	#include "helper_functions.c"
	#include "globals.h"
}

TEST_GROUP(LuT)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(LuT, lut_check)
{
	CHECK_EQUAL(1, 1);
}