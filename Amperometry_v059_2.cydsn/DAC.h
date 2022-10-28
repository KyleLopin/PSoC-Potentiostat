/*********************************************************************************
* File Name: DAC.h
*
* Description:
*  This file contains the function prototypes and constants used for
*  the custom DAC, an 8-bit VDAC or DVDAC selectable by the user
*
**********************************************************************************
 * Copyright Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#if !defined(DAC_H)
#define DAC_H

#include <project.h>
#include "cytypes.h"
#include "helper_functions.h"
#include "globals.h"

/**************************************
*        AMuX API Constants
**************************************/
    
#define DVDAC_channel 1
#define VDAC_channel 0
    
    
/***************************************
*        Variables
***************************************/     
    
uint8_t selected_voltage_source;
extern uint16_t dac_ground_value;
    
    
/***************************************
*        Function Prototypes
***************************************/ 
    
void DAC_Start(void);
void DAC_Sleep(void);
void DAC_Wakeup(void);
void DAC_SetValue(uint16_t value);
    
#endif
/* [] END OF FILE */
