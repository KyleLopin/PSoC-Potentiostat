/*********************************************************************************
* File Name: DAC.c
*
* Description:
*  An abstraction of 2 different DACs, an 8-bit VDAC or 12-bit DVDAC.
*  This file contains the source code for
*  the custom DAC, an 8-bit VDAC or 12-bit DVDAC selectable by the user
*
**********************************************************************************
 * Copyright Naresuan University, Phitsanulok Thailand
 * Released under Creative Commons Attribution-ShareAlike  3.0 (CC BY-SA 3.0 US)
*********************************************************************************/

#include "DAC.h"
#include "globals.h"

/******************************************************************************
* Function Name: DAC_Start
*******************************************************************************
*
* Summary:
*  Start the correct voltage source.  
*  Figure what source is being used, set the  correct AMux settings and start 
*  the correct source
*
* Parameters:
*
*
*  Global variables:
*  selected_voltage_source:  voltage source that is set to run, 
*      [VDAC_IS_VDAC or VDAC_IS_DVDAC]
*
*******************************************************************************/

void DAC_Start(void) {
    selected_voltage_source = helper_check_voltage_source();  // check which DAC is being used
    
    if (selected_voltage_source == VDAC_IS_DVDAC) {
        DVDAC_Start();
        dac_ground_value = VIRTUAL_GROUND;  //VIRTUAL_GROUND / 1 mV the value of the DAC that will make 0 V across the working and aux electrodes 
        AMux_V_source_Select(DVDAC_channel);
    }
    else {
        VDAC_source_Start();
        dac_ground_value = VIRTUAL_GROUND / 16;  // value of dac to make 0 V across working and aux electrodes
        AMux_V_source_Select(VDAC_channel);
    }
}

/******************************************************************************
* Function Name: DAC_Sleep
*******************************************************************************
*
* Summary:
*  Put to sleep the correct voltage source
*
* Parameters:
*
*
* Global variables:
*  selected_voltage_source:  voltage source that is set to run, 
*      [VDAC_IS_VDAC or VDAC_IS_DVDAC]
*
*******************************************************************************/

void DAC_Sleep(void) {
    if (selected_voltage_source == VDAC_IS_DVDAC) {
        DVDAC_Sleep();
    }
    else {
        VDAC_source_Sleep();
    }
}


/******************************************************************************
* Function Name: DAC_Wakeup
*******************************************************************************
*
* Summary:
*  Wake up the correct voltage source
*
* Parameters:
*
*
* Global variables:
*  selected_voltage_source:  voltage source that is set to run, 
*      [VDAC_IS_VDAC or VDAC_IS_DVDAC]
*
*******************************************************************************/

void DAC_Wakeup(void) {
    if (selected_voltage_source == VDAC_IS_DVDAC) {
        DVDAC_Wakeup();
    }
    else {
        VDAC_source_Wakeup();
    }
}


/******************************************************************************
* Function Name: DAC_SetValue
*******************************************************************************
*
* Summary:
*  Set the value of the correct voltage source
*
* Parameters:
*  uint16 value: number to place in the appropriate DAC
*
* Global variables:
*  selected_voltage_source:  voltage source that is set to run, 
*      [VDAC_IS_VDAC or VDAC_IS_DVDAC]
*
*******************************************************************************/

void DAC_SetValue(uint16 value) {
    
    if (selected_voltage_source == VDAC_IS_DVDAC) {
        DVDAC_SetValue(value);
    }
    else {
        VDAC_source_SetValue(value);
    }
}



/* [] END OF FILE */
