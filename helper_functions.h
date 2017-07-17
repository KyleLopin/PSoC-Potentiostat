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
#include "globals.h"
#include "DAC.h"
    
/***************************************
*        Variables
***************************************/     
    
extern uint8 selected_voltage_source;
    
    
/***************************************
*        Function Prototypes
***************************************/ 
    
uint8 helper_check_voltage_source(void);
void helper_set_voltage_source(uint8 selected_voltage_source);
void helper_Writebyte_EEPROM(uint8 data, uint16 address);
uint8 helper_Readbyte_EEPROM(uint16 address);

void helper_HardwareSetup(void);
void helper_HardwareStart(void);
void helper_HardwareSleep(void);
void helper_HardwareWakeup(void);

uint16 helper_Convert2Dec(uint8 array[], uint8 len);


#endif

/* [] END OF FILE */
