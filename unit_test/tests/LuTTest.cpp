#include "CppUTest/TestHarness.h"

extern "C"
{
	//#include "../../Amperometry_v059_2.cydsn/lut_protocols.h"
	#include "lut_protocols.c"
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
	//uint16 waveform_lut[5000];
	int index = LUT_MakeTriangle_Wave(0, 10);
	//int index = LUT_test();
	//int index = 1;
    printf("%i\n", index);
    printf("waveform: %i", waveform_lut[2]);
}