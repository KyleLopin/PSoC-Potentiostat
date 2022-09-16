//- Copyright (c) 2008-2013 James Grenning --- All rights reserved
//- For exclusive use by participants in Renaissance Software Consulting training courses.
//- Cannot be used by attendees to train others without written permission.
//- www.renaissancesoftware.net james@renaissancesoftware.net


#include "CppUTest/CommandLineTestRunner.h"

#include "CppUTest/TestHarness.h"

extern "C" {
  #include "../../Amperometry_v059_2.cydsn/lut_protocols.h"
}
#include <unistd.h>
#include <stdio.h>
#include <limits.h>


// int main() {
//    char cwd[256];
//    if (getcwd(cwd, sizeof(cwd)) != NULL) {
//        printf("Current working dir: %s\n", cwd);
//    } 
//    else {
//        perror("getcwd() error");
//        return 1;
//    }
// 	CHECK_EQUAL(1, 1);
// 	printf("Hello\n");
// 	return 0;
// }
int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

