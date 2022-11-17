/*******************************************************************************
* File Name: helper_functions.h
*
* Description:
*  Functions prototypes and their variables used by main.
*
**********************************************************************************
 * Copyright Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#if !defined(HELPER_FUNCTIONS_H)
#define HELPER_FUNCTIONS_H

#include <project.h>
#include "cytypes.h"
#include "stdio.h"  // gets rid of the type errors
#include "globals.h"
#include "DAC.h"
    
/***************************************
*        Variables
***************************************/     
    
extern uint8_t selected_voltage_source;
//extern struct RunParams run_params;
    
    
/***************************************
*        Function Prototypes
***************************************/ 

uint8_t helper_check_voltage_source(void);
void helper_set_voltage_source(uint8_t selected_voltage_source);
uint8_t helper_Writebyte_EEPROM(uint8_t data, uint16_t address);
uint8_t helper_Readbyte_EEPROM(uint16_t address);

void helper_HardwareSetup(void);
void helper_HardwareStart(void);
void helper_HardwareSleep(void);
void helper_HardwareWakeup(void);
void make_run_params(const uint8_t data_buffer[], struct RunParams *run_params);
uint16_t helper_Convert2Dec(const uint8_t array[], const uint8_t len);


#endif

/* [] END OF FILE */
